// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_MoveCardToCell.h"

#include "AI/TbfAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"

UBTTask_MoveCardToCell::UBTTask_MoveCardToCell(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Move Selected Card To Targeted Cell";
}

EBTNodeResult::Type UBTTask_MoveCardToCell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform Card Move To Cell Action
			OwnerCharacter->PlaySelectedCard();
			// finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
