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

	PlayerTurnText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerTurnText0"));
	PlayerTurnText->SetRelativeLocation(FVector(250.0f, 0.f, 0.f));
	PlayerTurnText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	PlayerTurnText->SetText(FText::FromString("O's Turn"));
	PlayerTurnText->SetupAttachment(DummyRoot);	
	
	OWinText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("OWinText0"));
	OWinText->SetupAttachment(DummyRoot);

	XWinText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("XWinText0"));
	XWinText->SetupAttachment(DummyRoot);

	// Set defaults *dont change Size for tic-tac-toe
	Size = 3;
	BlockSpacing = 300.f;

	P1Score = 0; //O
	P2Score = 0; //X

	IsGameEnded = false;

}

//RM this if i can
void AMyProjectBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	StartGame();
}

void AMyProjectBlockGrid::StartGame()
{
	// Number of blocks
	const int32 NumBlocks = Size * Size;

	BP1Turn = true;
	BP2Turn = false;

	int32 x = 3;
	int32 y = 3;
	int32 BlockIndex = 0;

	for (int32 BlockX = 0; BlockX < x; BlockX++)
	{

		for (int32 BlockY = 0; BlockY < y; BlockY++) {

			const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

																	  // Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			// Spawn a block
			AMyProjectBlock* NewBlock = GetWorld()->SpawnActor<AMyProjectBlock>(BlockLocation, FRotator(0, 0, 0));

			NewBlock->BlockIndex = BlockIndex;
			BlockGrid.Add(NewBlock);

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

	if (IsGameEnded) return;

	// Update text
	if (BP1Turn) {
		BP1Turn = false;
		BP2Turn = true;
		BlockGrid[BlockIndex]->CharPiece = 'O';
		PlayerTurnText->SetText(FText::FromString("X's Turn"));
	}
	else {
		BP1Turn = true;
		BP2Turn = false;
		BlockGrid[BlockIndex]->CharPiece = 'X';
		PlayerTurnText->SetText(FText::FromString("O's Turn"));
	}

	
	PrintGrid(); //Debug purposes only
	CheckGameStalemate();
	TCHAR winner = CheckGameEnd();

}

void AMyProjectBlockGrid::PrintGrid()
{
	for (int i = BlockGrid.Num()-1; i >= 2; i-=3)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %s %s\n"), &BlockGrid[i-2]->CharPiece, &BlockGrid[i-1]->CharPiece, &BlockGrid[i]->CharPiece);
	}
}

TCHAR AMyProjectBlockGrid::CheckLineWin(int x, int y, int z)
{
	if (BlockGrid[x]->CharPiece == '-') {
		return '-';
	}
	if (BlockGrid[x]->CharPiece != BlockGrid[y]->CharPiece) { 
		return '-';
	}
	if (BlockGrid[y]->CharPiece != BlockGrid[z]->CharPiece) {
		return '-';
	}
	return BlockGrid[x]->CharPiece;
}

TCHAR AMyProjectBlockGrid::CheckGameEnd()
{
	char Winner;


	// Horizontal
	if ((Winner = CheckLineWin(6, 7, 8)) != '-') {
		ChangeColorOnWinGrid(6, 8, 1);
		return Winner;
	}
	if ((Winner = CheckLineWin(3, 4, 5)) != '-') {
		ChangeColorOnWinGrid(3, 5, 1);
		return Winner;
	}
	if ((Winner = CheckLineWin(0, 1, 2)) != '-') {
		ChangeColorOnWinGrid(0, 2, 1);
		return Winner;
	}

	// Vertical

	if ((Winner = CheckLineWin(6, 3, 0)) != '-') {
		ChangeColorOnWinGrid(0, 6, 3);
		return Winner;
	}
	if ((Winner = CheckLineWin(7, 4, 1)) != '-') {
		ChangeColorOnWinGrid(1, 7, 3);
		return Winner;
	}
	if ((Winner = CheckLineWin(8, 5, 2)) != '-') {
		ChangeColorOnWinGrid(2, 8, 3);
		return Winner;
	}

	// Diagonal

	if ((Winner = CheckLineWin(6, 4, 2)) != '-') {
		ChangeColorOnWinGrid(2,6, 2);
		return Winner;
	}
	if ((Winner = CheckLineWin(0, 4, 8)) != '-') {
		ChangeColorOnWinGrid(0, 8, 4);
		return Winner;
	}

	return '-';
}


void AMyProjectBlockGrid::CheckGameStalemate() {
	for (int32 i = 0; i < BlockGrid.Num(); i++) {
		if (BlockGrid[i]->CharPiece == '-') {
			return;
		}
	}
	IsGameEnded = true;
	GameEndDelay();
}

void AMyProjectBlockGrid::ChangeColorOnWinGrid(int32 start, int32 end, int32 addBy)
{
	//change text for winner
	IsGameEnded = true;
	for (int32 i = start; i <= end; i+= addBy) {
		BlockGrid[i]->ChangeColorOnWin();
	}
	GameEndDelay();

}

void AMyProjectBlockGrid::HandleGameEnd()
{
	TCHAR winner = CheckGameEnd();
	if (winner == 'X') {
		P2Score++;
		XWinText->SetText(FString::Printf(TEXT("X won %d times."), P2Score));
	}
	else if (winner == 'O') {
		P1Score++;
		OWinText->SetText(FString::Printf(TEXT("O won %d times."), P1Score));
	}
	for (int32 i = 0; i < BlockGrid.Num(); i++) {
		BlockGrid[i]->ResetBlock();
	}
	IsGameEnded = false;
}

void AMyProjectBlockGrid::GameEndDelay()
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		if (IsGameEnded) {
			HandleGameEnd();
		}
		}, 2, 1);
}

#undef LOCTEXT_NAMESPACE
