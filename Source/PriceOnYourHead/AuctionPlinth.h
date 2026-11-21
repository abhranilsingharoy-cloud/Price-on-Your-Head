#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuctionPlinth.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class PRICEONYOURHEAD_API AAuctionPlinth : public AActor
{
	GENERATED_BODY()

public:
	AAuctionPlinth();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plinth")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plinth")
	UStaticMeshComponent* PlinthMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plinth")
	UStaticMeshComponent* LotItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plinth")
	UTextRenderComponent* HoloPriceTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lot")
	bool bIsLotOne = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lot")
	int64 LotValue = 250000;

	bool bItemTaken = false;

	void TakeItem();
};
