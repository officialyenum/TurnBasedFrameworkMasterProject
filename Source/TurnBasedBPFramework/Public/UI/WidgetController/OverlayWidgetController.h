// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfWidgetController.h"
#include "OverlayWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class TURNBASEDBPFRAMEWORK_API UOverlayWidgetController : public UTbfWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;

	// Player Controller Attributes
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerMaxHealthChanged;

	// AI Controller Attributes
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRowDelegate;
};
