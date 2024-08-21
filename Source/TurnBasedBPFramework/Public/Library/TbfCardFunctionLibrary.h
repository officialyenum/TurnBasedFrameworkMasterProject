// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Actor/CardBase.h"
#include "Character/TbfCharacterAI.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TbfCardFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfCardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static TArray<FTbfCardInfoSim> GetDeckSim(UDataTable* DataTable);
	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static TArray<FTbfCardInfo> GetDeck(UDataTable* DataTable);
	
	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static FTbfCardInfo GetRandomCardFromDataTable(UDataTable* DataTable);

	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static FTbfCardInfo GetRandomCardByType(UDataTable* DataTable, ECardType CardType);

	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static FTbfCardInfo GetRandomUnitCard(UDataTable* DataTable);

	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static FTbfCardInfo GetRandomSpellCard(UDataTable* DataTable);

	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static FTbfCardInfo GetRandomTrapCard(UDataTable* DataTable);
	
	UFUNCTION(BlueprintCallable, Category = "Card DataTable")
	static FTbfCardInfoSim ConvertCardToSim(FGameStateSim State, FName Card);
	
	UFUNCTION(BlueprintCallable, Category = "Card Library")
	static ACardBase* GetRandomCardForPlayer(ATbfCharacter* Player);
	
	UFUNCTION(BlueprintCallable, Category = "Card Library")
	static TArray<ACardBase*> GetAllCardForPlayer(ATbfCharacter* Player);
};
