// Copyright Chukwuyenum Opone @officialyenum


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "UI/Widget/TbfUserWidget.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UTbfAttributeSet* PlayerAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	OnPlayerHealthChanged.Broadcast(PlayerAttributeSet->GetHealth());
	OnPlayerMaxHealthChanged.Broadcast(PlayerAttributeSet->GetMaxHealth());
	// Get the Ability System Component from the AI controller's pawn
	if (AActor* AIPawn = AIController->GetPawn())
	{
		if (UAbilitySystemComponent* AIASC = AIPawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (const UTbfAttributeSet* AIAttributeSet = AIASC->GetSet<UTbfAttributeSet>())
			{
				// Broadcast AI attribute values
				OnAIHealthChanged.Broadcast(AIAttributeSet->GetHealth());
				OnAIMaxHealthChanged.Broadcast(AIAttributeSet->GetMaxHealth());
			}
		}
	}
}

void UOverlayWidgetController::BindCallbackToDependencies()
{
	// Player Bind
	const UTbfAttributeSet* PlayerAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerMaxHealthChanged.Broadcast(Data.NewValue);});
	// Cast<UTbfAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
	// 	[this](const FGameplayTagContainer& AssetTags)
	// 	{
			
			// for (const FGameplayTag& Tag : AssetTags)
			// {
			// 	// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
			// 	FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			// 	if(Tag.MatchesTag(MessageTag))
			// 	{
			// 		//TODO: Broadcast tag to Widget Controller
			// 		const FString Msg = FString::Printf(TEXT("Broadcasted GE Tags : %s"), *Tag.GetTagName().ToString());
			// 		GEngine->AddOnScreenDebugMessage(-1, 8.f,FColor::Blue,Msg);
			// 		FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			// 		OnMessageWidgetRowDelegate.Broadcast(*Row);
			// 	}
			// }
		// }
	// );

	//AI Bind
	// Get the Ability System Component from the AI controller's pawn
	if (AActor* AIPawn = AIController->GetPawn())
	{
		if (UAbilitySystemComponent* AIASC = AIPawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (const UTbfAttributeSet* AIAttributeSet = AIASC->GetSet<UTbfAttributeSet>())
			{
				// Broadcast AI attribute values
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIHealthChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIMaxHealthChanged.Broadcast(Data.NewValue);});
			}
		}
	}
}
