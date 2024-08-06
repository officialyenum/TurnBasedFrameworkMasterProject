// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"


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

void ATbfCharacterBase::InitAbilityActorInfo()
{
}

void ATbfCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()))
	check(GameplayEffectClass)
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATbfCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}


