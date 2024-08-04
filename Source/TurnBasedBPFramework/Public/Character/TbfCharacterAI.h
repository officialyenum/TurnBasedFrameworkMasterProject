// Copyright Chukwuyenum Opone @officialyenum

#pragma once
#include "TbfCharacter.h"
#include "Actor/CardBase.h"
#include "BehaviorTree/BehaviorTree.h"

struct FTbfUnitInfoSim;
struct FTbfUnitInfo;

struct FGameState
{
	int32 LifePoints;
	int32 OpponentLifePoints;
	TArray<FTbfCardInfo*> Deck;
	TArray<ACardBase*> Hand;
	TArray<ACardBase*> CardField;
	TArray<ACardBase*> OpponentCardField;
	TArray<ATbfCharacterUnit*> UnitField;
	TArray<ATbfCharacterUnit*> OpponentUnitField;
	// Add other game state variables as needed
};

struct FGameStateSim
{
	// AI
	int32 LifePoints;
	TArray<FTbfCardInfoSim> Deck;
	TArray<FTbfCardInfoSim> Hand;
	TArray<FTbfCardInfoSim> CardField;
	TArray<FTbfUnitInfoSim> UnitField;
	// Opponent
	int32 OpponentLifePoints;
	TArray<FTbfCardInfoSim> OpponentCardDeck;
	TArray<FTbfCardInfoSim> OpponentCardHand;
	TArray<FTbfCardInfoSim> OpponentCardField;
	TArray<FTbfUnitInfoSim> OpponentUnitField;
	// Add other game state variables as needed
};

#include "CoreMinimal.h"
#include "TbfCharacterBase.h"
#include "TbfCharacterAI.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacterAI : public ATbfCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATbfCharacterAI();

	UBehaviorTree* GetBehaviorTree() const;
	
	void MakeMove();
	// Helper methods for AI Game decision making
	UFUNCTION()
	void PerformDrawPhase();
	UFUNCTION()
	void PerformMainPhase();
	UFUNCTION()
	void PerformBattlePhase();
	UFUNCTION()
	void PerformEndPhase();

	// Helper methods for AI card decision-making
	UFUNCTION()
	int32 ChooseCardInHand() const;
	UFUNCTION()
	int32 ChooseCardOnField() const;
	UFUNCTION()
	ATbfGridCell* ChooseCell() const;
	UFUNCTION()
	int32 ChooseCardToAttack();

	// AlphaBeta Pruning method for AI card selection
	UFUNCTION()
	int32 AlphaBetaPruning(int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer);
	UFUNCTION()
	int32 EvaluateBoardState();

	// MonteCarlo Simulation method for AI card selection
	UFUNCTION()
	int32 MonteCarloSimulation(int32 Simulations);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Params")
	UDataTable* DeckDTSim;
	
protected:
	virtual void BeginPlay() override;
	FGameState InitialGameState;
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
	UFUNCTION()
	void SaveGameState(const UObject* WorldContextObject);
	UFUNCTION()
	void RestoreGameState(const UObject* WorldContextObject);
};
