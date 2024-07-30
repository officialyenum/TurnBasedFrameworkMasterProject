// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Character/TbfCharacterBase.h"
#include "GameFramework/GameModeBase.h"
#include "TbfGameMode.generated.h"

class UTbfGameInstance;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATbfGameMode();
	
	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game Instance")
	TObjectPtr<UTbfGameInstance> GI;
	
	UFUNCTION(BlueprintCallable, Category = "Game Functions Players")
	ATbfCharacter* GetPlayerOne() const;

	UFUNCTION(BlueprintCallable, Category = "Game Functions Players")
	ATbfCharacter* GetPlayerTwo() const;
	
	UFUNCTION(BlueprintCallable, Category = "Game Functions Turn")
	bool GetIsPlayerOneTurn() const;

	UFUNCTION(BlueprintCallable, Category = "Game Functions Turn")
	void SwitchTurn();
	
	UPROPERTY(BlueprintReadWrite, Category="Game Properties")
	TObjectPtr<ATbfCharacterBase> PlayerOne;
	UPROPERTY(BlueprintReadWrite, Category="Game Properties")
	TObjectPtr<ATbfCharacterBase> PlayerTwo;

	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void SetUpPlayers();
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void StartGame();
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void PauseGame();
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void QuitGame();
};
