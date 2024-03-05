// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectables.h"
#include "Sound/SoundBase.h"
#include "Components/SphereComponent.h"
#include "WS4Character.h"
#include "Kismet/GameplayStatics.h"
#include"WallDisappear.h"
#include"TriggerActor.h"
// Sets default values
ACollectables::ACollectables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	coll = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"));
	RootComponent = coll;
	coll->SetSimulatePhysics(true);
	coll->SetCollisionProfileName("PhysicsActor");
	coll->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void ACollectables::BeginPlay()
{
	Super::BeginPlay();
	coll->OnComponentHit.AddDynamic(this, &ACollectables::OnHit);
}

// Called every frame
void ACollectables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectables::OnHit(UPrimitiveComponent*HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AWS4Character* player = Cast<AWS4Character>(otherActor);
	if(player)
 {
		if (soundCollection != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(coll, soundCollection, NormalImpulse);

			player->onGrab(this, HitComp);

			player->health -= 5;
			
		}
 }
	ATriggerActor* tA = Cast<ATriggerActor>(otherActor);
	if (tA)
	{
		if (soundCollection != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(coll, soundCollection, NormalImpulse);
			tA->OnTrigger();
			Destroy();
		}
		
	}
}



