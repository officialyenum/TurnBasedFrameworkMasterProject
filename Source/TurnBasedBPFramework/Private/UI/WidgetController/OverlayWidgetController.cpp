// Copyright Chukwuyenum Opone @officialyenum


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "UI/Widget/TbfUserWidget.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UTbfAttributeSet* PlayerAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	OnPlayerHealthChanged.Broadcast(PlayerAttributeSet->GetHealth());
	OnPlayerDrawPointsChanged.Broadcast(PlayerAttributeSet->GetDrawPoints());
	OnPlayerMovePointsChanged.Broadcast(PlayerAttributeSet->GetMovePoints());
	OnPlayerActivatePointsChanged.Broadcast(PlayerAttributeSet->GetActivatePoints());
	OnPlayerBattlePointsChanged.Broadcast(PlayerAttributeSet->GetBattlePoints());
	OnPlayerCardInDeckChanged.Broadcast(PlayerAttributeSet->GetCardInDeck());
	OnPlayerCardInHandChanged.Broadcast(PlayerAttributeSet->GetCardInHand());
	OnPlayerCardInFieldChanged.Broadcast(PlayerAttributeSet->GetCardInField());
	OnPlayerUnitInFieldChanged.Broadcast(PlayerAttributeSet->GetUnitInField());
	// Get the Ability System Component from the AI controller's pawn
	if (AActor* AIPawn = AIController->GetPawn())
	{
		if (UAbilitySystemComponent* AIASC = AIPawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			if (const UTbfAttributeSet* AIAttributeSet = AIASC->GetSet<UTbfAttributeSet>())
			{
				// Broadcast AI attribute values
				OnAIHealthChanged.Broadcast(AIAttributeSet->GetHealth());
				OnAIDrawPointsChanged.Broadcast(AIAttributeSet->GetDrawPoints());
				OnAIMovePointsChanged.Broadcast(AIAttributeSet->GetMovePoints());
				OnAIActivatePointsChanged.Broadcast(AIAttributeSet->GetActivatePoints());
				OnAIBattlePointsChanged.Broadcast(AIAttributeSet->GetBattlePoints());
				OnAICardInDeckChanged.Broadcast(AIAttributeSet->GetCardInDeck());
				OnAICardInHandChanged.Broadcast(AIAttributeSet->GetCardInHand());
				OnAICardInFieldChanged.Broadcast(AIAttributeSet->GetCardInField());
				OnAIUnitInFieldChanged.Broadcast(AIAttributeSet->GetUnitInField());
			}
		}
	}
}

void UOverlayWidgetController::BindCallbackToDependencies()
{
	// Player Bind
	const UTbfAttributeSet* PlayerAttributeSet = CastChecked<UTbfAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerHealthChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetDrawPointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerDrawPointsChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMovePointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerMovePointsChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetActivatePointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerActivatePointsChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetBattlePointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerBattlePointsChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetCardInDeckAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerCardInDeckChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetCardInHandAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerCardInHandChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetCardInFieldAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerCardInFieldChanged.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetUnitInFieldAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnPlayerUnitInFieldChanged.Broadcast(Data.NewValue);});
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
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetDrawPointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIDrawPointsChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetMovePointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIMovePointsChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetActivatePointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIActivatePointsChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetBattlePointsAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIBattlePointsChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetCardInDeckAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAICardInDeckChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetCardInHandAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAICardInHandChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetCardInFieldAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAICardInFieldChanged.Broadcast(Data.NewValue);});
				AIASC->GetGameplayAttributeValueChangeDelegate(AIAttributeSet->GetUnitInFieldAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnAIUnitInFieldChanged.Broadcast(Data.NewValue);});
	
			}
		}
	}
}
