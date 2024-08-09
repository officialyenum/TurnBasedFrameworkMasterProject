// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectTargetCell.h"

#include "Actor/TbfGridCell.h"
#include "AI/TbfAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"

UBTTask_SelectTargetCell::UBTTask_SelectTargetCell(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Target Cell";
}

EBTNodeResult::Type UBTTask_SelectTargetCell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform CardSelection Here
			OwnerCharacter->TargetedCell = OwnerCharacter->ChooseCell();
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OwnerCharacter->TargetedCell);
			// finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
