// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/AlphaBetaPruningComponent.h"

#include "Character/TbfCharacterAI.h"
#include "Field/FieldSystemNoiseAlgo.h"


// Sets default values for this component's properties
UAlphaBetaPruningComponent::UAlphaBetaPruningComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

FName UAlphaBetaPruningComponent::ChooseBestCardInHand(const FGameStateSim& GameState, int32 Depth)
{
	int32 BestCardIndex = INDEX_NONE;
	int32 BestValue = INT32_MIN;

	for (int32 i = 0; i < GameState.Hand.Num() - 1; ++i)
	{
		FGameStateSim SimulatedState = GameState;

		// Simulate playing the card
		SimulatePlayCard(SimulatedState, i, false);

		// Perform Alpha-Beta Pruning on the simulated state
		int32 Value = AlphaBetaPruning(SimulatedState, Depth, INT32_MIN, INT32_MAX, false, false);

		if (Value > BestValue)
		{
			BestValue = Value;
			BestCardIndex = i;
		}
	}
	if (BestCardIndex >= 0)
	{
		return GameState.Hand[BestCardIndex].Name;
	}
	return FName();
}

FName UAlphaBetaPruningComponent::ChooseBestCardInField(const FGameStateSim& GameState, int32 Depth)
{
	int32 BestCardIndex = INDEX_NONE;
	int32 BestValue = INT32_MIN;

	for (int32 i = 0; i < GameState.CardField.Num() - 1; ++i)
	{
		FGameStateSim SimulatedState = GameState;

		// Simulate playing the card
		SimulatePlayCard(SimulatedState, i, true);

		// Perform Alpha-Beta Pruning on the simulated state
		int32 Value = AlphaBetaPruning(SimulatedState, Depth, INT32_MIN, INT32_MAX, false, true);

		if (Value > BestValue)
		{
			BestValue = Value;
			BestCardIndex = i;
		}
	}

	if (BestCardIndex >= 0)
	{
		return GameState.CardField[BestCardIndex].Name;
	}
	return FName();
	
}

void UAlphaBetaPruningComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UAlphaBetaPruningComponent::AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta,
	bool bIsMaximizingPlayer, bool bIsField)
{
	if (Depth == 0)
	{
		return EvaluateBoardState(GameState);
	}

	if (bIsMaximizingPlayer)
	{
		int32 MaxEval = INT32_MIN;

		for (int32 i = 0; i < GameState.Hand.Num() - 1; ++i)
		{
			FGameStateSim SimulatedState = GameState;

			// Simulate playing the card
			SimulatePlayCard(SimulatedState, i, bIsField);

			int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, false, bIsField);
			MaxEval = FMath::Max(MaxEval, Eval);
			Alpha = FMath::Max(Alpha, Eval);

			if (Beta <= Alpha)
			{
				break;
			}
		}

		return MaxEval;
	}
	int32 MinEval = INT32_MAX;
	GeneratePossibleHands(GameState);
	// Assuming opponent plays optimally; simulate opponent's best response
	for (int32 i = 0; i < GameState.OpponentCardHand.Num() - 1; ++i)
	{
		FGameStateSim SimulatedState = GameState;

		// Simulate opponent playing the card
		SimulateOpponentPlayCard(SimulatedState, i, bIsField);

		int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, true, bIsField);
		MinEval = FMath::Min(MinEval, Eval);
		Beta = FMath::Min(Beta, Eval);

		if (Beta <= Alpha)
		{
			break;
		}
	}

	return MinEval;
}

void UAlphaBetaPruningComponent::GeneratePossibleHands(FGameStateSim& GameState)
{

	// Empty out Hand and Deck to Help you repopulate them
	GameState.OpponentCardDeck.Empty();
	// Add the 40 Cards using the GeneralDeck like when initialized
	GameState.OpponentCardDeck.Append(GameState.GeneralDeck);

	// Add all known cards (field, and any discarded cards) to KnownCards array
	TArray<FTbfCardInfoSim> KnownCards;
	KnownCards.Append(GameState.OpponentCardField);
	KnownCards.Append(GameState.OpponentDiscardedCards);
    
	// Remove all known cards from AllCards to get the unknown cards
	for (FTbfCardInfoSim& KnownCard : KnownCards)
	{
		GameState.OpponentCardDeck.RemoveAll([&](const FTbfCardInfoSim& CardInDeck) {
			return CardInDeck.Name == KnownCard.Name;
		});
	}
		
	GameState.OpponentCardHand = GameState.OpponentCardDeck;
}

void UAlphaBetaPruningComponent::SimulatePlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField)
{
	// Retrieve the card from the hand
	FTbfCardInfoSim& CardToPlay = GameState.Hand[CardIndex];
	switch (CardToPlay.Type)
	{
	case ECardType::Unit:
		{
			// Add the unit to the field
			if (!bIsField)
			{
				GameState.UnitField.Add(CardToPlay.Unit);
			}
			else
			{
				CardToPlay.Unit.Attack += 400;
				CardToPlay.Unit.Defence += 400;
			}
			// Simulate Attack if Opponent UnitField has unit
			if (GameState.OpponentUnitField.Num() > 0)
			{
				for (FTbfUnitInfoSim& Unit : GameState.OpponentUnitField)
				{
					float NewDefence = Unit.Defence - CardToPlay.Unit.Attack;
					Unit.Defence += NewDefence;
					// Calculate the actual damage dealt (the difference between the original defense and the new defense)
					float DamageDealt = Unit.Defence - NewDefence;
					
					// Update the attacker's attack value by reducing it by the amount of damage dealt
					CardToPlay.Unit.Attack -= DamageDealt;
				}
			}
			else
			{
				GameState.OpponentLifePoints -= CardToPlay.Unit.Attack;
			}
			break;
		}
	case ECardType::Spell:
		{
			// Apply spell effects (modify attack, defense, etc.)
			if (!bIsField)
			{
				GameState.CardField.Add(CardToPlay);
			}
			if (GameState.UnitField.Num() > 0)
			{
				// reduce the chances of selecting this card since it has no unit to boost
				CardToPlay.Unit.Attack -= 500;
				CardToPlay.Unit.Defence -= 500;
			}else
			{
				// increase the chances of selecting this card since it has one or more unit to boost
				CardToPlay.Unit.Attack += 1000;
				CardToPlay.Unit.Defence += 1000;
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
			}
			
			break;
		}
	case ECardType::Trap:
		{
			// Set the trap on the field
			if (!bIsField)
			{
				CardToPlay.Unit.Attack += 500;
				CardToPlay.Unit.Defence += 500;
				GameState.CardField.Add(CardToPlay);
			}
			if (GameState.UnitField.Num() > 0)
			{
				// reduce the chances of selecting this card since it has no unit to boost
				CardToPlay.Unit.Attack -= 500;
				CardToPlay.Unit.Defence -= 500;
			}else
			{
				// increase the chances of selecting this card since it has one or more unit to boost
				CardToPlay.Unit.Attack += 1000;
				CardToPlay.Unit.Defence += 1000;
				if (CardToPlay.ModifierParam == EModifierParam::Attack)
				{
					for (FTbfUnitInfoSim& Unit : GameState.OpponentUnitField)
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
					for (FTbfUnitInfoSim& Unit : GameState.OpponentUnitField)
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
				
			}
			break;
		}
	default:
		break;
	}

	// Remove the card from the AI's hand after it's been played
	GameState.Hand.RemoveAt(CardIndex);
}

void UAlphaBetaPruningComponent::ApplySpellOrTrapEffect(FGameStateSim& GameState, const FTbfCardInfoSim& CardToPlay, bool bIsOpponent)
{
	GameState.CardField.Add(CardToPlay);
	TArray<FTbfUnitInfoSim> Field = bIsOpponent ? GameState.OpponentUnitField : GameState.UnitField;
	if (CardToPlay.ModifierParam == EModifierParam::Attack)
	{
		for (FTbfUnitInfoSim& Unit : Field)
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
		for (FTbfUnitInfoSim& Unit : Field)
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

	if (bIsOpponent)
	{
		GameState.OpponentUnitField = Field;
		return;
	}
	GameState.UnitField = Field;
}

void UAlphaBetaPruningComponent::SimulateOpponentPlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField)
{
	// Retrieve the card from the hand
	FTbfCardInfoSim& CardToPlay = GameState.OpponentCardHand[CardIndex];
	GameState.OpponentDiscardedCards.Add(CardToPlay);

	switch (CardToPlay.Type)
	{
	case ECardType::Unit:
		{
			// Add the unit to the field
			if (!bIsField)
			{
				CardToPlay.Unit.Attack += 500;
				CardToPlay.Unit.Defence += 500;
				GameState.CardField.Add(CardToPlay);
			}
			if (!bIsField)
			{
				GameState.OpponentUnitField.Add(CardToPlay.Unit);
			}
			else
			{
				CardToPlay.Unit.Attack += 400;
				CardToPlay.Unit.Defence += 400;
			}
			// Simulate Attack if UnitField has unit
			if (GameState.UnitField.Num() > 0)
			{
				for (FTbfUnitInfoSim& Unit : GameState.UnitField)
				{
					float NewDefence = Unit.Defence - CardToPlay.Unit.Attack;
					Unit.Defence += NewDefence;
					// Calculate the actual damage dealt (the difference between the original defense and the new defense)
					float DamageDealt = Unit.Defence - NewDefence;
					// Update the attacker's attack value by reducing it by the amount of damage dealt
					CardToPlay.Unit.Attack = CardToPlay.Unit.Attack - DamageDealt;
				}
			}
			else
			{
				GameState.LifePoints -= CardToPlay.Unit.Attack;
			}
			break;
		}
	case ECardType::Spell:
		{
			// Apply spell effects (modify attack, defense, etc.)
			if (bIsField)
			{
				CardToPlay.Unit.Attack += 500;
				CardToPlay.Unit.Defence += 500;
			}
			GameState.OpponentCardField.Add(CardToPlay);
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
			// Set the trap on the field and Apply
			if (!bIsField)
			{
				CardToPlay.Unit.Attack += 500;
				CardToPlay.Unit.Defence += 500;
			}
			GameState.OpponentCardField.Add(CardToPlay);
			if (CardToPlay.ModifierParam == EModifierParam::Attack)
			{
				for (FTbfUnitInfoSim& Unit : GameState.OpponentUnitField)
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
				for (FTbfUnitInfoSim& Unit : GameState.OpponentUnitField)
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
	default:
		break;
	}

	// Remove the card from the AI's hand after it's been played
	GameState.OpponentCardHand.RemoveAt(CardIndex);
}

int32 UAlphaBetaPruningComponent::EvaluateBoardState(const FGameStateSim& GameState) const
{
	// board evaluation:
	int32 Value = 0;

	// Add value based on life points
	Value += GameState.LifePoints - GameState.OpponentLifePoints;
	
	// Add value based on the number and strength of cards on the field
	for (const FTbfCardInfoSim& CardInfo : GameState.CardField)
	{
		Value += CardInfo.Unit.Attack + CardInfo.Unit.Defence;
		if (CardInfo.ModifierType == EModifierType::Add)
		{
			Value += CardInfo.ModifierValue;
		}
		else if (CardInfo.ModifierType == EModifierType::Multiply)
		{
			Value *= CardInfo.ModifierValue;
		}
	}
	// Add value based on the number and strength of units on the field
	for (const FTbfUnitInfoSim& Unit : GameState.UnitField)
	{
		Value += Unit.Attack + Unit.Defence;
	}

	// Subtract value based on opponent's units on the field
	for (const FTbfUnitInfoSim& OpponentUnit : GameState.OpponentUnitField)
	{
		Value -= OpponentUnit.Attack + OpponentUnit.Defence;
	}
	// Subtract value based on opponent's cards on the field
	for (const FTbfCardInfoSim& CardInfo : GameState.OpponentCardField)
	{
		Value -= CardInfo.Unit.Attack + CardInfo.Unit.Defence;
		if (CardInfo.ModifierType == EModifierType::Add)
		{
			Value -= CardInfo.ModifierValue;
		}
		else if (CardInfo.ModifierType == EModifierType::Multiply)
		{
			Value *= CardInfo.ModifierValue;
		}
	}

	return Value;
}
