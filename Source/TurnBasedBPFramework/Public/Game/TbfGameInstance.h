// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TbfGameInstance.generated.h"

class ATbfCharacter;
class ATbfCharacterUnit;
class ACardBase;
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
	TObjectPtr<ATbfCharacter> PlayerOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Players")
	TObjectPtr<ATbfCharacter> PlayerTwo;
	
	UFUNCTION(BlueprintCallable, Category= "Player Setup")
	void SetPlayerOne(ATbfCharacter* Player);
	
	UFUNCTION(BlueprintCallable, Category= "Player Setup")
	void SetPlayerTwo(ATbfCharacter* Player);
	
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ACardBase*> PlayerOneFieldedCards();
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ACardBase*> PlayerTwoFieldedCards();

	
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ATbfCharacterUnit*> PlayerOneFieldedUnits();
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ATbfCharacterUnit*> PlayerTwoFieldedUnits();
};
