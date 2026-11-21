#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PorterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTextRenderComponent;

UENUM(BlueprintType)
enum class EHeistTool : uint8
{
	None,
	FakeLotTag,
	SmokeCartridge,
	DecoyPaddle,
	MagneticHook
};

UCLASS()
class PRICEONYOURHEAD_API APorterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APorterCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input Actions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartCrouch();
	void StopCrouch();
	void InteractGrab();
	void ActivateTool();
	void SelectTool1();
	void SelectTool2();
	void ConfirmStartHeist();

	// Inventory & Bounty
	bool bCarryingLotOne = false;
	int32 BonusLotsCarried = 0;
	int64 GetCurrentBounty() const;

	// Tools
	EHeistTool EquippedTools[2];
	int32 ActiveToolIndex = 0;
	float FakeTagActiveTimer = 0.0f;

	// Camera & Visuals
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bounty")
	UTextRenderComponent* BountyHoloText;

private:
	void UpdateHoloPriceDisplay();
	void AutoplayStealthStep(float DeltaSeconds);

	TArray<FVector> AutoplayWaypoints;
	int32 CurrentWaypointIndex = 0;
};
