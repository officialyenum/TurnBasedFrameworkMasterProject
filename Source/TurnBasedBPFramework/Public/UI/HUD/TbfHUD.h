// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/TbfUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/UnitWidgetController.h"
#include "TbfHUD.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfHUD : public AHUD
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly, Category="Overlay Widget")
	TObjectPtr<UTbfUserWidget> OverlayWidget;
	UPROPERTY(BlueprintReadOnly, Category="Overlay Widget")
	TObjectPtr<UUserWidget> PauseWidget;
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UUnitWidgetController* GetUnitWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, AAIController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Pause UI Actions")
	void PauseGameEvent();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Pause UI Actions")
	void ResumeGameEvent();
private:
	UPROPERTY(EditAnywhere, Category="Tbf HUD")
	TSubclassOf<UTbfUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere, Category="Tbf HUD")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere, Category="Tbf HUD")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UUnitWidgetController> UnitWidgetController;
	
	UPROPERTY(EditAnywhere, Category="Yegun HUD")
	TSubclassOf<UUnitWidgetController> UnitWidgetControllerClass;
};
