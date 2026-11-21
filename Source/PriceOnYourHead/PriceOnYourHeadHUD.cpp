#include "PriceOnYourHeadHUD.h"
#include "PriceOnYourHeadGameMode.h"
#include "PorterCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Kismet/GameplayStatics.h"

APriceOnYourHeadHUD::APriceOnYourHeadHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> FontObj(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	if (FontObj.Succeeded())
	{
		MainFont = FontObj.Object;
	}
	else
	{
		MainFont = GEngine ? GEngine->GetMediumFont() : nullptr;
	}
}

void APriceOnYourHeadHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas) return;

	float ScreenW = Canvas->SizeX;
	float ScreenH = Canvas->SizeY;

	APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM) return;

	switch (GM->GetHeistState())
	{
	case EHeistState::Planning:
		DrawPlanningScreen(ScreenW, ScreenH);
		break;
	case EHeistState::InHeist:
		DrawGameplayHUD(ScreenW, ScreenH);
		break;
	case EHeistState::Won:
		DrawEndScreen(ScreenW, ScreenH, true);
		break;
	case EHeistState::LostPinned:
	case EHeistState::LostGavelSold:
		DrawEndScreen(ScreenW, ScreenH, false);
		break;
	}
}

void APriceOnYourHeadHUD::DrawThemedCard(float X, float Y, float Width, float Height, FLinearColor BgColor, FLinearColor BorderColor)
{
	// Solid themed background card with contrast border
	DrawRect(BgColor, X, Y, Width, Height);
	// Outer border (2px)
	DrawRect(BorderColor, X, Y, Width, 2.0f);
	DrawRect(BorderColor, X, Y + Height - 2.0f, Width, 2.0f);
	DrawRect(BorderColor, X, Y, 2.0f, Height);
	DrawRect(BorderColor, X + Width - 2.0f, Y, 2.0f, Height);
}

void APriceOnYourHeadHUD::DrawPlanningScreen(float ScreenW, float ScreenH)
{
	// Dark semi-transparent backdrop
	DrawRect(FLinearColor(0.01f, 0.02f, 0.05f, 0.85f), 0, 0, ScreenW, ScreenH);

	// Central Blueprint Card
	float CardW = 900.0f;
	float CardH = 650.0f;
	float CardX = (ScreenW - CardW) * 0.5f;
	float CardY = (ScreenH - CardH) * 0.5f;

	DrawThemedCard(CardX, CardY, CardW, CardH, FLinearColor(0.02f, 0.05f, 0.1f, 0.95f), FLinearColor(0.0f, 0.8f, 1.0f, 1.0f));

	// Title
	DrawText(TEXT("DECK PLAN // BARGE AUCTION: LOT ONE"), FLinearColor(0.0f, 0.9f, 1.0f), CardX + 40.0f, CardY + 30.0f, MainFont, 1.3f);
	DrawText(TEXT("Select tools & spend insider tips to reveal patrol routes"), FLinearColor(0.7f, 0.8f, 0.9f), CardX + 40.0f, CardY + 70.0f, MainFont, 1.0f);

	// Tools loadout card
	DrawThemedCard(CardX + 40.0f, CardY + 120.0f, 400.0f, 320.0f, FLinearColor(0.04f, 0.08f, 0.15f), FLinearColor(0.2f, 0.6f, 0.9f));
	DrawText(TEXT("EQUIPPED TOOLS:"), FLinearColor(0.0f, 0.9f, 1.0f), CardX + 60.0f, CardY + 140.0f, MainFont, 1.1f);
	DrawText(TEXT("[1] FAKE LOT TAG (8s Bounty Camouflage)"), FLinearColor::White, CardX + 60.0f, CardY + 190.0f, MainFont, 1.0f);
	DrawText(TEXT("[2] DECOY PADDLE (Starts Bidding War, Blinds Drones)"), FLinearColor::White, CardX + 60.0f, CardY + 240.0f, MainFont, 1.0f);

	// Target objective
	DrawThemedCard(CardX + 470.0f, CardY + 120.0f, 390.0f, 320.0f, FLinearColor(0.04f, 0.08f, 0.15f), FLinearColor(0.9f, 0.2f, 0.2f));
	DrawText(TEXT("PRIMARY TARGET:"), FLinearColor(1.0f, 0.3f, 0.3f), CardX + 490.0f, CardY + 140.0f, MainFont, 1.1f);
	DrawText(TEXT("LOT 01: Billionaire's Frozen Head"), FLinearColor::White, CardX + 490.0f, CardY + 190.0f, MainFont, 1.0f);
	DrawText(TEXT("Value: $1,000,000 | Location: Center Plinth"), FLinearColor(0.8f, 0.8f, 0.8f), CardX + 490.0f, CardY + 230.0f, MainFont, 0.9f);
	DrawText(TEXT("WARNING: Drones detect carried bounty from afar!"), FLinearColor(1.0f, 0.7f, 0.2f), CardX + 490.0f, CardY + 280.0f, MainFont, 0.9f);

	// Start prompt
	DrawThemedCard(CardX + 150.0f, CardY + 520.0f, 600.0f, 70.0f, FLinearColor(0.0f, 0.6f, 0.8f), FLinearColor::White);
	DrawText(TEXT("PRESS [SPACE / ENTER] TO INFILTRATE BARGE"), FLinearColor::White, CardX + 200.0f, CardY + 542.0f, MainFont, 1.2f);
}

void APriceOnYourHeadHUD::DrawGameplayHUD(float ScreenW, float ScreenH)
{
	const float Margin = 64.0f;
	APriceOnYourHeadGameMode* GM = Cast<APriceOnYourHeadGameMode>(UGameplayStatics::GetGameMode(this));
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	APorterCharacter* Porter = PC ? Cast<APorterCharacter>(PC->GetPawn()) : nullptr;

	// 1. Top-Left: Gavel Countdown Clock
	float TimeLeft = GM ? GM->GetRemainingGavelTime() : 90.0f;
	int32 Mins = FMath::FloorToInt(TimeLeft / 60.0f);
	int32 Secs = FMath::FloorToInt(TimeLeft) % 60;
	FString TimeStr = FString::Printf(TEXT("%02d:%02d"), Mins, Secs);

	DrawThemedCard(Margin, Margin, 260.0f, 90.0f, FLinearColor(0.03f, 0.05f, 0.1f, 0.85f), FLinearColor(1.0f, 0.3f, 0.3f));
	DrawText(TEXT("GAVEL COUNTDOWN"), FLinearColor(0.9f, 0.3f, 0.3f), Margin + 15.0f, Margin + 12.0f, MainFont, 0.85f);
	DrawText(TimeStr, FLinearColor::White, Margin + 15.0f, Margin + 35.0f, MainFont, 1.6f);

	// 2. Top-Right: Total Haul / Score Readout
	int64 Haul = GM ? GM->GetTotalHaulScore() : 0;
	FString HaulStr = FString::Printf(TEXT("$%lld"), Haul);

	float RightCardW = 300.0f;
	float RightCardX = ScreenW - Margin - RightCardW;
	DrawThemedCard(RightCardX, Margin, RightCardW, 90.0f, FLinearColor(0.03f, 0.05f, 0.1f, 0.85f), FLinearColor(0.0f, 0.8f, 1.0f));
	DrawText(TEXT("TOTAL HAUL VALUE"), FLinearColor(0.0f, 0.8f, 1.0f), RightCardX + 15.0f, Margin + 12.0f, MainFont, 0.85f);
	DrawText(HaulStr, FLinearColor::White, RightCardX + 15.0f, Margin + 35.0f, MainFont, 1.6f);

	// 3. Top-Center: Objective Line (2-4 words)
	FString ObjText = (Porter && Porter->bCarryingLotOne) ? TEXT("ESCAPE VIA SKIFF") : TEXT("STEAL LOT ONE");
	DrawThemedCard((ScreenW - 320.0f) * 0.5f, Margin, 320.0f, 45.0f, FLinearColor(0.02f, 0.04f, 0.08f, 0.85f), FLinearColor(0.9f, 0.8f, 0.2f));
	DrawText(ObjText, FLinearColor(1.0f, 0.9f, 0.3f), (ScreenW - 240.0f) * 0.5f, Margin + 12.0f, MainFont, 1.1f);

	// 4. Bottom-Right: Active Tool Card
	float BottomCardH = 80.0f;
	float BottomCardY = ScreenH - Margin - BottomCardH;
	FString ToolName = (Porter && Porter->ActiveToolIndex == 1) ? TEXT("DECOY PADDLE [Q]") : TEXT("FAKE LOT TAG [Q]");

	DrawThemedCard(RightCardX, BottomCardY, RightCardW, BottomCardH, FLinearColor(0.03f, 0.05f, 0.1f, 0.85f), FLinearColor(0.3f, 0.7f, 1.0f));
	DrawText(TEXT("EQUIPPED TOOL"), FLinearColor(0.3f, 0.7f, 1.0f), RightCardX + 15.0f, BottomCardY + 12.0f, MainFont, 0.85f);
	DrawText(ToolName, FLinearColor::White, RightCardX + 15.0f, BottomCardY + 36.0f, MainFont, 1.1f);

	// Center Bidding War Alert
	if (GM && GM->IsBiddingWarActive())
	{
		DrawThemedCard((ScreenW - 550.0f) * 0.5f, 180.0f, 550.0f, 55.0f, FLinearColor(0.8f, 0.1f, 0.1f, 0.9f), FLinearColor::White);
		DrawText(TEXT("BIDDING WAR ACTIVE // DRONES BLINDED"), FLinearColor::White, (ScreenW - 460.0f) * 0.5f, 195.0f, MainFont, 1.2f);
	}

	// 5. Bottom-Left: Stealth Tactical Radar / Minimap
	DrawStealthMinimap(Margin, ScreenH - Margin - 150.0f, 150.0f);

	// 6. Bottom Corner: Key Input Visualizer (per style guide)
	DrawKeyInputVisualizer(RightCardX, ScreenH - Margin - 150.0f);
}

void APriceOnYourHeadHUD::DrawStealthMinimap(float X, float Y, float Size)
{
	// Radar background circle card
	DrawThemedCard(X, Y, Size, Size, FLinearColor(0.01f, 0.03f, 0.08f, 0.92f), FLinearColor(0.0f, 0.8f, 1.0f));

	float CenterX = X + Size * 0.5f;
	float CenterY = Y + Size * 0.5f;

	// Crosshairs
	DrawRect(FLinearColor(0.0f, 0.6f, 0.9f, 0.4f), CenterX - Size * 0.45f, CenterY, Size * 0.9f, 1.0f);
	DrawRect(FLinearColor(0.0f, 0.6f, 0.9f, 0.4f), CenterX, CenterY - Size * 0.45f, 1.0f, Size * 0.9f);

	// Player dot in center
	DrawRect(FLinearColor(0.0f, 1.0f, 0.4f), CenterX - 4.0f, CenterY - 4.0f, 8.0f, 8.0f);

	// Radar blips for plinths and extraction skiff
	DrawRect(FLinearColor(1.0f, 0.8f, 0.0f), CenterX + 35.0f, CenterY - 20.0f, 6.0f, 6.0f); // Bonus Lot A
	DrawRect(FLinearColor(1.0f, 0.8f, 0.0f), CenterX + 35.0f, CenterY + 20.0f, 6.0f, 6.0f); // Bonus Lot B
	DrawRect(FLinearColor(1.0f, 0.2f, 0.2f), CenterX + 50.0f, CenterY, 7.0f, 7.0f);          // Lot 01
	DrawRect(FLinearColor(0.0f, 0.8f, 1.0f), CenterX + 65.0f, CenterY + 25.0f, 8.0f, 8.0f);  // Skiff

	DrawText(TEXT("RADAR // 25M"), FLinearColor(0.0f, 0.8f, 1.0f), X + 10.0f, Y + 8.0f, MainFont, 0.75f);
}

void APriceOnYourHeadHUD::DrawKeyInputVisualizer(float X, float Y)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	// Check pressed keys
	bool bW = PC->IsInputKeyDown(EKeys::W);
	bool bA = PC->IsInputKeyDown(EKeys::A);
	bool bS = PC->IsInputKeyDown(EKeys::S);
	bool bD = PC->IsInputKeyDown(EKeys::D);
	bool bCtrl = PC->IsInputKeyDown(EKeys::LeftControl);
	bool bE = PC->IsInputKeyDown(EKeys::E);
	bool bQ = PC->IsInputKeyDown(EKeys::Q);

	auto GetKeyColor = [](bool bPressed) {
		return bPressed ? FLinearColor(0.0f, 1.0f, 0.4f, 0.95f) : FLinearColor(0.1f, 0.15f, 0.25f, 0.7f);
	};

	// Draw key boxes
	DrawRect(GetKeyColor(bW), X + 40.0f, Y, 30.0f, 30.0f);
	DrawText(TEXT("W"), FLinearColor::White, X + 50.0f, Y + 6.0f, MainFont, 0.8f);

	DrawRect(GetKeyColor(bA), X, Y + 35.0f, 30.0f, 30.0f);
	DrawText(TEXT("A"), FLinearColor::White, X + 10.0f, Y + 41.0f, MainFont, 0.8f);

	DrawRect(GetKeyColor(bS), X + 40.0f, Y + 35.0f, 30.0f, 30.0f);
	DrawText(TEXT("S"), FLinearColor::White, X + 50.0f, Y + 41.0f, MainFont, 0.8f);

	DrawRect(GetKeyColor(bD), X + 80.0f, Y + 35.0f, 30.0f, 30.0f);
	DrawText(TEXT("D"), FLinearColor::White, X + 90.0f, Y + 41.0f, MainFont, 0.8f);

	DrawRect(GetKeyColor(bCtrl), X + 120.0f, Y + 35.0f, 45.0f, 30.0f);
	DrawText(TEXT("CTRL"), FLinearColor::White, X + 126.0f, Y + 43.0f, MainFont, 0.7f);

	DrawRect(GetKeyColor(bE), X + 175.0f, Y + 35.0f, 30.0f, 30.0f);
	DrawText(TEXT("E"), FLinearColor::White, X + 185.0f, Y + 41.0f, MainFont, 0.8f);

	DrawRect(GetKeyColor(bQ), X + 215.0f, Y + 35.0f, 30.0f, 30.0f);
	DrawText(TEXT("Q"), FLinearColor::White, X + 225.0f, Y + 41.0f, MainFont, 0.8f);
}

void APriceOnYourHeadHUD::DrawEndScreen(float ScreenW, float ScreenH, bool bWon)
{
	DrawRect(FLinearColor(0.01f, 0.02f, 0.04f, 0.90f), 0, 0, ScreenW, ScreenH);

	float CardW = 750.0f;
	float CardH = 450.0f;
	float CardX = (ScreenW - CardW) * 0.5f;
	float CardY = (ScreenH - CardH) * 0.5f;

	FLinearColor BorderCol = bWon ? FLinearColor(0.0f, 0.9f, 0.3f) : FLinearColor(1.0f, 0.2f, 0.2f);
	DrawThemedCard(CardX, CardY, CardW, CardH, FLinearColor(0.02f, 0.05f, 0.1f, 0.98f), BorderCol);

	if (bWon)
	{
		DrawText(TEXT("HEIST SUCCESSFUL // ESCAPED"), FLinearColor(0.0f, 1.0f, 0.4f), CardX + 50.0f, CardY + 40.0f, MainFont, 1.5f);
		DrawText(TEXT("MEDAL RANK: GOLD"), FLinearColor(1.0f, 0.85f, 0.2f), CardX + 50.0f, CardY + 100.0f, MainFont, 1.8f);
		DrawText(TEXT("Lot One (Frozen Head) Secured!"), FLinearColor::White, CardX + 50.0f, CardY + 170.0f, MainFont, 1.2f);
	}
	else
	{
		DrawText(TEXT("HEIST FAILED"), FLinearColor(1.0f, 0.2f, 0.2f), CardX + 50.0f, CardY + 40.0f, MainFont, 1.6f);
		DrawText(TEXT("You were pinned by security drones or the auction timed out."), FLinearColor(0.8f, 0.8f, 0.8f), CardX + 50.0f, CardY + 110.0f, MainFont, 1.1f);
	}

	// Instructions
	DrawText(TEXT("Press [R] to Restart | [Q] to Quit"), FLinearColor(0.6f, 0.8f, 1.0f), CardX + 50.0f, CardY + 360.0f, MainFont, 1.1f);
}
