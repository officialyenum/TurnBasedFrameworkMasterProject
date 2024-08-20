// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectOpponentUnit.h"

#include "AI/TbfAIController.h"
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

			// finish with success
			ATbfCharacterUnit* OpponentUnit = UTbfGameFunctionLibrary::GetRandomOpponentUnit(OwnerCharacter);
			OwnerCharacter->TargetedUnit = OpponentUnit;
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OpponentUnit);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
