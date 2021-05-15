// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectBlockGrid.h"
#include "MyProjectBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMyProjectBlockGrid::AMyProjectBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	PlayerTurnText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerTurnText0"));
	PlayerTurnText->SetRelativeLocation(FVector(250.0f, 0.f, 0.f));
	PlayerTurnText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	PlayerTurnText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Player 1 Turn: {0}"), BP1Turn));
	PlayerTurnText->SetupAttachment(DummyRoot);

	// Set defaults *dont change Size for tictactoe
	Size = 3;
	BlockSpacing = 300.f;

	//create grid
}


void AMyProjectBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	BP1Turn = true;
	BP2Turn = false;

	int32 x = 3; //todo change how this is 
	int32 y = 3;

	//new loop to spawn each block
	for (int32 BlockX = 0; BlockX < x; BlockX++)
	{

		for (int32 BlockY = 0; BlockY < y; BlockY++) {

			const float XOffset = ((BlockX+1*BlockY+1)/ Size) * BlockSpacing; // Divide by dimension
			const float YOffset = ((BlockX + 1 * BlockY + 1) % Size) * BlockSpacing; // Modulo gives remainder

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			UE_LOG(LogTemp, Warning, TEXT("I just started running %s"), *BlockLocation.ToString());

			// Spawn a block
			AMyProjectBlock* NewBlock = GetWorld()->SpawnActor<AMyProjectBlock>(BlockLocation, FRotator(0, 0, 0));

			NewBlock->xVal = BlockX;
			NewBlock->yVal = BlockY;

			UE_LOG(LogClass, Log, TEXT("My X Value: %d"), NewBlock->xVal);
			UE_LOG(LogClass, Log, TEXT("My Y Value: %d"), NewBlock->yVal);

			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningGrid = this;
			}
		}
	}

	//// Loop to spawn each block
	//for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	//{
	//	const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
	//	const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

	//	// Make position vector, offset from Grid location
	//	const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

	//	UE_LOG(LogTemp, Warning, TEXT("I just started running %s"), *BlockLocation.ToString());
	//

	//	// Spawn a block
	//	AMyProjectBlock* NewBlock = GetWorld()->SpawnActor<AMyProjectBlock>(BlockLocation, FRotator(0,0,0));

	//	NewBlock->xVal = x;
	//	NewBlock->yVal = y;

	//	//TODO: add before updating x vals
	//	//GridBlocks.Add(NewBlock);

	//	//logic to change x, y vals



	//	// Tell the block about its owner
	//	if (NewBlock != nullptr)
	//	{
	//		NewBlock->OwningGrid = this;
	//	}
	//}
}


void AMyProjectBlockGrid::HandleTurn()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
	if (BP1Turn) {
		BP1Turn = false;
		BP2Turn = true;
		PlayerTurnText->SetText(FText::FromString("Player 1's Turn"));
	}
	else {
		BP1Turn = true;
		BP2Turn = false;
		PlayerTurnText->SetText(FText::FromString("Player 2's Turn"));
	}

	CheckGameEnd();
}

void AMyProjectBlockGrid::CheckGameEnd()
{

}

#undef LOCTEXT_NAMESPACE
