// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TbfUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
