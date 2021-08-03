// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

	USceneComponent* RootSceneComponent;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* PlatformBase;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* PlatformGlow;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UBoxComponent* BoxCollider;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UParticleSystem* LaunchPadEffect;

	UPROPERTY(EditAnywhere, Category="Launch")
	float UpwardForce;
	UPROPERTY(EditAnywhere, Category="Launch")
	float ForwardForce;


	UFUNCTION()
	void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);
public:
};
