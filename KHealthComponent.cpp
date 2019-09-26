// Fill out your copyright notice in the Description page of Project Settings.


#include "KHealthComponent.h"
#include "GameFramework\Actor.h"
#include "..\..\Public\Components\KHealthComponent.h"

// Sets default values for this component's properties
UKHealthComponent::UKHealthComponent()
{


	// ...
	DefaultHealth = 100;
}


// Called when the game starts
void UKHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UKHealthComponent::HandleTakeAnyDamage);
	}
	Health = DefaultHealth;
}

void UKHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health))

		OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}



