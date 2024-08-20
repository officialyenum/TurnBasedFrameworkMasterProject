// Copyright Chukwuyenum Opone @officialyenum


#include "UI/WidgetController/TbfWidgetController.h"

void UTbfWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AIController = WCParams.AIController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTbfWidgetController::BroadcastInitialValues()
{
}

void UTbfWidgetController::BindCallbackToDependencies()
{
}
