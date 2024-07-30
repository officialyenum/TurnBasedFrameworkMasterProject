// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterBase.h"



ATbfCharacterBase::ATbfCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* ATbfCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATbfCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


