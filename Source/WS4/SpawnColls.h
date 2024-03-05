// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnColls.generated.h"

class UBoxComponent;
class Collectables;

UCLASS()
class WS4_API ASpawnColls : public AActor
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		UBoxComponent* whereToSpawn;

	FVector getRandomPtInVolume();
	void spawnAHo();
	UPROPERTY(EditDefaultsOnly, Category = PickUps)
		TSubclassOf<class ACollectables> _collectablesClass;
public:	
	// Sets default values for this actor's properties
	ASpawnColls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
