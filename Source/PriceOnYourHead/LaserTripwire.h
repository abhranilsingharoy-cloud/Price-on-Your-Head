#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserTripwire.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class PRICEONYOURHEAD_API ALaserTripwire : public AActor
{
	GENERATED_BODY()

public:
	ALaserTripwire();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser")
	UBoxComponent* LaserTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser")
	UStaticMeshComponent* EmitterPostA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser")
	UStaticMeshComponent* EmitterPostB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser")
	UStaticMeshComponent* LaserBeamMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	float BeamLength = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	float BeamHeight = 70.0f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);

private:
	float PulseTimer = 0.0f;
};
