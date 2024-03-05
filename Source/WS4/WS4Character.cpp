// Copyright Epic Games, Inc. All Rights Reserved.

#include "WS4Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h" 

//////////////////////////////////////////////////////////////////////////
// AWS4Character

AWS4Character::AWS4Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	physicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	health = 100.0f;
	grabActor = nullptr;
	grabComp = nullptr;
	_UIMessage = "Grab collectables and throw toward the Goal Area";
	score = 40;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWS4Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWS4Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWS4Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWS4Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWS4Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AWS4Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AWS4Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AWS4Character::OnResetVR);
	//drop and throw
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AWS4Character::dropObject);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AWS4Character::throwObject);
}

void AWS4Character::Tick(float DeltaTime)
{
	
}

void AWS4Character::onGrab(AActor* a, UPrimitiveComponent* HitComp)
{
	if(grabActor == nullptr && HitComp->IsSimulatingPhysics())
	{
		grabActor = Cast<AActor>(a);
		grabComp = HitComp;
		compCollisionType = grabComp->GetCollisionEnabled();
		grabComp->SetSimulatePhysics(false);
		grabComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		physicsHandle->GrabComponentAtLocation(grabComp, FName("None"), grabActor->GetActorLocation());
		const USkeletalMeshSocket *socket= GetMesh()->GetSocketByName(FName("CollSocket"));
		if (socket)
		{
			grabActor->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), TEXT("CollSocket"));
		}
	}
		
}

void AWS4Character::dropObject()
{
	if (grabActor)
	{	
		detachObject();
		if(grabComp)
		grabComp->AddImpulse(GetActorForwardVector() * 10.0f, NAME_None, true);
	}
}

void AWS4Character::detachObject()
{
	grabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	grabComp->SetSimulatePhysics(true);
	grabComp->SetCollisionEnabled(compCollisionType);
	grabActor = nullptr;
}

void AWS4Character::throwObject()
{
	if (grabActor)
	{
		detachObject();
		if(grabComp)
		grabComp->AddImpulse(GetActorForwardVector() * impulseStrength, NAME_None, true);
	}
}


void AWS4Character::OnResetVR()
{
	// If WS4 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in WS4.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AWS4Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AWS4Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AWS4Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWS4Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AWS4Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWS4Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
