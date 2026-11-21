#include "PriceOnYourHeadGameMode.h"
#include "PorterCharacter.h"
#include "PriceOnYourHeadHUD.h"
#include "BargeLevelGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"

APriceOnYourHeadGameMode::APriceOnYourHeadGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APorterCharacter::StaticClass();
	HUDClass = APriceOnYourHeadHUD::StaticClass();
}

void APriceOnYourHeadGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Ensure the Barge Environment exists
	AActor* ExistingGen = UGameplayStatics::GetActorOfClass(this, ABargeLevelGenerator::StaticClass());
	if (!ExistingGen)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<ABargeLevelGenerator>(ABargeLevelGenerator::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	// Parse Autoplay command line flags
	const TCHAR* CmdLine = FCommandLine::Get();
	if (FParse::Param(CmdLine, TEXT("autoplay")))
	{
		bIsAutoplay = true;
		UE_LOG(LogTemp, Log, TEXT("APriceOnYourHeadGameMode: Autoplay mode ENABLED"));
	}
	if (FParse::Param(CmdLine, TEXT("autoplayquit")))
	{
		bAutoplayQuit = true;
	}

	CurrentState = EHeistState::Planning;
	RemainingGavelSeconds = 90.0f;
	CurrentHaulScore = 0;
	BiddingWarTimer = 0.0f;
	ElapsedHeistSeconds = 0.0f;
	QAViolationCount = 0;

	// In autoplay, immediately finish planning after 1.5s
	if (bIsAutoplay)
	{
		FTimerHandle PlanTimer;
		GetWorldTimerManager().SetTimer(PlanTimer, this, &APriceOnYourHeadGameMode::StartHeist, 1.5f, false);
	}
}

void APriceOnYourHeadGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentState == EHeistState::InHeist)
	{
		ElapsedHeistSeconds += DeltaSeconds;
		RemainingGavelSeconds -= DeltaSeconds;

		if (BiddingWarTimer > 0.0f)
		{
			BiddingWarTimer -= DeltaSeconds;
		}

		if (RemainingGavelSeconds <= 0.0f)
		{
			RemainingGavelSeconds = 0.0f;
			TriggerLossGavelSold();
		}

		CheckQARules(DeltaSeconds);

		if (bIsAutoplay)
		{
			UpdateAutoplay(DeltaSeconds);
		}
	}
}

void APriceOnYourHeadGameMode::StartHeist()
{
	if (CurrentState == EHeistState::Planning)
	{
		CurrentState = EHeistState::InHeist;
		UE_LOG(LogTemp, Log, TEXT("APriceOnYourHeadGameMode: Heist Started. Gavel timer running."));
	}
}

void APriceOnYourHeadGameMode::TriggerWin()
{
	if (CurrentState == EHeistState::InHeist)
	{
		CurrentState = EHeistState::Won;
		FString Medal = (CurrentHaulScore >= 1500000 && ElapsedHeistSeconds <= 65.0f) ? TEXT("GOLD") :
		                (CurrentHaulScore >= 1000000) ? TEXT("SILVER") : TEXT("BRONZE");

		UE_LOG(LogTemp, Log, TEXT("=== HEIST WON! Medal: %s | Final Haul: $%lld | Time: %.1fs ==="), *Medal, CurrentHaulScore, ElapsedHeistSeconds);

		if (bAutoplayQuit)
		{
			FGenericPlatformMisc::RequestExit(false);
		}
	}
}

void APriceOnYourHeadGameMode::TriggerLossPinned()
{
	if (CurrentState == EHeistState::InHeist)
	{
		CurrentState = EHeistState::LostPinned;
		UE_LOG(LogTemp, Warning, TEXT("=== HEIST FAILED: Pinned by Security Drones! ==="));

		if (bAutoplayQuit)
		{
			FGenericPlatformMisc::RequestExit(false);
		}
	}
}

void APriceOnYourHeadGameMode::TriggerLossGavelSold()
{
	if (CurrentState == EHeistState::InHeist)
	{
		CurrentState = EHeistState::LostGavelSold;
		UE_LOG(LogTemp, Warning, TEXT("=== HEIST FAILED: Final Gavel Struck! Lot One has been sold! ==="));

		if (bAutoplayQuit)
		{
			FGenericPlatformMisc::RequestExit(false);
		}
	}
}

void APriceOnYourHeadGameMode::AddHaul(int64 Value)
{
	CurrentHaulScore += Value;
}

void APriceOnYourHeadGameMode::TriggerBiddingWar(FVector Location, float Duration)
{
	BiddingWarTimer = Duration;
	UE_LOG(LogTemp, Log, TEXT("Bidding War triggered at %s for %.1fs! Drones blinded."), *Location.ToString(), Duration);
}

void APriceOnYourHeadGameMode::LogQAViolation(const FString& Reason)
{
	QAViolationCount++;
	UE_LOG(LogTemp, Error, TEXT("QA-VIOLATION [Frame %lld]: %s"), GFrameCounter, *Reason);
}

void APriceOnYourHeadGameMode::CheckQARules(float DeltaSeconds)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;

	// Check if player clipped into solid geometry
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerPawn);
	bool bInsideSolid = GetWorld()->SweepSingleByChannel(
		Hit,
		PlayerPawn->GetActorLocation(),
		PlayerPawn->GetActorLocation() + FVector(0, 0, 1),
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeSphere(25.0f),
		Params
	);
	if (bInsideSolid && Hit.bBlockingHit && Hit.Distance == 0.0f)
	{
		LogQAViolation(TEXT("Player character embedded in solid collision"));
	}
}

void APriceOnYourHeadGameMode::UpdateAutoplay(float DeltaSeconds)
{
	// Automated navigation and mechanics testing handled by player pawn in autoplay mode
}
