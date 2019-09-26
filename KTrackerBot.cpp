// Fill out your copyright notice in the Description page of Project Settings.


#include "KTrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "..\..\Public\AI\KTrackerBot.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime\NavigationSystem\Public\NavigationSystem.h"
#include "Runtime\NavigationSystem\Public\NavigationPath.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "KHealthComponent.h"
#include "Components/SphereComponent.h"
#include "KCharacter.h"
#include "Sound/SoundCue.h"

// Sets default values
AKTrackerBot::AKTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);

	HealthComp = CreateDefaultSubobject<UKHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AKTrackerBot::HandleTakeDamage);


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	bUseVelocityChange = false;
	ForceMagnitude = 9000;
	RequiredDistanceToTarget = 100;

	ExplosionDamage = 40;
	ExplosionRadius = 200;

	SelfDestructTickTime = 0.5f;
}

// Called when the game starts or when spawned
void AKTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	FVector	NextPoint = GetNextPathPoint();
}

void AKTrackerBot::HandleTakeDamage(UKHealthComponent * OwningHealthComp, float Health, float HealthData, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());

	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

FVector AKTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return (NavPath->PathPoints[1]);
	}
	return GetActorLocation();
}

void AKTrackerBot::SelfDestruct()
{

	if (bExploded)
		return;

	bExploded = true;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);	///POSSIBLE ERROR ZONE!!

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionDamage, 12, FColor::Red, false, 2.0f, 0, 1.0f);

	UGameplayStatics::SpawnSoundAttached(ExplosionSound, RootComponent);

	Destroy();

}

void AKTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

// Called every frame
void AKTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= RequiredDistanceToTarget)
	{

		NextPathPoint = GetNextPathPoint();	///If we are close enough, we get next path point

		DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
	}
	else
	{
			//Move to next target
		///else we keep applying force to reach the path point
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		ForceDirection *= ForceMagnitude;

		MeshComp->AddImpulse(ForceDirection, NAME_None, bUseVelocityChange); 
		
		

		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation() + ForceDirection,  GetActorLocation(), 32, FColor::Emerald, false, 0.0f, 0, 1.0f);
	}

	DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Cyan, false, 0.0f, 1.50f); 
}

void AKTrackerBot::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (!bStartedSelfDestruction)
	{
		AKCharacter* PlayerPawn = Cast<AKCharacter>(OtherActor);

		if (PlayerPawn) //player overlapped, start self destruct
		{
			UGameplayStatics::SpawnSoundAttached(SelfDestructStartSound, RootComponent);
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &AKTrackerBot::DamageSelf, SelfDestructTickTime, true, 0.0f);
			bStartedSelfDestruction = true;
		}
	}

	
}

