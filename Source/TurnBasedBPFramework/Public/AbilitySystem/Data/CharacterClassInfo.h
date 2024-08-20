// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass: uint8
{
	Player,
	Unit,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category= "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category= "Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
};

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category= "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);
};
