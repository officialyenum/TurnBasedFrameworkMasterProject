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

	GENERATED_BODY()

public:
	UMonteCarloComponent();

	UFUNCTION(BlueprintCallable)
	FName ChooseBestAttackingUnit(const FGameStateSim& GameState, int32 NumSimulations);

	UFUNCTION(BlueprintCallable)
	FName ChooseBestTargetUnit(const FGameStateSim& GameState, int32 NumSimulations);

protected:
	virtual void BeginPlay() override;

private:
	UMonteCarloNode* TreePolicy(UMonteCarloNode* Root);
	UMonteCarloNode* Expand(UMonteCarloNode* Node);
	int32 DefaultPolicy(FGameStateSim& State);
	void Backpropagate(UMonteCarloNode* Node, int32 Result);
	bool IsTerminalState(const FGameStateSim& State);
	int32 GetRandomLivingUnitIndex(const TArray<FTbfUnitInfoSim>& UnitField);
	void SimulateAttack(FGameStateSim& GameState, int32 AttackingUnitIndex, int32 TargetUnitIndex, bool IsOpponent);
	bool AllUnitsDead(const TArray<FTbfUnitInfoSim>& UnitField);

	int32 OutAttackingUnitIndex;
	int32 OutTargetUnitIndex;
	FTbfUnitInfoSim BestAttackingUnit;
	FTbfUnitInfoSim BestTargetUnit;
};
