#include "AuctionPlinth.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

AAuctionPlinth::AAuctionPlinth()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->InitBoxExtent(FVector(50.0f, 50.0f, 60.0f));
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = CollisionBox;

	PlinthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlinthMesh"));
	PlinthMesh->SetupAttachment(RootComponent);

	LotItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LotItemMesh"));
	LotItemMesh->SetupAttachment(RootComponent);
	LotItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));

	HoloPriceTag = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HoloPriceTag"));
	HoloPriceTag->SetupAttachment(LotItemMesh);
	HoloPriceTag->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	HoloPriceTag->SetHorizontalAlignment(EHTA_Center);
	HoloPriceTag->SetTextRenderColor(FColor(0, 230, 255, 230));
	HoloPriceTag->SetWorldSize(28.0f);
}

void AAuctionPlinth::BeginPlay()
{
	Super::BeginPlay();

	if (bIsLotOne)
	{
		LotValue = 1000000;
		HoloPriceTag->SetText(FText::FromString(TEXT("LOT 01: $1,000,000")));
		HoloPriceTag->SetTextRenderColor(FColor(255, 60, 60, 255));
	}
	else
	{
		HoloPriceTag->SetText(FText::FromString(FString::Printf(TEXT("LOT: $%lld"), LotValue)));
	}
}

void AAuctionPlinth::TakeItem()
{
	if (!bItemTaken)
	{
		bItemTaken = true;
		LotItemMesh->SetVisibility(false);
		HoloPriceTag->SetText(FText::FromString(TEXT("SOLD / EMPTY")));
		HoloPriceTag->SetTextRenderColor(FColor(120, 120, 120, 150));
	}
}
