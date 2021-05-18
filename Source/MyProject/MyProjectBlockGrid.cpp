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
	PlayerTurnText->SetText(FText::FromString("Player 1's Turn"));
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
	int32 BlockIndex = 0;

	//new loop to spawn each block
	for (int32 BlockX = 0; BlockX < x; BlockX++)
	{

		for (int32 BlockY = 0; BlockY < y; BlockY++) {

			const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			UE_LOG(LogTemp, Warning, TEXT("I just started running %s"), *BlockLocation.ToString());

			// Spawn a block
			AMyProjectBlock* NewBlock = GetWorld()->SpawnActor<AMyProjectBlock>(BlockLocation, FRotator(0, 0, 0));

			NewBlock->xVal = BlockX;
			NewBlock->yVal = BlockY;
			NewBlock->BlockIndex = BlockIndex;
			//NewBlock->CharPiece = 'N';

			BlockGrid.Add(NewBlock);

			//UE_LOG(LogTemp, Warning, TEXT("I just started running %s"), BlockGrid.Num());
			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningGrid = this;
			}
			BlockIndex++;
		}
	}
}


void AMyProjectBlockGrid::HandleTurn(int BlockIndex)
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
	if (BP1Turn) {
		BP1Turn = false;
		BP2Turn = true;
		BlockGrid[BlockIndex]->CharPiece = 'x';
		PlayerTurnText->SetText(FText::FromString("O's Turn"));
	}
	else {
		BP1Turn = true;
		BP2Turn = false;
		BlockGrid[BlockIndex]->CharPiece = 'o';
		PlayerTurnText->SetText(FText::FromString("X's Turn"));
	}

	//add to grid when handling turn
	PrintGrid();
	TCHAR winner = CheckGameEnd();

	UE_LOG(LogTemp, Warning, TEXT("WINNER:  %s"), &winner);
}

void AMyProjectBlockGrid::PrintGrid()
{
	//UE_LOG(LogTemp, Warning, TEXT(" block grid num : %s "), BlockGrid[0]);
	for (int i = BlockGrid.Num()-1; i >= 2; i-=3)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %s %s\n"), &BlockGrid[i-2]->CharPiece, &BlockGrid[i-1]->CharPiece, &BlockGrid[i]->CharPiece);
	}
}

TCHAR AMyProjectBlockGrid::CheckLineWin(int x, int y, int z)
{
	if (BlockGrid[x]->CharPiece == '-') {
		UE_LOG(LogTemp, Warning, TEXT("1st check"));
		return 'o';
	}
	if (BlockGrid[x]->CharPiece != BlockGrid[y]->CharPiece) { 
		UE_LOG(LogTemp, Warning, TEXT("2nd check"));
		return 'o';
	}
	if (BlockGrid[y]->CharPiece != BlockGrid[z]->CharPiece) {
		UE_LOG(LogTemp, Warning, TEXT("3rd check"));
		return 'o';
	}
	return BlockGrid[x]->CharPiece;
}

TCHAR AMyProjectBlockGrid::CheckGameEnd()
{
	char Winner;
	// Horizontal lines.

	if ((Winner = CheckLineWin(6, 7, 8)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE 1st check"));
		return Winner;
	}
	if ((Winner = CheckLineWin(3, 4, 5)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE 2nd check"));
		return Winner;
	}
	if ((Winner = CheckLineWin(0, 1, 2)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE 3rd check"));
		return Winner;
	}

	// Vertical lines.

	if ((Winner = CheckLineWin(6, 3, 0)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE VERT 1st check"));
		return Winner;
	}
	if ((Winner = CheckLineWin(7, 4, 1)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE VERT 2nd check"));
		return Winner;
	}
	if ((Winner = CheckLineWin(8, 5, 2)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE VERT 3rd check"));
		return Winner;
	}

	// Diagonal lines.

	if ((Winner = CheckLineWin(6, 4, 2)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE DIAG 1st check"));
		return Winner;
	}
	if ((Winner = CheckLineWin(0, 4, 8)) != 'o') {
		UE_LOG(LogTemp, Warning, TEXT("CGE DIAG 2nd check"));
		return Winner;
	}

	return 'o';
}

#undef LOCTEXT_NAMESPACE
