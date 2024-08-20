// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_DrawCard.h"

#include "AI/TbfAIController.h"
#include "Character/TbfCharacterAI.h"

UBTTask_DrawCard::UBTTask_DrawCard(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Draw Card From Deck";
}

EBTNodeResult::Type UBTTask_DrawCard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			//TODO: Draw Card Logic Here
			OwnerCharacter->DrawCard();
			// finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
