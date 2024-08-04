// Copyright Chukwuyenum Opone @officialyenum


#include "AI/TbfAIController.h"

#include "Character/TbfCharacterAI.h"


ATbfAIController::ATbfAIController(FObjectInitializer const& ObjectInitializer)
{
}

void ATbfAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ATbfCharacterAI* const AI = Cast<ATbfCharacterAI>(InPawn))
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
