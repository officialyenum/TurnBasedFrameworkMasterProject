// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonteCarloComponent.generated.h"


struct FGameStateSim;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDBPFRAMEWORK_API UMonteCarloComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMonteCarloComponent();

	int32 MonteCarloSimulation(FGameStateSim GameState, int32 Simulations);

protected:
	virtual void BeginPlay() override;

private:
	void PlaySelectedUnit(FGameStateSim& GameState, int32 CardIndex);
	void UndoSelectedUnit(FGameStateSim& GameState, int32 CardIndex);
	bool SimulateGame(FGameStateSim& GameState);
};
