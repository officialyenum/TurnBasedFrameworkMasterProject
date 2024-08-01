// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfWidgetController.h"
#include "UnitWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class TURNBASEDBPFRAMEWORK_API UUnitWidgetController : public UTbfWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;

	// Unit Controller Attributes
	UPROPERTY(BlueprintAssignable, Category="GAS|Unit|Attributes")
	FOnAttributeChangedSignature OnUnitHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Unit|Attributes")
	FOnAttributeChangedSignature OnUnitMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Unit|Attributes")
	FOnAttributeChangedSignature OnUnitAttackChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Unit|Attributes")
	FOnAttributeChangedSignature OnUnitMaxAttackChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Unit|Attributes")
	FOnAttributeChangedSignature OnUnitDefenceChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Unit|Attributes")
	FOnAttributeChangedSignature OnUnitMaxDefenceChanged;
};
