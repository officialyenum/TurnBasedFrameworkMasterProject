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
	
	UPROPERTY()
	TObjectPtr<UTbfUserWidget> OverlayWidget;
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UUnitWidgetController* GetUnitWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC, AAIController* AC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
private:
	UPROPERTY(EditAnywhere, Category="Tbf HUD")
	TSubclassOf<UTbfUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere, Category="Tbf HUD")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UUnitWidgetController> UnitWidgetController;
	
	UPROPERTY(EditAnywhere, Category="Yegun HUD")
	TSubclassOf<UUnitWidgetController> UnitWidgetControllerClass;
};
