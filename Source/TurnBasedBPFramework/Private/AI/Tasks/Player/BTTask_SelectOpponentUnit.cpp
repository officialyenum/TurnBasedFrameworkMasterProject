// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectOpponentUnit.h"

#include "AI/TbfAIController.h"
#include "Character/TbfCharacterAI.h"

UBTTask_SelectOpponentUnit::UBTTask_SelectOpponentUnit(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Opponent Unit";
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
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
