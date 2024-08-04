// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_AttackOpponentUnit.h"

#include "AI/TbfAIController.h"
#include "Character/TbfCharacterAI.h"

class ATbfCharacterAI;
class ATbfAIController;

UBTTask_AttackOpponentUnit::UBTTask_AttackOpponentUnit(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Attack Selected Target Unit";
}

EBTNodeResult::Type UBTTask_AttackOpponentUnit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			//TODO: Perform Attack Opponent Unit Logic Here
			// finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
