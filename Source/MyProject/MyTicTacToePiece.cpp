// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTicTacToePiece.h"

// Sets default values
AMyTicTacToePiece::AMyTicTacToePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTicTacToePiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTicTacToePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

