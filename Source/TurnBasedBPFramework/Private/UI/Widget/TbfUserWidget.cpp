// Copyright Chukwuyenum Opone @officialyenum


#include "UI/Widget/TbfUserWidget.h"

void UTbfUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
