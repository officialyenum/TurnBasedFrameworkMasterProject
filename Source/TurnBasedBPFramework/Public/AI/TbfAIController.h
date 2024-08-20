// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TbfAIController.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	explicit ATbfAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
