// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "DrawDebugHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"	//to register sight sense
#include "Perception/AIPerceptionSystem.h" //to register this pawn
#include "WS4Character.h"

 //Sets default values
AEnemy::AEnemy()
{
	_sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	_aiPercComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_currVelocity.X = 0;
	_currVelocity.Y = 0;
	_currVelocity.Z = 0;
	_speed = 200.0f;
	_Radius = 500.0f;
	_distSquared = BIG_NUMBER;
	_backToBase = false;
	_sightConfig->SightRadius = _Radius;
	_sightConfig->LoseSightRadius = _Radius + 100.0f;
	_sightConfig->PeripheralVisionAngleDegrees = 90;
	_sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	_sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	_sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	_sightConfig->SetMaxAge(0.5);

	_aiPercComp->ConfigureSense(*_sightConfig);
	_aiPercComp->SetDominantSense(_sightConfig->Implementation);
	_aiPercComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);
}

//Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	player = GetWorld()->GetFirstPlayerController()->GetPawn();
	_baseLoc = GetActorLocation();
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(GetWorld(), UAISense_Sight::StaticClass(), this);
}

//Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UWorld* world = GetWorld();
	FVector playerLocation = player->GetActorLocation();
	FVector enemyLocation = GetActorLocation();
	FVector requiredDirection = playerLocation - enemyLocation;
	requiredDirection.Z = 0;
	
	FVector newLocation = enemyLocation;

	if (!_currVelocity.IsZero())
	{
		newLocation = (_currVelocity * DeltaTime) + enemyLocation;
	}
	if (_backToBase)
	{
		if (((newLocation - _baseLoc).SizeSquared2D()) < _distSquared)
		{
			_distSquared = (newLocation - _baseLoc).SizeSquared2D();

		}
		else
		{
			_currVelocity.X = 0;
			_currVelocity.Y = 0;
			_currVelocity.Z = 0;
			_distSquared = BIG_NUMBER;
			_backToBase = false;
		}
	}
	else 
	{
		SetActorRotation(requiredDirection.Rotation());
	}
	SetActorLocation(newLocation);
}

//Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetNewRotation(FVector targPos, FVector currPos)
{
	FVector dir;
	dir = targPos - currPos;
	dir.Z = 0.0f;
	_enemyRotator = dir.Rotation() - GetActorRotation();
	SetActorRotation(dir.Rotation());
}

void AEnemy::OnSensed(const TArray<AActor*>& testActors)
{
	FVector enemyLocation = GetActorLocation();
	for (int i = 0; i < testActors.Num(); i++)
	{
		FString n;
		n = testActors[i]->GetName();
		FActorPerceptionBlueprintInfo info;
		_aiPercComp->GetActorsPerception(testActors[i], info);
		if (info.LastSensedStimuli.Num() > 0 && info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			FVector dir = testActors[i]->GetActorLocation() - enemyLocation;
			dir.Z = 0.0f;
			_currVelocity = dir.GetSafeNormal() * _speed;
			
		}
		else
		{
			
			FVector dir = _baseLoc - enemyLocation;
			dir.Z = 0.0f;
			if (dir.SizeSquared() > 1.0f)
			{
				_currVelocity = dir.GetSafeNormal() * _speed;
				_backToBase = true;
				SetNewRotation(_baseLoc, GetActorLocation());
				
			}

		}
	}

}





