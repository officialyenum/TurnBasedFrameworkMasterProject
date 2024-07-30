// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TbfGameInstance.generated.h"

class ATbfCharacterUnit;
class ACardBase;
class ATbfCharacterBase;
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	UTbfGameInstance();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Turn")
	bool bIsPlayerOneTurn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Players")
	TObjectPtr<ATbfCharacterBase> PlayerOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Players")
	TObjectPtr<ATbfCharacterBase> PlayerTwo;

	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ACardBase*> PlayerOneFieldedCards();
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ACardBase*> PlayerTwoFieldedCards();

	
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ATbfCharacterUnit*> PlayerOneFieldedUnits();
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ATbfCharacterUnit*> PlayerTwoFieldedUnits();
};
