// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectUnit.h"

#include "AI/TbfAIController.h"
#include "AI/Algo/MonteCarloComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"
#include "Character/TbfCharacterUnit.h"
#include "Library/TbfGameFunctionLibrary.h"

UBTTask_SelectUnit::UBTTask_SelectUnit(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Unit On Your Board";
}

EBTNodeResult::Type UBTTask_SelectUnit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform Unit Selection Here
			OwnerCharacter->SelectedUnit =
				OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::MonteCarlo_Random || OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::MonteCarlo_MonteCarlo
				? UTbfGameFunctionLibrary::GetPlayerUnitByIndex(OwnerCharacter, OwnerCharacter->ChooseUnitOnField())
				: OwnerCharacter->SelectedUnit = UTbfGameFunctionLibrary::GetRandomUnitForPlayer(OwnerCharacter);
			if (!OwnerCharacter->SelectedUnit && OwnerCharacter->CurrentState == ETbfPlayerState::Battle)
			{
				OwnerCharacter->GoToNextState();
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OwnerCharacter->SelectedUnit);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;

}
