// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectCardFromBoard.h"

#include "AI/TbfAIController.h"
#include "Character/TbfCharacterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectCardFromBoard::UBTTask_SelectCardFromBoard(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Card From Board";
}

EBTNodeResult::Type UBTTask_SelectCardFromBoard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform CardSelection Here
			int32 CardIndex = OwnerCharacter->ChooseCardOnField();
			if(CardIndex >= 0)
			{
				OwnerCharacter->SelectedCard = OwnerCharacter->CardOnField[CardIndex];
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OwnerCharacter->CardOnField[CardIndex]);
				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
