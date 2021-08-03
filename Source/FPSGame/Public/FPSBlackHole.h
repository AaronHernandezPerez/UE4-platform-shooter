// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSBlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	USphereComponent* GravityPullSphere;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USphereComponent* SingularitySphere;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* SingularityMesh;
	UPROPERTY(EditDefaultsOnly, Category="Black Hole")
	float PullStrength;

	UFUNCTION()
	void OverlapSingularitySphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
