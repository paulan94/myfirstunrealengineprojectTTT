// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class AMyProjectBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AActor> OPieceActorToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AActor> XPieceActorToSpawn;

protected:
	
	UFUNCTION()
	void SpawnOX(bool bIsO, FVector Loc, FRotator Rot); //redo logic for the bool to use charpiece

public:
	AMyProjectBlock();

	/** Are we currently active? */
	bool bIsActive;

	TCHAR CharPiece;

	//PAN CHANGES
	int32 BlockIndex;

	AActor* SpawnedPiece;


	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	/** Pointer to red material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* RedMaterial;

	/** Pointer to red material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* GreenMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class AMyProjectBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	void HandleClicked();

	void Highlight(bool bOn);

	void ChangeColorOnWin();

	void ResetBlock();


public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};



