// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectCardFromHand.h"

#include "AI/TbfAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"
#include "Library/TbfCardFunctionLibrary.h"
#include "Library/TbfGameFunctionLibrary.h"

UBTTask_SelectCardFromHand::UBTTask_SelectCardFromHand(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Card From Hand";
}

EBTNodeResult::Type UBTTask_SelectCardFromHand::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform Card Selection
			int32 CardIndex = OwnerCharacter->ChooseCardInHand();
			if (CardIndex >= 0)
			{
				OwnerCharacter->SelectedCard = OwnerCharacter->Hand[CardIndex];
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), OwnerCharacter->Hand[CardIndex]);
				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			OwnerCharacter->GoToNextState();
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
