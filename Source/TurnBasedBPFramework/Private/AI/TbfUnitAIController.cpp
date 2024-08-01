// Copyright Chukwuyenum Opone @officialyenum


#include "AI/TbfUnitAIController.h"


// Sets default values
ATbfUnitAIController::ATbfUnitAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATbfUnitAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATbfUnitAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

