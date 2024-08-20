// Copyright Chukwuyenum Opone @officialyenum


#include "UI/WidgetController/UnitWidgetController.h"

#include "AbilitySystem/TbfAttributeSet.h"


void UUnitWidgetController::BroadcastInitialValues()
{
	const UTbfAttributeSet* UnitAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	OnUnitAttackChanged.Broadcast(UnitAttributeSet->GetAttack());
	OnUnitDefenceChanged.Broadcast(UnitAttributeSet->GetDefence());
}

void UUnitWidgetController::BindCallbackToDependencies()
{
	// Unit Bind
	const UTbfAttributeSet* UnitAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetAttackAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitAttackChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UnitAttributeSet->GetDefenceAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnUnitDefenceChanged.Broadcast(Data.NewValue);});
	
}
