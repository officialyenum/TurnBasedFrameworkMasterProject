// Copyright Chukwuyenum Opone @officialyenum


#include "AI/TbfAIController.h"


// Sets default values
ATbfAIController::ATbfAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATbfAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATbfAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

