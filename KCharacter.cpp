// Fill out your copyright notice in the Description page of Project Settings.


#include "KCharacter.h"
#include "..\Public\KCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


// Sets default values
AKCharacter::AKCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 60.0f;
	ZoomInSpeed = 20;

	//debug PART
	CamLag = 4.0f;

	WeaponAttachSocketName = "WeaponSocket";

	SprintMultiplier = 2.0f;

	
}

// Called when the game starts or when spawned
void AKCharacter::BeginPlay()
{
	Super::BeginPlay();

  

	DefaultFOV = CameraComp->FieldOfView;	//Caches the dafault FOV during start of the game, needed to return to original FOV.
	
	//Spawns default weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	CurrentWeapon = GetWorld()->SpawnActor<AKWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
	}
}

void AKCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector()* Value);
}

void AKCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector()* Value);
}

void AKCharacter::BeginCrouch()
{
	Crouch();
	
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = CamLag;
}

void AKCharacter::EndCrouch()
{
	UnCrouch();
	
	SpringArmComp->bEnableCameraLag = false;
}



void AKCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void AKCharacter::EndZoom()
{
	bWantsToZoom = false;
}



void AKCharacter::Fire()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}

}

//Sprint is BUGGYY

void AKCharacter::Sprint()
{
	
	GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
}

void AKCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
}

// Called every frame
void AKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV;

	if (bWantsToZoom)
	{
		TargetFOV = ZoomedFOV;
	}
	else
	{
		TargetFOV = DefaultFOV;
	}

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInSpeed);

	CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void AKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &AKCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ScanS", this, &AKCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AKCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("UnCrouch", IE_Pressed, this, &AKCharacter::EndCrouch);
   

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AKCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AKCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AKCharacter::Fire);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AKCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AKCharacter::EndSprint);

}

FVector AKCharacter::GetPawnViewLocation() const
{

	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();		//if cameraComp is NOT NULL returns the it.
	}

	return Super::GetPawnViewLocation(); //returns the default implementation.
}
