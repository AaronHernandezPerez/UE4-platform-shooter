// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"


#include "FPSCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	// Default scene component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	// Attach the mesh
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBase"));
	PlatformBase->SetupAttachment(RootComponent);
	// Attach the glow
	PlatformGlow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformGlow"));
	PlatformGlow->SetupAttachment(RootComponent);
	// Attach the collider
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchPad);

	// Set default forces
	ForwardForce = 500;
	UpwardForce = 1000;
}


void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
	                                 "Overlap!!!");


	FRotator CurrRotation = OverlappedComponent->GetAttachmentRoot()->GetRelativeRotation();
	FVector ForceVector = FVector(ForwardForce, 0, UpwardForce);

	ForceVector = CurrRotation.RotateVector(ForceVector);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
	                                 "Vector: " + ForceVector.ToString());

	AFPSCharacter* MyChar = Cast<AFPSCharacter>(OtherActor);

	if (MyChar)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "My Char");
		MyChar->LaunchCharacter(ForceVector, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Simulating physics " + OtherComp->GetName());
		OtherComp->AddImpulse(ForceVector, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchPadEffect, GetActorLocation());
	}
}
