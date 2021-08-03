// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SingularityMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SingularityMeshText"));
	SingularityMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = SingularityMesh;

	SingularitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Singularity"));
	SingularitySphere->SetupAttachment(RootComponent);
	// SingularitySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// SingularitySphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	SingularitySphere->SetSphereRadius(100);
	SingularitySphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapSingularitySphere);

	GravityPullSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GravityPullSphere"));
	GravityPullSphere->SetupAttachment(RootComponent);
	// GravityPullSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// GravityPullSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	GravityPullSphere->SetSphereRadius(2000);

	PullStrength = -4000;
}

void AFPSBlackHole::OverlapSingularitySphere(UPrimitiveComponent* OverlappedComp,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex,
                                             bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Test tick!");
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> PulledComps;
	GravityPullSphere->GetOverlappingComponents(PulledComps);

	for (auto PulledComp : PulledComps)
	{
		if (PulledComp->IsSimulatingPhysics())
		{
			PulledComp->AddRadialForce(GetActorLocation(),
			                           GravityPullSphere->GetScaledSphereRadius(),
			                           PullStrength, RIF_Linear, true);
		}
	}
}
