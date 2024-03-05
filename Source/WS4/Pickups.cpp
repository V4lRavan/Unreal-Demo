// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"
#include "Sound/SoundBase.h"
#include "Components/SphereComponent.h"
#include "WS4Character.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
APickups::APickups()
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
void APickups::BeginPlay()
{
	Super::BeginPlay();
	coll->OnComponentHit.AddDynamic(this, &APickups::OnHit);
}

// Called every frame
void APickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickups::OnHit(UPrimitiveComponent* HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AWS4Character* player = Cast<AWS4Character>(otherActor);
	if (player)
	{
			UGameplayStatics::PlaySoundAtLocation(coll, soundCollection, NormalImpulse);
			player->score+=5;
		
	Destroy();	
	}
}

