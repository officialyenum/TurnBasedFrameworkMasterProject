// Copyright Chukwuyenum Opone @officialyenum


#include "AbilitySystem/TbfAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UTbfAttributeSet::UTbfAttributeSet()
{
	InitHealth(8000.0f);
	InitAttack(5000.0f);
	InitDefence(5000.0f);
	
	InitMaxHealth(8000.0f);
	InitMaxAttack(5000.0f);
	InitMaxDefence(5000.0f);
}

void UTbfAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	/** Vital */
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	
	/** Secondary */
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, MaxAttack, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, MaxDefence, COND_None, REPNOTIFY_Always);
}

void UTbfAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, Health, OldHealth);
}

void UTbfAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, MaxHealth, OldMaxHealth);
}

void UTbfAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, Attack, OldAttack);
}

void UTbfAttributeSet::OnRep_MaxAttack(const FGameplayAttributeData& OldMaxAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, MaxAttack, OldMaxAttack);
}

void UTbfAttributeSet::OnRep_Defence(const FGameplayAttributeData& OldDefence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, Defence, OldDefence);
}

void UTbfAttributeSet::OnRep_MaxDefence(const FGameplayAttributeData& OldMaxDefence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, MaxDefence, OldMaxDefence);
}
