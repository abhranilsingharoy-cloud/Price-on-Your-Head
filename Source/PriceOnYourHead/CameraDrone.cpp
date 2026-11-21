#include "CameraDrone.h"
#include "PorterCharacter.h"
#include "PriceOnYourHeadGameMode.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

ACameraDrone::ACameraDrone()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(45.0f);
	RootComponent = CollisionSphere;

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);

	SearchLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SearchLight"));
	SearchLight->SetupAttachment(RootComponent);
	SearchLight->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SearchLight->InnerConeAngle = 25.0f;
	SearchLight->OuterConeAngle = 45.0f;
	SearchLight->SetLightColor(FLinearColor(0.0f, 0.8f, 1.0f));
	SearchLight->Intensity = 5000.0f;
}

void ACameraDrone::BeginPlay()
{
	Super::BeginPlay();

	if (PatrolPoints.Num() == 0)
	{
		FVector Base = GetActorLocation();
		PatrolPoints.Add(Base + FVector(300.0f, 0.0f, 0.0f));
		PatrolPoints.Add(Base + FVector(-300.0f, 0.0f, 0.0f));
	}
}

void ACameraDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->IsBiddingWarActive())
	{
		// Bidding war blinds drones
		SearchLight->SetLightColor(FLinearColor(1.0f, 0.1f, 0.1f));
		PinnedTimer = 0.0f;
		return;
	}

	SearchLight->SetLightColor(bAlerted ? FLinearColor(1.0f, 0.2f, 0.0f) : FLinearColor(0.0f, 0.8f, 1.0f));

	UpdatePatrol(DeltaSeconds);
	CheckPlayerVision(DeltaSeconds);
}

void ACameraDrone::UpdatePatrol(float DeltaSeconds)
{
	if (PatrolPoints.Num() == 0) return;

	FVector Target = PatrolPoints[CurrentPatrolIndex];
	FVector CurrentPos = GetActorLocation();
	FVector Dir = (Target - CurrentPos);
	float Dist = Dir.Size();

	if (Dist < 30.0f)
	{
		CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
	}
	else
	{
		Dir.Normalize();
		SetActorLocation(CurrentPos + Dir * 180.0f * DeltaSeconds);
		FRotator TargetRot = Dir.Rotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaSeconds, 5.0f));
	}
}

void ACameraDrone::CheckPlayerVision(float DeltaSeconds)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;
	APorterCharacter* Porter = Cast<APorterCharacter>(PC->GetPawn());
	if (!Porter) return;

	// Scale detection range based on visible bounty
	int64 VisibleBounty = Porter->GetCurrentBounty();
	float EffectiveVisionRange = BaseVisionRange;
	if (VisibleBounty > 0)
	{
		// Scaled detection from farther away
		EffectiveVisionRange += FMath::Clamp((float)VisibleBounty / 1000.0f, 0.0f, 750.0f);
	}

	// Crouch reduces detection range by 45%
	if (Porter->bIsCrouched)
	{
		EffectiveVisionRange *= 0.55f;
	}

	FVector DronePos = GetActorLocation();
	FVector PorterPos = Porter->GetActorLocation();
	float Dist = FVector::Dist(DronePos, PorterPos);

	if (Dist <= EffectiveVisionRange)
	{
		// Check line of sight
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, DronePos, PorterPos, ECC_Visibility, Params);

		if (!bHit || Hit.GetActor() == Porter)
		{
			bAlerted = true;

			// If drone gets right on player for 2.0s, player is pinned
			if (Dist < 160.0f)
			{
				PinnedTimer += DeltaSeconds;
				if (PinnedTimer >= 2.0f)
				{
					APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
					if (GM)
					{
						GM->TriggerLossPinned();
					}
				}
			}
			return;
		}
	}

	bAlerted = false;
	PinnedTimer = 0.0f;
}
