// Copyright Chukwuyenum Opone @officialyenum

#pragma once
#include "TbfCharacter.h"
#include "Actor/CardBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CoreMinimal.h"
#include "TbfCharacterAI.generated.h"

class UMonteCarloComponent;
class UAlphaBetaPruningComponent;



UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacterAI : public ATbfCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATbfCharacterAI();

	UBehaviorTree* GetBehaviorTree() const;

	// START: Helper methods for AI card and unit selection decision-making
	UFUNCTION()
	int32 ChooseCardInHand();
	UFUNCTION()
	int32 ChooseCardOnField();
	UFUNCTION()
	int32 ChooseUnitOnField() const;
	UFUNCTION()
	int32 ChooseOpponentUnitOnField() const;
	UFUNCTION()
	ATbfGridCell* ChooseCell() const;
	UFUNCTION()
	void UpdateGameState();
	// END: Helper methods for AI card and unit selection decision-making

	
	// AI components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess="true"))
	UAlphaBetaPruningComponent* AlphaBetaPruningComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess="true"))
	UMonteCarloComponent* MonteCarloComponent;
	
	// AI Algorithms
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI Algorithm")
	ECardAlgo SelectedCardAlgorithm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI Algorithm")
	EUnitAlgo SelectedUnitAlgorithm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Params")
	UDataTable* DeckDTSim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Params")
	FGameStateSim GameStateSim;
	
	
protected:
	virtual void BeginPlay() override;
	
private:
	virtual void InitAbilityActorInfo() override;
};
