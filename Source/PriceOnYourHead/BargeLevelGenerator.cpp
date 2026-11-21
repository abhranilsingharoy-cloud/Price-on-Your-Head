#include "BargeLevelGenerator.h"
#include "AuctionPlinth.h"
#include "CameraDrone.h"
#include "EscapeSkiff.h"
#include "LaserTripwire.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"

ABargeLevelGenerator::ABargeLevelGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
}

void ABargeLevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	BuildBargeEnvironment();
}

void ABargeLevelGenerator::BuildBargeEnvironment()
{
	BuildDeckHull();
	BuildRailingAndWalkways();
	BuildAuctionStageAndPlinths();
	BuildBidderBleachers();
	BuildCyberpunkLighting();
	BuildExtractionSkiffDock();
	BuildExtractionSkiffDock();
	SpawnPatrolDrones();
	UE_LOG(LogTemp, Log, TEXT("ABargeLevelGenerator: Cyberpunk Auction Barge generated successfully."));
}

void ABargeLevelGenerator::BuildDeckHull()
{
	// Main Deck Floor: 24m x 16m barge deck
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!CubeMesh) return;

	UStaticMeshComponent* DeckFloor = NewObject<UStaticMeshComponent>(this);
	DeckFloor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeckFloor->SetStaticMesh(CubeMesh);
	DeckFloor->SetRelativeLocation(FVector(1000.0f, 0.0f, -25.0f));
	DeckFloor->SetRelativeScale3D(FVector(24.0f, 16.0f, 0.5f));
	DeckFloor->SetCollisionProfileName(TEXT("BlockAll"));
	DeckFloor->RegisterComponent();

	// Surrounding Flooded Water Plane
	UStaticMeshComponent* WaterPlane = NewObject<UStaticMeshComponent>(this);
	WaterPlane->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WaterPlane->SetStaticMesh(CubeMesh);
	WaterPlane->SetRelativeLocation(FVector(1000.0f, 0.0f, -50.0f));
	WaterPlane->SetRelativeScale3D(FVector(60.0f, 40.0f, 0.1f));
	WaterPlane->SetCollisionProfileName(TEXT("OverlapAll"));
	WaterPlane->RegisterComponent();
}

void ABargeLevelGenerator::BuildRailingAndWalkways()
{
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!CubeMesh) return;

	// Perimeter safety barriers (preventing falling into water except at skiff dock)
	FVector WallConfigs[] = {
		FVector(1000.0f, 800.0f, 50.0f),  FVector(24.0f, 0.2f, 1.0f), // North Rail
		FVector(1000.0f, -800.0f, 50.0f), FVector(24.0f, 0.2f, 1.0f), // South Rail
		FVector(-200.0f, 0.0f, 50.0f),    FVector(0.2f, 16.0f, 1.0f), // West Stern
		FVector(2200.0f, -400.0f, 50.0f), FVector(0.2f, 8.0f, 1.0f),  // East Bow Rail (leaves opening for skiff)
	};

	for (int32 i = 0; i < 4; i++)
	{
		UStaticMeshComponent* Rail = NewObject<UStaticMeshComponent>(this);
		Rail->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Rail->SetStaticMesh(CubeMesh);
		Rail->SetRelativeLocation(WallConfigs[i * 2]);
		Rail->SetRelativeScale3D(WallConfigs[i * 2 + 1]);
		Rail->SetCollisionProfileName(TEXT("BlockAll"));
		Rail->RegisterComponent();
	}
}

void ABargeLevelGenerator::BuildAuctionStageAndPlinths()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// 1. Center Stage Plinth: LOT ONE (Billionaire's Frozen Head)
	FVector CenterStageLoc(1200.0f, 0.0f, 0.0f);
	AAuctionPlinth* LotOnePlinth = GetWorld()->SpawnActor<AAuctionPlinth>(AAuctionPlinth::StaticClass(), CenterStageLoc, FRotator::ZeroRotator, SpawnParams);
	if (LotOnePlinth)
	{
		LotOnePlinth->bIsLotOne = true;
	}

	// 2. Flanking Plinths: Bonus Lots
	FVector BonusLocs[] = {
		FVector(500.0f, 350.0f, 0.0f),   // Plinth A: Golden Biometric Drive ($250k)
		FVector(500.0f, -350.0f, 0.0f),  // Plinth B: Cryo-preserved Neural Chip ($250k)
		FVector(900.0f, -450.0f, 0.0f),  // Plinth C: Syndicate Bearer Bonds ($350k)
		FVector(1500.0f, 400.0f, 0.0f)   // Plinth D: Prototype Quantum Core ($400k)
	};

	for (int32 i = 0; i < 4; i++)
	{
		AAuctionPlinth* BonusPlinth = GetWorld()->SpawnActor<AAuctionPlinth>(AAuctionPlinth::StaticClass(), BonusLocs[i], FRotator::ZeroRotator, SpawnParams);
		if (BonusPlinth)
		{
			BonusPlinth->bIsLotOne = false;
			BonusPlinth->LotValue = (i == 3) ? 400000 : (i == 2) ? 350000 : 250000;
		}
	}

	// 3. Laser Security Tripwires across access corridors
	GetWorld()->SpawnActor<ALaserTripwire>(ALaserTripwire::StaticClass(), FVector(750.0f, 150.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), SpawnParams);
	GetWorld()->SpawnActor<ALaserTripwire>(ALaserTripwire::StaticClass(), FVector(750.0f, -150.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), SpawnParams);
}

void ABargeLevelGenerator::BuildBidderBleachers()
{
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!CubeMesh) return;

	// Tiered seating along northern gallery for VIP masked bidders
	for (int32 Tier = 0; Tier < 3; Tier++)
	{
		UStaticMeshComponent* TierMesh = NewObject<UStaticMeshComponent>(this);
		TierMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		TierMesh->SetStaticMesh(CubeMesh);
		TierMesh->SetRelativeLocation(FVector(1200.0f, 650.0f + Tier * 60.0f, 30.0f + Tier * 50.0f));
		TierMesh->SetRelativeScale3D(FVector(10.0f, 1.2f, 0.8f));
		TierMesh->SetCollisionProfileName(TEXT("BlockAll"));
		TierMesh->RegisterComponent();
	}
}

void ABargeLevelGenerator::BuildCyberpunkLighting()
{
	// Keeping scene bright, vibrant, and well-lit to prevent near-black violations
	struct FLightSetup {
		FVector Loc;
		FLinearColor Color;
		float Intensity;
		float Radius;
	};

	FLightSetup Lights[] = {
		{ FVector(1200.0f, 0.0f, 450.0f),    FLinearColor(0.0f, 0.8f, 1.0f), 12000.0f, 1600.0f }, // Cyan Stage Key Light
		{ FVector(500.0f, 350.0f, 300.0f),   FLinearColor(0.1f, 0.9f, 1.0f), 6000.0f, 900.0f },   // Bonus A Fill
		{ FVector(500.0f, -350.0f, 300.0f),  FLinearColor(1.0f, 0.2f, 0.4f), 6000.0f, 900.0f },   // Bonus B Accent
		{ FVector(1500.0f, 400.0f, 300.0f),  FLinearColor(0.0f, 1.0f, 0.7f), 6000.0f, 900.0f },   // North Plinth Light
		{ FVector(2000.0f, 300.0f, 350.0f),  FLinearColor(1.0f, 0.8f, 0.2f), 8000.0f, 1200.0f },  // Skiff Dock Amber Beacon
		{ FVector(0.0f, 0.0f, 350.0f),       FLinearColor(0.2f, 0.6f, 1.0f), 7000.0f, 1200.0f }   // Infiltration Stern Light
	};

	for (int32 i = 0; i < 6; i++)
	{
		UPointLightComponent* PointLight = NewObject<UPointLightComponent>(this);
		PointLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		PointLight->SetRelativeLocation(Lights[i].Loc);
		PointLight->SetLightColor(Lights[i].Color);
		PointLight->Intensity = Lights[i].Intensity;
		PointLight->AttenuationRadius = Lights[i].Radius;
		PointLight->CastShadows = true;
		PointLight->RegisterComponent();
	}
}

void ABargeLevelGenerator::BuildExtractionSkiffDock()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// Extraction zone at the bow of the barge
	FVector SkiffLoc(2100.0f, 300.0f, -10.0f);
	FRotator SkiffRot(0.0f, -45.0f, 0.0f);

	GetWorld()->SpawnActor<AEscapeSkiff>(AEscapeSkiff::StaticClass(), SkiffLoc, SkiffRot, SpawnParams);
}

void ABargeLevelGenerator::SpawnPatrolDrones()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// Drone 1: Sweeps the Center Stage
	FVector Drone1Pos(1100.0f, -250.0f, 180.0f);
	ACameraDrone* Drone1 = GetWorld()->SpawnActor<ACameraDrone>(ACameraDrone::StaticClass(), Drone1Pos, FRotator::ZeroRotator, SpawnParams);
	if (Drone1)
	{
		Drone1->PatrolPoints.Empty();
		Drone1->PatrolPoints.Add(FVector(1100.0f, -350.0f, 180.0f));
		Drone1->PatrolPoints.Add(FVector(1100.0f, 350.0f, 180.0f));
	}

	// Drone 2: Sweeps the Bow and Escape Skiff Corridor
	FVector Drone2Pos(1700.0f, 100.0f, 190.0f);
	ACameraDrone* Drone2 = GetWorld()->SpawnActor<ACameraDrone>(ACameraDrone::StaticClass(), Drone2Pos, FRotator::ZeroRotator, SpawnParams);
	if (Drone2)
	{
		Drone2->PatrolPoints.Empty();
		Drone2->PatrolPoints.Add(FVector(1500.0f, 200.0f, 190.0f));
		Drone2->PatrolPoints.Add(FVector(1900.0f, -150.0f, 190.0f));
	}
}
