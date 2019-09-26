// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class ROGUE_API AKWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWeapon();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USkeletalMeshComponent* MeshComp;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun_Mechanics")
		int Range = 10000; //defines the range of the weapon., vary this for different class of weapons 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float BaseDamage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf <UDamageType> DamageType;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;


	void PlayFireEffects(FVector TracerEndPoint);


	UPROPERTY(EditAnywhere, Category = "Weapon")
	int RateOfFire;

	
	
public:	
		UFUNCTION(BlueprintCallable, Category="Weapon")
		virtual void Fire();

		

};
