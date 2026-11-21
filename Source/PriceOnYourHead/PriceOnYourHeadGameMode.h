#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PriceOnYourHeadGameMode.generated.h"

UENUM(BlueprintType)
enum class EHeistState : uint8
{
	Planning,
	InHeist,
	Won,
	LostPinned,
	LostGavelSold
};

UCLASS()
class PRICEONYOURHEAD_API APriceOnYourHeadGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APriceOnYourHeadGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Heist Flow
	void StartHeist();
	void TriggerWin();
	void TriggerLossPinned();
	void TriggerLossGavelSold();

	// State accessors
	EHeistState GetHeistState() const { return CurrentState; }
	float GetRemainingGavelTime() const { return RemainingGavelSeconds; }
	int64 GetTotalHaulScore() const { return CurrentHaulScore; }
	void AddHaul(int64 Value);

	// Bidding war event
	void TriggerBiddingWar(FVector Location, float Duration = 4.0f);
	bool IsBiddingWarActive() const { return BiddingWarTimer > 0.0f; }

	// Autoplay flags
	bool bIsAutoplay = false;
	bool bAutoplayQuit = false;

	// QA Violation Tracking
	void LogQAViolation(const FString& Reason);
	int32 QAViolationCount = 0;

private:
	EHeistState CurrentState = EHeistState::Planning;
	float RemainingGavelSeconds = 90.0f;
	int64 CurrentHaulScore = 0;
	float BiddingWarTimer = 0.0f;
	float ElapsedHeistSeconds = 0.0f;

	void UpdateAutoplay(float DeltaSeconds);
	void CheckQARules(float DeltaSeconds);
};
