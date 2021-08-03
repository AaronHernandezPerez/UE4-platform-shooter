// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "AIController.h"
#include "GameFramework/Character.h"

#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class ATargetPoint;
class AAIController;


UENUM(BlueprintType)
enum class EAIState: uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnPawnHeard(APawn* HeardPawn, const FVector& Location, float Volume);

	FTimerHandle TimerHandle_ReturnOrientation;
	FRotator OriginalRotation;
	UFUNCTION()
	void ResetOrientation();

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState(); // Only on clients

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category="AI")
	void OnStateChange(EAIState NewState);

	UPROPERTY(EditAnywhere, Category="AI")
	TArray<ATargetPoint*> PatrolTargets;

	bool bIsMoving;
	int8 currentIndex;

	void MoveToActor(AActor* ToActor);
	AActor* GetNextPatrolTarget();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
