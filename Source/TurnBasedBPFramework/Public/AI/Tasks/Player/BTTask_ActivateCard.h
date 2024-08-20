// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ActivateCard.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UBTTask_ActivateCard : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_ActivateCard(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
