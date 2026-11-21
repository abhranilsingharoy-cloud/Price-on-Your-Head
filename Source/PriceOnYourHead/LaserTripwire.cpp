#include "LaserTripwire.h"
#include "PorterCharacter.h"
#include "PriceOnYourHeadGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ALaserTripwire::ALaserTripwire()
{
	PrimaryActorTick.bCanEverTick = true;

	LaserTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserTrigger"));
	LaserTrigger->InitBoxExtent(FVector(200.0f, 15.0f, 10.0f));
	LaserTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	LaserTrigger->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = LaserTrigger;

	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));

	// Emitter Posts on both sides
	EmitterPostA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmitterPostA"));
	EmitterPostA->SetupAttachment(RootComponent);
	EmitterPostA->SetStaticMesh(CubeMesh);
	EmitterPostA->SetRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
	EmitterPostA->SetRelativeScale3D(FVector(0.2f, 0.2f, 1.4f));

	EmitterPostB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmitterPostB"));
	EmitterPostB->SetupAttachment(RootComponent);
	EmitterPostB->SetStaticMesh(CubeMesh);
	EmitterPostB->SetRelativeLocation(FVector(-200.0f, 0.0f, 0.0f));
	EmitterPostB->SetRelativeScale3D(FVector(0.2f, 0.2f, 1.4f));

	// Animated glowing Laser Beam
	LaserBeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserBeamMesh"));
	LaserBeamMesh->SetupAttachment(RootComponent);
	LaserBeamMesh->SetStaticMesh(CubeMesh);
	LaserBeamMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LaserBeamMesh->SetRelativeScale3D(FVector(4.0f, 0.04f, 0.04f));
	LaserBeamMesh->SetCollisionProfileName(TEXT("NoCollision"));

	LaserTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALaserTripwire::OnOverlapBegin);
}

void ALaserTripwire::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserTripwire::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PulseTimer += DeltaSeconds * 4.0f;
	float ScaleY = 0.04f + 0.015f * FMath::Sin(PulseTimer);
	LaserBeamMesh->SetRelativeScale3D(FVector(4.0f, ScaleY, ScaleY));
}

void ALaserTripwire::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                    bool bFromSweep, const FHitResult& SweepResult)
{
	APorterCharacter* Porter = Cast<APorterCharacter>(OtherActor);
	if (Porter)
	{
		// If player is standing (not crouched), tripwire triggers security alert!
		if (!Porter->bIsCrouched)
		{
			UE_LOG(LogTemp, Warning, TEXT("SECURITY TRIPWIRE BREACHED! Player crossed standing!"));
			APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
			if (GM)
			{
				// Increase visible detection profile and sound alarm
				UGameplayStatics::PlaySoundAtLocation(this, nullptr, GetActorLocation());
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Player successfully crouched under laser tripwire."));
		}
	}
}
