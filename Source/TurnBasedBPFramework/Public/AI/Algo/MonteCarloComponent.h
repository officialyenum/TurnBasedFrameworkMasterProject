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

	int32 GetAttackingUnitIndex() const { return OutAttackingUnitIndex; };
	int32 GetTargetUnitIndex() const { return OutTargetUnitIndex; };
	// Function to choose the best unit to attack with and the best target
	void ChooseBestAttack(FGameStateSim& GameState, int32 NumSimulations);

protected:
	virtual void BeginPlay() override;
	
private:
	int32 MonteCarloSimulation(FGameStateSim& SimulatedState, int32 AttackingUnitIndex, int32 TargetUnitIndex, int32 NumSimulations);
	void SimulateAttack(FGameStateSim& GameState, int32 AttackingUnitIndex, int32 TargetUnitIndex);
	int32 EvaluateBoardState(const FGameStateSim& GameState) const;
	int32 OutAttackingUnitIndex;
	int32 OutTargetUnitIndex;
};
