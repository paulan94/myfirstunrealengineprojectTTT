// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectBlock.h"
#include "MyProjectBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"


AMyProjectBlock::AMyProjectBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, RedMaterial(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
			, GreenMaterial(TEXT("/Game/Puzzle/Meshes/GreenMaterial.GreenMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	static ConstructorHelpers::FObjectFinder<UBlueprint> OPieceSpawnObject(TEXT("Blueprint'/Game/Puzzle/OPiece_Blueprint.OPiece_Blueprint'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> XPieceSpawnObject(TEXT("Blueprint'/Game/Puzzle/XPiece_Blueprint.XPiece_Blueprint'"));

	OPieceActorToSpawn = (UClass*)OPieceSpawnObject.Object->GeneratedClass;
	XPieceActorToSpawn = (UClass*)XPieceSpawnObject.Object->GeneratedClass;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMyProjectBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMyProjectBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	GreenMaterial = ConstructorStatics.GreenMaterial.Get();

	//set o and x 
	CharPiece = '-';
	
}

void AMyProjectBlock::SpawnOX(bool bIsO, FVector Loc, FRotator Rot)
{
	//todo: this is where we spawn the O or X for visibility
	FActorSpawnParameters SpawnParams;

	if (bIsO) {
		//try to ::subclass() if not work
		GetWorld()->SpawnActor<AActor>(OPieceActorToSpawn, Loc, Rot, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Spawned O at Loc: %s Rot: %s"), *Loc.ToString(), *Rot.ToString());
	}
	else {
		GetWorld()->SpawnActor<AActor>(XPieceActorToSpawn, Loc, Rot, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Spawned X at Loc: %s Rot: %s"), *Loc.ToString(), *Rot.ToString());
	}
}


void AMyProjectBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AMyProjectBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AMyProjectBlock::HandleClicked()
{
	//TODO: update grid when clicked

	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		FVector BlockLoc = GetActorLocation();
		FRotator BlockRot = GetActorRotation();

		BlockLoc.Z += 100;

		if (OwningGrid->BP1Turn) {
			// Change material
			SpawnOX(true, BlockLoc, BlockRot);
			BlockMesh->SetMaterial(0, OrangeMaterial);
		}
		else if (OwningGrid->BP2Turn) {
			// Change material
			SpawnOX(false, BlockLoc, BlockRot);
			BlockMesh->SetMaterial(0, RedMaterial);
		}

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->HandleTurn(BlockIndex);
		}
	}
}

void AMyProjectBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void AMyProjectBlock::ChangeColorOnWin()
{
	BlockMesh->SetMaterial(0, GreenMaterial);
}
