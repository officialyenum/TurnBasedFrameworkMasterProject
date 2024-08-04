// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Tasks/Player/BTTask_SelectUnit.h"

#include "AI/TbfAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TbfCharacterAI.h"
#include "Character/TbfCharacterUnit.h"
#include "Library/TbfGameFunctionLibrary.h"

UBTTask_SelectUnit::UBTTask_SelectUnit(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Tbf Select Unit On Your Board";
}

EBTNodeResult::Type UBTTask_SelectUnit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller and Character
	if (ATbfAIController* const OwnerController = Cast<ATbfAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATbfCharacterAI* const OwnerCharacter = Cast<ATbfCharacterAI>( OwnerController->GetPawn()))
		{
			// Perform Unit Selection Here
			int32 CardIndex = OwnerCharacter->ChooseCardOnField();
			ATbfCharacterUnit* Unit = UTbfGameFunctionLibrary::GetRandomUnitForPlayer(OwnerCharacter);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Unit);
			// finish with success
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;

}
