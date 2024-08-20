// Copyright Chukwuyenum Opone @officialyenum


#include "AbilitySystem/TbfAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UTbfAttributeSet::UTbfAttributeSet()
{
	InitHealth(10000.0f);
	
	InitDrawPoints(5.0f);
	InitMovePoints(2.0f);
	InitActivatePoints(2.0f);
	InitBattlePoints(0.0f);

	InitAttack(0.0f);
	InitDefence(0.0f);

	InitCardInDeck(40.0f);
	InitCardInHand(0.0f);
	InitCardInField(0.0f);
	InitUnitInField(0.0f);
}

void UTbfAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	/** Health */
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, Health, COND_None, REPNOTIFY_Always);
	
	/** Game */
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, DrawPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, MovePoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, ActivatePoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, BattlePoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, CardInDeck, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, CardInHand, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, CardInField, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTbfAttributeSet, UnitInField, COND_None, REPNOTIFY_Always);
}

void UTbfAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, Health, OldHealth);
}

void UTbfAttributeSet::OnRep_DrawPoints(const FGameplayAttributeData& OldDrawPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, DrawPoints, OldDrawPoints);
}

void UTbfAttributeSet::OnRep_MovePoints(const FGameplayAttributeData& OldMovePoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, MovePoints, OldMovePoints);
}

void UTbfAttributeSet::OnRep_ActivatePoints(const FGameplayAttributeData& OldActivatePoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, ActivatePoints, OldActivatePoints);
}

void UTbfAttributeSet::OnRep_BattlePoints(const FGameplayAttributeData& OldBattlePoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, BattlePoints, OldBattlePoints);
}

void UTbfAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, Attack, OldAttack);
}

void UTbfAttributeSet::OnRep_Defence(const FGameplayAttributeData& OldDefence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, Defence, OldDefence);
}

void UTbfAttributeSet::OnRep_UnitInField(const FGameplayAttributeData& OldUnitInField) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, UnitInField, OldUnitInField);
}

void UTbfAttributeSet::OnRep_CardInHand(const FGameplayAttributeData& OldCardInHand) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, CardInHand, OldCardInHand);
}

void UTbfAttributeSet::OnRep_CardInDeck(const FGameplayAttributeData& OldCardInDeck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, CardInDeck, OldCardInDeck);
}

void UTbfAttributeSet::OnRep_CardInField(const FGameplayAttributeData& OldCardInField) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTbfAttributeSet, CardInField, OldCardInField);
}
