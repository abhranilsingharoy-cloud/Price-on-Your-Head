#include "EscapeSkiff.h"
#include "PorterCharacter.h"
#include "PriceOnYourHeadGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AEscapeSkiff::AEscapeSkiff()
{
	PrimaryActorTick.bCanEverTick = false;

	ExtractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ExtractionTrigger"));
	ExtractionTrigger->InitBoxExtent(FVector(150.0f, 150.0f, 80.0f));
	ExtractionTrigger->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = ExtractionTrigger;

	SkiffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkiffMesh"));
	SkiffMesh->SetupAttachment(RootComponent);

	ExtractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEscapeSkiff::OnOverlapBegin);
}

void AEscapeSkiff::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                   bool bFromSweep, const FHitResult& SweepResult)
{
	APorterCharacter* Porter = Cast<APorterCharacter>(OtherActor);
	if (Porter && Porter->bCarryingLotOne)
	{
		APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->TriggerWin();
		}
	}
}
