// Copyright Chukwuyenum Opone @officialyenum

#pragma once
#include "TbfCharacter.h"
#include "Actor/CardBase.h"

struct FGameState
{
	int32 LifePoints;
	int32 OpponentLifePoints;
	TArray<ACardBase*> Deck;
	TArray<ACardBase*> Hand;
	TArray<ACardBase*> CardField;
	TArray<ACardBase*> OpponentCardField;
	TArray<ATbfCharacterUnit*> UnitField;
	TArray<ATbfCharacterUnit*> OpponentUnitField;
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
	
	void MakeMove();
protected:
	virtual void BeginPlay() override;
	FGameState InitialGameState;
	// Helper methods for AI Game decision making
	void PerformDrawPhase();
	void PerformMainPhase();
	void PerformBattlePhase();
	void PerformEndPhase();

	// Helper methods for AI card decision-making
	int32 ChooseCardToPlay() const;
	int32 ChooseCardToAttack();

	// AlphaBeta Pruning method for AI card selection
	int32 AlphaBetaPruning(int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer);
	int32 EvaluateBoardState();

	// MonteCarlo Simulation method for AI card selection
	int32 MonteCarloSimulation(int32 Simulations);

private:
	void SaveGameState(const UObject* WorldContextObject);
	void RestoreGameState(const UObject* WorldContextObject);
};
