// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnColls.h"
#include"Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include"Collectables.h"


// Sets default values
ASpawnColls::ASpawnColls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	whereToSpawn->SetBoxExtent(FVector(1000.0, 500.0, 20.0));
	whereToSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawnColls::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < 3; i++)
		spawnAHo();
}

// Called every frame
void ASpawnColls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnColls::getRandomPtInVolume()
{
	FVector spawnOrigin = whereToSpawn->Bounds.Origin;
	FVector spawnExtent = whereToSpawn->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void ASpawnColls::spawnAHo()
{
	if (GetWorld())
	{
		FActorSpawnParameters spawnParam;
		spawnParam.Owner = this;
		spawnParam.Instigator = GetInstigator();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector spawnPos = getRandomPtInVolume();
		FRotator rot = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<ACollectables>(_collectablesClass, spawnPos, rot, spawnParam);
	}
}
