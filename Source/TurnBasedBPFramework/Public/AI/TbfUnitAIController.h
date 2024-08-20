// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TbfUnitAIController.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	explicit ATbfUnitAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
