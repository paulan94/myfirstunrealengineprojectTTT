// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectBlock.h"
#include "MyProjectBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AMyProjectBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the player turn */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* PlayerTurnText;

	/** Text component for the player turn */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* OWinText;

	/** Text component for the player turn */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* XWinText;

public:
	AMyProjectBlockGrid();

	int32 P1Score;
	int32 P2Score;

	bool BP1Turn;
	bool BP2Turn;

	bool IsGameEnded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
	TArray<AMyProjectBlock*> BlockGrid;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	void StartGame();
	// End AActor interface
	

public:

	/** Handle the block being clicked */
	void HandleTurn(int BlockIndex);

	TCHAR CheckLineWin(int x, int y, int z);

	/** Check game end */
	TCHAR CheckGameEnd();

	void CheckGameStalemate();

	//pattern: 1-> horizontal, 2-> vertical, 3/4 -> diagonal patterns
	void ChangeColorOnWinGrid(int32 start, int32 end, int32 addBy);

	void HandleGameEnd();

	void GameEndDelay();

	void PrintGrid();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	/** Returns PlayerTurnText subobject **/
	FORCEINLINE class UTextRenderComponent* GetPlayerTurnText() const { return PlayerTurnText; }
};

