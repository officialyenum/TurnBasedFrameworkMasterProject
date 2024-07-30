// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TbfCardFunctionLibrary.generated.h"

struct FTbfCardInfo;
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfCardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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
};
