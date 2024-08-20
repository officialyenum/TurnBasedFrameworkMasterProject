// Copyright Chukwuyenum Opone @officialyenum


#include "AI/TbfUnitAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Character/TbfCharacterUnit.h"


ATbfUnitAIController::ATbfUnitAIController(FObjectInitializer const& ObjectInitializer)
{
}

void ATbfUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ATbfCharacterUnit* const AI = Cast<ATbfCharacterUnit>(InPawn))
	{
		if (UBehaviorTree* const tree = AI->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
			
		}
	}
}
