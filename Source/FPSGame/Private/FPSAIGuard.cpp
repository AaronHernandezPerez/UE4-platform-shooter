// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"


#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "NavigationSystem.h"
#include "UnrealNetwork.h"
#include "Engine/TargetPoint.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnPawnHeard);

	OriginalRotation = GetActorRotation();
	GuardState = EAIState::Idle;

	currentIndex = 0;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	if (PatrolTargets.Num() > 0)
	{
		bIsMoving = true;
		MoveToActor(PatrolTargets[currentIndex]);
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);
	SetGuardState(EAIState::Alerted);
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	Controller->StopMovement();
	bIsMoving = false;
}

void AFPSAIGuard::OnPawnHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);
	Controller->StopMovement();
	bIsMoving = false;

	FVector Direciton = Location - GetActorLocation();
	Direciton.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direciton).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ReturnOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ReturnOrientation, this, &AFPSAIGuard::ResetOrientation, 3.f, false);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	bIsMoving = true;
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChange(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
	OnRep_GuardState();
	OnStateChange(GuardState);
}

void AFPSAIGuard::MoveToActor(AActor* ToActor)
{
	AAIController* AIController = Cast<AAIController>(Controller);
	if (AIController)
	{
		EPathFollowingRequestResult::Type ResultType = AIController->MoveToActor(ToActor, 5, true);
		UE_LOG(LogTemp, Log, TEXT("Moved"));
	}
}

AActor* AFPSAIGuard::GetNextPatrolTarget()
{
	currentIndex++;
	if (currentIndex >= PatrolTargets.Num())
	{
		currentIndex = 0;
	}

	return PatrolTargets[currentIndex];
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Terrible way of doing it buuuut, instead we should create an new Ai controller and overwrite OnMoveCompleted https://www.vikram.codes/blog/ai/01-basic-navigation
	if (!bIsMoving || PatrolTargets.Num() == 0)
	{
		return;
	}
	AAIController* AIController = Cast<AAIController>(Controller);
	if (AIController && AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		MoveToActor(GetNextPatrolTarget());
	}
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
