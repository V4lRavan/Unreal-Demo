// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthOrbs.generated.h"

class USphereComponent;
class USoundBase;

UCLASS()
class WS4_API AHealthOrbs : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthOrbs();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		USphereComponent* coll;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlay)
		USoundBase* soundCollection;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
