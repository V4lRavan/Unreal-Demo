// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnHO.h"
#include"HealthOrbs.h"
#include"Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnHO::ASpawnHO()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	whereToSpawn->SetBoxExtent(FVector(1000.0, 500.0, 20.0));
	whereToSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawnHO::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < 6; i++)
		spawnAHo();
}

// Called every frame
void ASpawnHO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnHO::getRandomPtInVolume()
{
	FVector spawnOrigin = whereToSpawn->Bounds.Origin;
	FVector spawnExtent = whereToSpawn->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void ASpawnHO::spawnAHo()
{
	if (GetWorld())
	{
		FActorSpawnParameters spawnParam;
		spawnParam.Owner = this;
		spawnParam.Instigator = GetInstigator();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector spawnPos = getRandomPtInVolume();
		FRotator rot = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AHealthOrbs>(_healthorbsClass, spawnPos, rot, spawnParam);
	}
}


