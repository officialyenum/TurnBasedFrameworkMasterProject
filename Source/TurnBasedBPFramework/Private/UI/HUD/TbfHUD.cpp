// Copyright Chukwuyenum Opone @officialyenum


#include "UI/HUD/TbfHUD.h"

#include "UI/Widget/TbfUserWidget.h"
#include "UI/WidgetController/TbfWidgetController.h"
#include "UI/WidgetController/UnitWidgetController.h"

UOverlayWidgetController* ATbfHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbackToDependencies();
	}
	return OverlayWidgetController;
}

UUnitWidgetController* ATbfHUD::GetUnitWidgetController(const FWidgetControllerParams& WCParams)
{
	if (UnitWidgetController == nullptr)
	{
		UnitWidgetController = NewObject<UUnitWidgetController>(this, UnitWidgetControllerClass);
		UnitWidgetController->SetWidgetControllerParams(WCParams);
		UnitWidgetController->BindCallbackToDependencies();
	}
	return UnitWidgetController;
}

void ATbfHUD::InitOverlay(APlayerController* PC, AAIController* AC, APlayerState* PS, UAbilitySystemComponent* ASC,
                          UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_TbfHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_TbfHUD"))
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UTbfUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC,AC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
