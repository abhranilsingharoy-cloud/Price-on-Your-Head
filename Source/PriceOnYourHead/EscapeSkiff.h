#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeSkiff.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class PRICEONYOURHEAD_API AEscapeSkiff : public AActor
{
	GENERATED_BODY()

public:
	AEscapeSkiff();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skiff")
	UBoxComponent* ExtractionTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skiff")
	UStaticMeshComponent* SkiffMesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);
};
