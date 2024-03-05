// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"TriggerActor.h"
#include "WallDisappear.generated.h"


UCLASS()
class WS4_API AWallDisappear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallDisappear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = ATriggerActor)
		ATriggerActor* tA[3];
	UPROPERTY(EditAnywhere, Category = wall)
		AActor* wall;
	void DestroyWall();
	
};
