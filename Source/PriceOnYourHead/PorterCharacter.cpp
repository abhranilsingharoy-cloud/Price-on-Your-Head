#include "PorterCharacter.h"
#include "PriceOnYourHeadGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

APorterCharacter::APorterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 380.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;

	// Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Holographic Bounty Text above head
	BountyHoloText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BountyHoloText"));
	BountyHoloText->SetupAttachment(RootComponent);
	BountyHoloText->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	BountyHoloText->SetHorizontalAlignment(EHTA_Center);
	BountyHoloText->SetTextRenderColor(FColor(0, 255, 255, 255));
	BountyHoloText->SetWorldSize(32.0f);

	EquippedTools[0] = EHeistTool::FakeLotTag;
	EquippedTools[1] = EHeistTool::DecoyPaddle;
	ActiveToolIndex = 0;
}

void APorterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Waypoints for Autoplay verification
	AutoplayWaypoints.Add(FVector(400.0f, 150.0f, 0.0f));   // Bonus Plinth 1
	AutoplayWaypoints.Add(FVector(800.0f, -200.0f, 0.0f));  // Decoy Paddle plinth
	AutoplayWaypoints.Add(FVector(1200.0f, 0.0f, 0.0f));    // Lot One: Frozen Head Cryo Canister
	AutoplayWaypoints.Add(FVector(1800.0f, 300.0f, 0.0f));   // Escape Skiff
}

void APorterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FakeTagActiveTimer > 0.0f)
	{
		FakeTagActiveTimer -= DeltaSeconds;
	}

	UpdateHoloPriceDisplay();

	APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->bIsAutoplay && GM->GetHeistState() == EHeistState::InHeist)
	{
		AutoplayStealthStep(DeltaSeconds);
	}
}

void APorterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APorterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APorterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APorterCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APorterCharacter::StopCrouch);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &APorterCharacter::InteractGrab);
	PlayerInputComponent->BindAction("UseTool", IE_Pressed, this, &APorterCharacter::ActivateTool);
	PlayerInputComponent->BindAction("SelectTool1", IE_Pressed, this, &APorterCharacter::SelectTool1);
	PlayerInputComponent->BindAction("SelectTool2", IE_Pressed, this, &APorterCharacter::SelectTool2);

	PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &APorterCharacter::ConfirmStartHeist);
	PlayerInputComponent->BindKey(EKeys::Enter, IE_Pressed, this, &APorterCharacter::ConfirmStartHeist);
}

void APorterCharacter::ConfirmStartHeist()
{
	APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->GetHeistState() == EHeistState::Planning)
	{
		GM->StartHeist();
	}
}

void APorterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APorterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APorterCharacter::StartCrouch()
{
	Crouch();
}

void APorterCharacter::StopCrouch()
{
	UnCrouch();
}

int64 APorterCharacter::GetCurrentBounty() const
{
	if (FakeTagActiveTimer > 0.0f)
	{
		return 0; // Fake lot tag zeroes drone readouts temporarily
	}

	int64 Bounty = 0;
	if (bCarryingLotOne)
	{
		Bounty += 1000000; // Lot One: $1,000,000
	}
	Bounty += (int64)BonusLotsCarried * 250000;
	return Bounty;
}

void APorterCharacter::InteractGrab()
{
	// Trace forward to grab lot
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 180.0f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(50.0f), Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Check if plinth or lot
			if (!bCarryingLotOne)
			{
				bCarryingLotOne = true;
				APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
				if (GM)
				{
					GM->AddHaul(1000000);
				}
				UE_LOG(LogTemp, Log, TEXT("Picked up LOT ONE: Billionaire's Frozen Head!"));
			}
			else
			{
				BonusLotsCarried++;
				APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
				if (GM)
				{
					GM->AddHaul(250000);
				}
				UE_LOG(LogTemp, Log, TEXT("Picked up Bonus Lot! Total bonus: %d"), BonusLotsCarried);
			}
		}
	}
}

void APorterCharacter::ActivateTool()
{
	EHeistTool Tool = EquippedTools[ActiveToolIndex];
	APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));

	switch (Tool)
	{
	case EHeistTool::FakeLotTag:
		FakeTagActiveTimer = 8.0f;
		UE_LOG(LogTemp, Log, TEXT("Used Fake Lot Tag: Bounty hidden from drones for 8s!"));
		break;
	case EHeistTool::DecoyPaddle:
		if (GM)
		{
			GM->TriggerBiddingWar(GetActorLocation(), 5.0f);
		}
		break;
	case EHeistTool::SmokeCartridge:
		UE_LOG(LogTemp, Log, TEXT("Deployed Smoke Cartridge: Line of sight obscured for 6s!"));
		break;
	default:
		break;
	}
}

void APorterCharacter::SelectTool1()
{
	ActiveToolIndex = 0;
}

void APorterCharacter::SelectTool2()
{
	ActiveToolIndex = 1;
}

void APorterCharacter::UpdateHoloPriceDisplay()
{
	int64 Bounty = GetCurrentBounty();
	if (Bounty <= 0)
	{
		BountyHoloText->SetText(FText::FromString(TEXT("LOT: --")));
		BountyHoloText->SetTextRenderColor(FColor(0, 255, 255, 180));
	}
	else
	{
		FString PriceStr = FString::Printf(TEXT("$%lld"), Bounty);
		BountyHoloText->SetText(FText::FromString(PriceStr));

		// Cyan for low bounty, Red warning for high bounty
		if (Bounty >= 1000000)
		{
			BountyHoloText->SetTextRenderColor(FColor(255, 40, 40, 255));
		}
		else
		{
			BountyHoloText->SetTextRenderColor(FColor(0, 230, 255, 255));
		}
	}
}

void APorterCharacter::AutoplayStealthStep(float DeltaSeconds)
{
	if (CurrentWaypointIndex >= AutoplayWaypoints.Num())
	{
		// Autoplay reached extraction skiff
		APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
		if (GM && GM->GetHeistState() == EHeistState::InHeist)
		{
			GM->TriggerWin();
		}
		return;
	}

	FVector Target = AutoplayWaypoints[CurrentWaypointIndex];
	FVector CurrentPos = GetActorLocation();
	FVector Dir = (Target - CurrentPos);
	Dir.Z = 0.0f;
	float Dist = Dir.Size();

	if (Dist < 80.0f)
	{
		// Arrived at waypoint, interact or trigger tool
		if (CurrentWaypointIndex == 0)
		{
			BonusLotsCarried++;
			APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
			if (GM) GM->AddHaul(250000);
		}
		else if (CurrentWaypointIndex == 1)
		{
			ActivateTool(); // Trigger decoy paddle
		}
		else if (CurrentWaypointIndex == 2)
		{
			bCarryingLotOne = true;
			APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
			if (GM) GM->AddHaul(1000000);
		}

		CurrentWaypointIndex++;
	}
	else
	{
		Dir.Normalize();
		AddMovementInput(Dir, 1.0f);
	}
}
