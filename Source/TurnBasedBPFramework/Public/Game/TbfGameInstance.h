// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Actor/CardBase.h"
#include "Engine/GameInstance.h"
#include "TbfGameInstance.generated.h"

class ATbfCharacter;
class ATbfCharacterUnit;
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

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	FText WinningMessage;
	
	UFUNCTION(BlueprintCallable, Category="Game")
	void CheckWinner();
	
	UFUNCTION(BlueprintCallable, Category="AI")
	void SetCardAlgorithm(ECardAlgo Algo);
	UFUNCTION(BlueprintCallable, Category="AI")
	void SetUnitAlgorithm(EUnitAlgo Algo);

	UFUNCTION(BlueprintCallable, Category="AI")
	ECardAlgo GetCardAlgorithm() { return CardAlgorithm; };
	UFUNCTION(BlueprintCallable, Category="AI")
	EUnitAlgo GetUnitAlgorithm() { return UnitAlgorithm; };
	
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ACardBase*> PlayerOneFieldedCards();
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ACardBase*> PlayerTwoFieldedCards();

	
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ATbfCharacterUnit*> PlayerOneFieldedUnits();
	UFUNCTION(BlueprintCallable, Category= "Card Trackers")
	TArray<ATbfCharacterUnit*> PlayerTwoFieldedUnits();
protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI Algorithm")
	ECardAlgo CardAlgorithm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI Algorithm")
	EUnitAlgo UnitAlgorithm;
};
