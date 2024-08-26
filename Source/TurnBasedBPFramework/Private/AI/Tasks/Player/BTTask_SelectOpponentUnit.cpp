// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectOpponentUnit.h"

#include "AI/TbfAIController.h"
#include "AI/Algo/MonteCarloComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"
#include "Library/TbfGameFunctionLibrary.h"

UBTTask_SelectOpponentUnit::UBTTask_SelectOpponentUnit(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Unit On Opponent Board";
}

EBTNodeResult::Type UBTTask_SelectOpponentUnit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			//TODO: Perform Opponent To Attack Selection
			OwnerCharacter->TargetedUnit =
				OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::Random_MonteCarlo || OwnerCharacter->SelectedUnitAlgorithm == EUnitAlgo::MonteCarlo_MonteCarlo
			? UTbfGameFunctionLibrary::GetOpponentUnitByIndex(OwnerCharacter, OwnerCharacter->ChooseOpponentUnitOnField())
			: UTbfGameFunctionLibrary::GetRandomOpponentUnit(OwnerCharacter);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OwnerCharacter->TargetedUnit);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
