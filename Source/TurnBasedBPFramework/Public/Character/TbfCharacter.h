// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "TbfCharacterBase.h"
#include "TbfCharacter.generated.h"

struct FTbfCardInfo;
class ATbfGridCell;

UENUM(BlueprintType)
enum class ETbfPlayerState: uint8
{
	Waiting,
	Draw,
	MainOne,
	Battle,
	MainTwo
};

class ATbfCharacterUnit;
class ACardBase;

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacter : public ATbfCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATbfCharacter();
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void DrawCard();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void PlaySelectedCard();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void PlaySelectedUnitBattle();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void ActivateSelectedCard();

	UFUNCTION(BlueprintCallable, Category="Gameplay Modifiers")
	void ResetCounters();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Modifiers")
	void RepositionCardInHand();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void GoToNextState();

	// START UI RELATED LOGIC
	UFUNCTION(BlueprintImplementableEvent, Category="Gameplay Actions")
	void UpdateUIStat();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Gameplay Actions")
	void ShowMessageInUI(const FText& Text);
	// END UI RELATED LOGIC
	
	// START Player Param
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	int32 id = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	bool bIsPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	TArray<FTbfCardInfo>  Deck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	TArray<ACardBase*> Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	TArray<ACardBase*> CardOnField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	TArray<ATbfCharacterUnit*> UnitOnField;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	int32 CardSpace = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	int32 DrawCountPerTurn = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	int32 MoveCountPerTurn = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	int32 ActivateCountPerTurn = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	int32 BattleCountPerTurn = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Params" )
	ETbfPlayerState CurrentState = ETbfPlayerState::Waiting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	UDataTable* DeckDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	ATbfCharacterUnit* SelectedUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	ATbfCharacterUnit* TargetedUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	ACardBase* SelectedCard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	ACardBase* TargetedCard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Params")
	ATbfGridCell* TargetedCell;
	// End Player Param

	// Handler Logic
	UFUNCTION()
	void HandleUnitDestroyed(AActor* DestroyedActor);
	UFUNCTION()
	void HandleCardDestroyed(AActor* DestroyedActor);
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Params")
	TSubclassOf<ACardBase> CardClass;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> CardSpawnDirectionArea;
};
