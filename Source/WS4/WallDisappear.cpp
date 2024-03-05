// Fill out your copyright notice in the Description page of Project Settings.


#include "WallDisappear.h"
#include"Collectables.h"
#include"WS4Character.h"
#include "Components/BoxComponent.h"
#include"Collectables.h"
// Sets default values
AWallDisappear::AWallDisappear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWallDisappear::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallDisappear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool ff;
	for (ATriggerActor* t : tA)
	{
		if (t)
		{
			ff &= t->hasTriggered();
		}
	}
		if (ff)
		{
			DestroyWall();
		}
	
}

void AWallDisappear::DestroyWall()
{
	wall->ConditionalBeginDestroy();
}



