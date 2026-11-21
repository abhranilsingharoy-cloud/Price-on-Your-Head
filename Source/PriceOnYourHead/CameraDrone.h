#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraDrone.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
class PRICEONYOURHEAD_API ACameraDrone : public AActor
{
	GENERATED_BODY()

public:
	ACameraDrone();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	UStaticMeshComponent* DroneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	USpotLightComponent* SearchLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	TArray<FVector> PatrolPoints;

	float BaseVisionRange = 350.0f;
	float PinnedTimer = 0.0f;

private:
	int32 CurrentPatrolIndex = 0;
	bool bAlerted = false;

	void UpdatePatrol(float DeltaSeconds);
	void CheckPlayerVision(float DeltaSeconds);
};
