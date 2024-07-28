// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacter.h"


// Sets default values
ATbfCharacter::ATbfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATbfCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATbfCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATbfCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

