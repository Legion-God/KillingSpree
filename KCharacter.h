// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KWeapon.h"
#include "KCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UKHealthComponent;
class USoundCue;

UCLASS()
class ROGUE_API AKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomedFOV;

	float DefaultFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UKHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInSpeed;

	void BeginZoom();

	void EndZoom();

	UPROPERTY(EditAnywhere, Category = "Player")
	float CamLag;

	AKWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<AKWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category= "Player")
	FName WeaponAttachSocketName;

	void Fire();

	UPROPERTY(EditAnywhere, Category = "Player")
	float SprintMultiplier;

	void Sprint();

	void EndSprint();

	float DefaultMaxWalkSpeed=900.0f;
	
	UFUNCTION()
	void OnHealthChanged(UKHealthComponent* OwningHealthComp, float Health, float HealthData, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool bDied;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	USoundCue* ZoomIn;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	USoundCue* ZoomOut;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	USoundCue* JumpSound;

	void KJump();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override; //gets pawn eye location
															//override redefines base implementation
};
