// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlphaBetaPruningComponent.generated.h"


struct FGameStateSim;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDBPFRAMEWORK_API UAlphaBetaPruningComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAlphaBetaPruningComponent();
	
	int32 ChooseBestCard(const FGameStateSim& GameState, int32 Depth);

protected:
	virtual void BeginPlay() override;

private:
	int32 AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer);

	void SimulatePlayCard(FGameStateSim& GameState, int32 CardIndex);
	void SimulateOpponentPlayCard(FGameStateSim& GameState, int32 CardIndex);
	int32 EvaluateBoardState(const FGameStateSim& GameState) const;
};
