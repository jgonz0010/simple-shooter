// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection) {
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr) {return false;}

	FVector NewLocation;
	FRotator NewRotation;
	OwnerController->GetPlayerViewPoint(NewLocation, NewRotation);
	ShotDirection = -NewRotation.Vector();

	FVector EndLocation = NewLocation + NewRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, NewLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);	
}

AController* AGun::GetOwnerController() const {
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) {return nullptr;}

	return OwnerPawn->GetController();
}
void AGun::PullTrigger() {
	//spawn a particle effect
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if(bSuccess) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location);

		AActor* ActorHit = Hit.GetActor();
		if(ActorHit != nullptr) {
			FPointDamageEvent DamageEvent(10, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			if(OwnerController == nullptr) {return;}
			ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	} 
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

