// Copyright Chukwuyenum Opone @officialyenum


#include "AbilitySystem/TbfAbilitySystemBlueprintLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/TbfGameMode.h"
#include "Kismet/GameplayStatics.h"

void UTbfAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                                    ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	ATbfGameMode* TbfGameMode = Cast<ATbfGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	if (TbfGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* ClassInfo = TbfGameMode->CharacterClassInfo;

	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}
