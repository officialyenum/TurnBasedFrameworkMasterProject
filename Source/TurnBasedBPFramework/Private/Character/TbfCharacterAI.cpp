// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterAI.h"


// Sets default values
ATbfCharacterAI::ATbfCharacterAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATbfCharacterAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATbfCharacterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATbfCharacterAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

