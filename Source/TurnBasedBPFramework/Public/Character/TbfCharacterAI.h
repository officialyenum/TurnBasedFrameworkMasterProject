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

	// Helper methods for AI card decision-making
	UFUNCTION()
	int32 ChooseCardInHand() const;
	UFUNCTION()
	int32 ChooseCardOnField() const;
	UFUNCTION()
	ATbfGridCell* ChooseCell() const;
	
	// AI components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess="true"))
	UAlphaBetaPruningComponent* AlphaBetaPruningComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess="true"))
	UMonteCarloComponent* MonteCarloComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Params")
	UDataTable* DeckDTSim;
	
	UFUNCTION()
	void UpdateGameState();
	
protected:
	virtual void BeginPlay() override;
	FGameStateSim GameStateSim;

	// Simulated Actions
	void PopulateDeck_Sim();
	void SelectCardAndCell_Sim();
	void MoveCardToCell_Sim();
	void ActivateCard_Sim();
	void SelectUnitAndTarget_Sim();
	void AttackUnitTarget_Sim();
	void AttackPlayer_Sim();
	// Simulated Actions End
	
private:
	virtual void InitAbilityActorInfo() override;
};
