// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_RunMonteCarloSimulation.h"

#include "AI/TbfAIController.h"
#include "AI/Algo/MonteCarloComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"
#include "Character/TbfCharacterUnit.h"
#include "Library/TbfGameFunctionLibrary.h"

UBTTask_RunMonteCarloSimulation::UBTTask_RunMonteCarloSimulation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Run Monte Carlo Simulation";
}

EBTNodeResult::Type UBTTask_RunMonteCarloSimulation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform Unit Selection Here
			if (
			OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::MonteCarlo_Random || 
				OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::Random_MonteCarlo ||
				OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::MonteCarlo_MonteCarlo
				)
			{
				OwnerCharacter->UpdateGameState();
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OwnerCharacter->SelectedUnit);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
