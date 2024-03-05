// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnHO.generated.h"

class UBoxComponent;

UCLASS()
class WS4_API ASpawnHO : public AActor
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		UBoxComponent* whereToSpawn;

	FVector getRandomPtInVolume();
	void spawnAHo();
	UPROPERTY(EditDefaultsOnly, Category = HealthOrb)
		TSubclassOf<class AHealthOrbs> _healthorbsClass;
	
public:	
	// Sets default values for this actor's properties
	ASpawnHO();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
