// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KTrackerBot.generated.h"


class UKHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class ROGUE_API AKTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComp;
	

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UKHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UFUNCTION()
	void HandleTakeDamage(UKHealthComponent* OwningHealthComp, float Health, float HealthData, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();

	FVector NextPathPoint; //Next point in Navigation Path

	UPROPERTY(EditDefaultsOnly ,Category="TrackerBot")
	float ForceMagnitude; 

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	UMaterialInstanceDynamic* MatInst; ///dynamic mat, pulses on damage.

	void SelfDestruct();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	bool bExploded;

	bool bStartedSelfDestruction;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;


	FTimerHandle TimerHandle_SelfDamage;

	void DamageSelf();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDestructTickTime;		//freq at which bot damages itself

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		USoundCue* SelfDestructStartSound;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
