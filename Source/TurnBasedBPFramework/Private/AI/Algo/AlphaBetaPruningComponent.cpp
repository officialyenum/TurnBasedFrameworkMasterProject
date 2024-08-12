// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/AlphaBetaPruningComponent.h"

#include "Character/TbfCharacterAI.h"


// Sets default values for this component's properties
UAlphaBetaPruningComponent::UAlphaBetaPruningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAlphaBetaPruningComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

int32 UAlphaBetaPruningComponent::AlphaBetaPruning(FGameStateSim GameState, int32 Depth, int32 Alpha, int32 Beta,
	bool bIsMaximizingPlayer)
{
	return 0;
}

int32 UAlphaBetaPruningComponent::EvaluateBoardState(const FGameStateSim& GameState)
{
	return 0;
}

void UAlphaBetaPruningComponent::PlaySelectedCard(FGameStateSim& GameState, int32 CardIndex)
{
}

void UAlphaBetaPruningComponent::UndoSelectedCard(FGameStateSim& GameState, int32 CardIndex)
{
}



