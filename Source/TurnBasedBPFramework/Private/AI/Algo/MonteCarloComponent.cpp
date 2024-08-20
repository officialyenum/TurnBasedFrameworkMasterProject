// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/MonteCarloComponent.h"

#include "Character/TbfCharacterAI.h"


// Sets default values for this component's properties
UMonteCarloComponent::UMonteCarloComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

int32 UMonteCarloComponent::MonteCarloSimulation(FGameStateSim GameState, int32 Simulations)
{
	return 0;
}

void UMonteCarloComponent::PlaySelectedUnit(FGameStateSim& GameState, int32 UnitIndex)
{
}

void UMonteCarloComponent::UndoSelectedUnit(FGameStateSim& GameState, int32 UnitIndex)
{
}

bool UMonteCarloComponent::SimulateGame(FGameStateSim& GameState)
{
	return true;
}


// Called when the game starts
void UMonteCarloComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

