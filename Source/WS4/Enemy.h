// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class UAISenseConfig_Sight;
class UAIPerceptionComponent;
class UAIPerceptionSense_Sight;
class UAIPerceptionSystem;

UCLASS()

class WS4_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BluePrintReadWrite, Category = "Movement")
		FVector _currVelocity;
	void SetNewRotation(FVector targPos, FVector currPos);
private:
	APawn* player;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
		UAIPerceptionComponent* _aiPercComp;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
		UAISenseConfig_Sight* _sightConfig;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector _baseLoc;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float _speed;
	UPROPERTY(EditAnywhere, Category = "Enemy")
		float _Radius;
	float _distSquared;
	bool _backToBase;
	UFUNCTION()
		void OnSensed(const TArray<AActor*>& testActors);
	FRotator _enemyRotator;


};
