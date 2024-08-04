// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_ActivateCard.h"

#include "AI/TbfAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"

class ATbfCharacterAI;

UBTTask_ActivateCard::UBTTask_ActivateCard(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Activate Selected Card";
}

EBTNodeResult::Type UBTTask_ActivateCard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform CardSelection Here
			OwnerCharacter->ActivateSelectedCard();
			// finish with success 
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
