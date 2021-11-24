// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay() {
    Super::BeginPlay();

    if(AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        APawn* ControlledPawn = AController::GetPawn();

        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), ControlledPawn->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime );
}

bool AShooterAIController::IsDead() const {
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

	if(ControlledCharacter != nullptr) {
		return ControlledCharacter->IsDead();
	}

	return true;
}