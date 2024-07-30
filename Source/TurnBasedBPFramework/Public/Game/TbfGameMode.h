// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Character/TbfCharacterBase.h"
#include "GameFramework/GameModeBase.h"
#include "TbfGameMode.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(BlueprintReadWrite, Category="Game Properties")
	TObjectPtr<ATbfCharacterBase> PlayerOne;
	UPROPERTY(BlueprintReadWrite, Category="Game Properties")
	TObjectPtr<ATbfCharacterBase> PlayerTwo;

	bool bIsPlayerOneTurn = true;
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void SetUpPlayers(ATbfCharacterBase* POne, ATbfCharacterBase* PTwo);
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void StartGame();
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void PauseGame();
	UFUNCTION(BlueprintNativeEvent, Category="Game Functions")
	void QuitGame();
	UFUNCTION(BlueprintCallable, Category="Game Functions")
	void SwitchTurn() { bIsPlayerOneTurn = !bIsPlayerOneTurn; }
};
