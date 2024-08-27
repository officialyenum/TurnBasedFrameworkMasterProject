// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Actor/TbfGridCell.h"
#include "Character/TbfCharacter.h"
#include "Actor/CardBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TbfGameFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static ATbfGridCell* GetRandomCellForPlayer(const ATbfCharacter* Player);

	static ATbfCharacterUnit* GetRandomUnitForPlayer(const ATbfCharacter* Player);
	static ATbfCharacterUnit* GetPlayerUnitByIndex(const ATbfCharacter* Player, const int32 UnitIndex);
	static ATbfCharacterUnit* GetRandomOpponentUnit(const ATbfCharacter* Player);
	static ATbfCharacterUnit* GetOpponentUnitByIndex(const ATbfCharacter* Player, const int32 UnitIndex);
	static TArray<ATbfCharacterUnit*> GetAllUnitForPlayer(const ATbfCharacter* Player);
	static TArray<ATbfCharacterUnit*> GetOpponentUnits(const ATbfCharacter* Player);
	
	static FString PlayerStateToString(ETbfPlayerState State);
};
