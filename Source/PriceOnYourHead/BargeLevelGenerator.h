#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BargeLevelGenerator.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class USpotLightComponent;

UCLASS()
class PRICEONYOURHEAD_API ABargeLevelGenerator : public AActor
{
	GENERATED_BODY()

public:
	ABargeLevelGenerator();

	virtual void BeginPlay() override;

	// Generates the complete auction barge environment procedurally
	void BuildBargeEnvironment();

private:
	void BuildDeckHull();
	void BuildRailingAndWalkways();
	void BuildAuctionStageAndPlinths();
	void BuildBidderBleachers();
	void BuildCyberpunkLighting();
	void BuildExtractionSkiffDock();
	void SpawnPatrolDrones();
};
