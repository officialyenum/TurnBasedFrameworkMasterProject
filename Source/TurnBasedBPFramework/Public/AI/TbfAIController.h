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
	ATbfAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
