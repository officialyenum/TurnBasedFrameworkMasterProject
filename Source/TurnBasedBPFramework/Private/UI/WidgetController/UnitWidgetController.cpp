// Copyright Chukwuyenum Opone @officialyenum


#include "UI/WidgetController/UnitWidgetController.h"

#include "AbilitySystem/TbfAttributeSet.h"


void UUnitWidgetController::BroadcastInitialValues()
{
	const UTbfAttributeSet* UnitAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	OnUnitHealthChanged.Broadcast(UnitAttributeSet->GetHealth());
	OnUnitMaxHealthChanged.Broadcast(UnitAttributeSet->GetMaxHealth());
	OnUnitAttackChanged.Broadcast(UnitAttributeSet->GetAttack());
	OnUnitMaxAttackChanged.Broadcast(UnitAttributeSet->GetMaxAttack());
	OnUnitDefenceChanged.Broadcast(UnitAttributeSet->GetDefence());
	OnUnitMaxDefenceChanged.Broadcast(UnitAttributeSet->GetMaxDefence());
}

void UUnitWidgetController::BindCallbackToDependencies()
{
	// Unit Bind
	const UTbfAttributeSet* UnitAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitMaxHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetAttackAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitAttackChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetMaxAttackAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitMaxAttackChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetDefenceAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitDefenceChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetMaxDefenceAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitMaxDefenceChanged.Broadcast(Data.NewValue);});
	
}
