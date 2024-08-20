// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectTargetCell.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UBTTask_SelectTargetCell : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_SelectTargetCell(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
