// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/AlphaBetaPruningComponent.h"

#include "Character/TbfCharacterAI.h"


// Sets default values for this component's properties
UAlphaBetaPruningComponent::UAlphaBetaPruningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

int32 UAlphaBetaPruningComponent::ChooseBestCard(const FGameStateSim& GameState, int32 Depth)
{
	int32 BestCardIndex = INDEX_NONE;
	int32 BestValue = INT32_MIN;

	for (int32 i = 0; i < GameState.Hand.Num(); ++i)
	{
		FGameStateSim SimulatedState = GameState;

		// Simulate playing the card
		SimulatePlayCard(SimulatedState, i);

		// Perform Alpha-Beta Pruning on the simulated state
		int32 Value = AlphaBetaPruning(SimulatedState, Depth, INT32_MIN, INT32_MAX, false);

		if (Value > BestValue)
		{
			BestValue = Value;
			BestCardIndex = i;
		}
	}

	return BestCardIndex;
}

void UAlphaBetaPruningComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UAlphaBetaPruningComponent::AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta,
	bool bIsMaximizingPlayer)
{
	if (Depth == 0)
	{
		return EvaluateBoardState(GameState);
	}

	if (bIsMaximizingPlayer)
	{
		int32 MaxEval = INT32_MIN;

		for (int32 i = 0; i < GameState.Hand.Num(); ++i)
		{
			FGameStateSim SimulatedState = GameState;

			// Simulate playing the card
			SimulatePlayCard(SimulatedState, i);

			int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, false);
			MaxEval = FMath::Max(MaxEval, Eval);
			Alpha = FMath::Max(Alpha, Eval);

			if (Beta <= Alpha)
			{
				break;
			}
		}

		return MaxEval;
	}
	else
	{
		int32 MinEval = INT32_MAX;

		// Assuming opponent plays optimally; simulate opponent's best response
		for (int32 i = 0; i < GameState.OpponentCardHand.Num(); ++i)
		{
			FGameStateSim SimulatedState = GameState;

			// Simulate opponent playing the card
			SimulatePlayCard(SimulatedState, i);

			int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, true);
			MinEval = FMath::Min(MinEval, Eval);
			Beta = FMath::Min(Beta, Eval);

			if (Beta <= Alpha)
			{
				break;
			}
		}

		return MinEval;
	}
}

void UAlphaBetaPruningComponent::SimulatePlayCard(FGameStateSim& GameState, int32 CardIndex)
{
	// Retrieve the card from the hand
	FTbfCardInfoSim& CardToPlay = GameState.Hand[CardIndex];

	switch (CardToPlay.Type)
	{
	case ECardType::Unit:
		{
			// Add the unit to the field
			GameState.UnitField.Add(CardToPlay.Unit);
			break;
		}
	case ECardType::Spell:
		{
			// Apply spell effects (modify attack, defense, etc.)
			if (CardToPlay.ModifierParam == EModifierParam::Attack)
			{
				for (FTbfUnitInfoSim& Unit : GameState.UnitField)
				{
					if (CardToPlay.ModifierType == EModifierType::Add)
					{
						Unit.Attack += CardToPlay.ModifierValue;
					}
					else if (CardToPlay.ModifierType == EModifierType::Multiply)
					{
						Unit.Attack *= CardToPlay.ModifierValue;
					}
				}
			}
			else if (CardToPlay.ModifierParam == EModifierParam::Defence)
			{
				for (FTbfUnitInfoSim& Unit : GameState.UnitField)
				{
					if (CardToPlay.ModifierType == EModifierType::Add)
					{
						Unit.Defence += CardToPlay.ModifierValue;
					}
					else if (CardToPlay.ModifierType == EModifierType::Multiply)
					{
						Unit.Defence *= CardToPlay.ModifierValue;
					}
				}
			}
			break;
		}
	case ECardType::Trap:
		{
			// Set the trap on the field
			GameState.CardField.Add(CardToPlay);
			break;
		}
	default:
		break;
	}

	// Remove the card from the AI's hand after it's been played
	GameState.Hand.RemoveAt(CardIndex);
}

void UAlphaBetaPruningComponent::SimulateOpponentPlayCard(FGameStateSim& GameState, int32 CardIndex)
{
}

int32 UAlphaBetaPruningComponent::EvaluateBoardState(const FGameStateSim& GameState) const
{
	// board evaluation:
	int32 Value = 0;

	// Add value based on life points
	Value += GameState.LifePoints - GameState.OpponentLifePoints;

	// Add value based on the number and strength of units on the field
	for (const FTbfUnitInfoSim& Unit : GameState.UnitField)
	{
		Value += Unit.Attack + Unit.Defence;
	}

	// Subtract value based on opponent's units
	for (const FTbfUnitInfoSim& OpponentUnit : GameState.OpponentUnitField)
	{
		Value -= OpponentUnit.Attack + OpponentUnit.Defence;
	}

	return Value;
}
