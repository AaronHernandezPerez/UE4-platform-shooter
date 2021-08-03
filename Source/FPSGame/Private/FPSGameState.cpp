// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

#include "FPSPlayerController.h"

void AFPSGameState::MultiCastOnMissionCompleted_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	UE_LOG(LogTemp, Log, TEXT("MultiCastOnMissionCompleted_Implementation"))
	// for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	// {
	// 	APawn* Pawn = It->Get();
	// 	if (Pawn && Pawn->IsLocallyControlled())
	// 	{
	// 		Pawn->DisableInput(nullptr);
	// 	}
	// }

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			// Disable input
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				Pawn->DisableInput(PC);
			}
		}
	}
}
