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
	FOnAttributeChangedSignature OnPlayerDrawPointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerMovePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerActivatePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerBattlePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerCardInDeckChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerCardInHandChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerUnitInFieldChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Player|Attributes")
	FOnAttributeChangedSignature OnPlayerCardInFieldChanged;

	// AI Controller Attributes
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIDrawPointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIMovePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIActivatePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIBattlePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAICardInDeckChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAICardInHandChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAIUnitInFieldChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|AI|Attributes")
	FOnAttributeChangedSignature OnAICardInFieldChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRowDelegate;
};
