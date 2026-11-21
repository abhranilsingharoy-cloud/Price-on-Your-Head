#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PriceOnYourHeadHUD.generated.h"

UCLASS()
class PRICEONYOURHEAD_API APriceOnYourHeadHUD : public AHUD
{
	GENERATED_BODY()

public:
	APriceOnYourHeadHUD();

	virtual void DrawHUD() override;

private:
	void DrawPlanningScreen(float ScreenW, float ScreenH);
	void DrawGameplayHUD(float ScreenW, float ScreenH);
	void DrawEndScreen(float ScreenW, float ScreenH, bool bWon);
	void DrawStealthMinimap(float X, float Y, float Size);
	void DrawKeyInputVisualizer(float X, float Y);

	void DrawThemedCard(float X, float Y, float Width, float Height, FLinearColor BgColor, FLinearColor BorderColor);
	void CheckUIRectOverlap(const FBox2D& BoxA, const FBox2D& BoxB, const FString& LabelA, const FString& LabelB);

	UPROPERTY()
	UFont* MainFont;

	float CalloutTimer = 0.0f;
	FString CurrentCalloutText;
};
