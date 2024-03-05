// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalArea.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include"Collectables.h"
#include "Kismet/GameplayStatics.h"
#include"WS4Character.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGoalArea::AGoalArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	coll = CreateDefaultSubobject<UBoxComponent>(FName("Collision Component"));
	RootComponent = coll;
	coll->SetSimulatePhysics(true);
	coll->SetCollisionProfileName("OverlapAll");
	coll->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

// Called when the game starts or when spawned
void AGoalArea::BeginPlay()
{
	Super::BeginPlay();
	coll->OnComponentBeginOverlap.AddDynamic(this, &AGoalArea::OverlapBegin);
}

// Called every frame
void AGoalArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoalArea::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWS4Character* player = Cast<AWS4Character>(OtherActor);
	if (player->score==50)
	{
		if (victoryS != NULL)
		{
			
				UE_LOG(LogTemp,Error,TEXT("You won"));
				UGameplayStatics::PlaySoundAtLocation(coll, victoryS, OtherActor->GetActorLocation());
		}
		
	}
}

