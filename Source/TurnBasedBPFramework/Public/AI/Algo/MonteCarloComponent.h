// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Node/MonteCarloNode.h"
#include "MonteCarloComponent.generated.h"


struct FGameStateSim;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDBPFRAMEWORK_API UMonteCarloComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMonteCarloComponent();
	virtual void BeginPlay() override;

	FName ChooseBestAttackingUnit(const FGameStateSim& GameState, int32 NumSimulations);
	FName ChooseBestTargetUnit(const FGameStateSim& GameState, int32 NumSimulations);

private:
	void SimulateAttack(FGameStateSim& GameState, int32 AttackingUnitIndex, int32 TargetUnitIndex, bool isOpponent);
	bool IsTerminalState(const FGameStateSim& State);
	int32 GetRandomLivingUnitIndex(const TArray<FTbfUnitInfoSim>& UnitField);
	UMonteCarloNode* TreePolicy(UMonteCarloNode* Root);
	int32 DefaultPolicy(FGameStateSim& State);
	void Backpropagate(UMonteCarloNode* Node, int32 Result);
	UMonteCarloNode* Expand(UMonteCarloNode* Node);
	bool AllUnitsDead(const TArray<FTbfUnitInfoSim>& UnitField);

	// These variables are used to store the best indices found
	int32 OutAttackingUnitIndex;
	int32 OutTargetUnitIndex;
};
