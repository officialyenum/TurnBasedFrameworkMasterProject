// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "TbfWidgetController.generated.h"


USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UTbfUserWidget> MessageWidget;
};

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, AAIController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), AIController(AC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, FUIWidgetRow, Row);
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbackToDependencies();
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAIController> AIController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
