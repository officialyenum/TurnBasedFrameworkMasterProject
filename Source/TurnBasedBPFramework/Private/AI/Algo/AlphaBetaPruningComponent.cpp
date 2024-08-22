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

FName UAlphaBetaPruningComponent::ChooseBestCardInHand(const FGameStateSim& GameState, int32 Depth)
{
	int32 BestCardIndex = INDEX_NONE;
	int32 BestValue = INT32_MIN;

	for (int32 i = 0; i < GameState.Hand.Num() - 1; ++i)
	{
		// Reset State before starting Simulation
		// ATbfCharacterAI* AIChar = Cast<ATbfCharacterAI>(GetOwner());
		// AIChar->UpdateGameState();
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
		// Reset State before starting Simulation
		// ATbfCharacterAI* AIChar = Cast<ATbfCharacterAI>(GetOwner());
		// AIChar->UpdateGameState();
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
		int32 NoOfCards = bIsField ? GameState.CardField.Num() : GameState.Hand.Num();
		for (int32 i = 0; i < NoOfCards - 1; ++i)
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
	
	int32 NoOfCards = bIsField ? GameState.OpponentCardField.Num() : GameState.OpponentCardHand.Num();
	for (int32 i = 0; i < NoOfCards - 1; ++i)
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
		RemoveFromCardArray(GameState.OpponentCardDeck, KnownCard);
	}
		
	GameState.OpponentCardHand = GameState.OpponentCardDeck;
}

void UAlphaBetaPruningComponent::SimulatePlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField)
{
	// Retrieve the card from the hand
	FTbfCardInfoSim& CardToPlay = bIsField ? GameState.CardField[CardIndex] : GameState.Hand[CardIndex];
	switch (CardToPlay.Type)
	{
		case ECardType::Unit:
			{
				// Add the unit to the field
				CardToPlay.Rank += CardToPlay.Unit.Attack + CardToPlay.Unit.Defence + 5000;
				if (!bIsField)
				{
					GameState.CardField.Add(CardToPlay);
					// Remove the card from the AI's hand after it's been played
					GameState.Hand.RemoveAt(CardIndex);
				}
				// Simulate Attack if Opponent UnitField has unit
				if (GameState.OpponentCardField.Num() > 0)
				{
					for (FTbfCardInfoSim& CardInfo : GameState.OpponentCardField)
					{
						float NewDefence = CardInfo.Unit.Defence - CardToPlay.Unit.Attack;
						CardInfo.Unit.Defence = NewDefence;
						// Calculate the actual damage dealt (the difference between the original defense and the new defense)
						float DamageDealt = CardInfo.Unit.Defence - NewDefence;
						
						// Update the attacker's attack value by reducing it by the amount of damage dealt
						CardToPlay.Unit.Attack -= FMath::Clamp(DamageDealt, 0, DamageDealt);
						CardInfo.Rank -= DamageDealt;
					}
				}
				else
				{
					GameState.OpponentLifePoints -= CardToPlay.Unit.Attack;
					CardToPlay.Rank += CardToPlay.Unit.Attack;
				}
				RemoveDeadUnitsFromArray(GameState.OpponentUnitField);
				break;
			}
		case ECardType::Spell:
			{
				// Apply spell effects (modify attack, defense, etc.)
				if (!bIsField)
				{
					GameState.CardField.Add(CardToPlay);
					CardToPlay.Rank += 1000.f;
				}
				// increase the chances of selecting this card since it has one or more unit to boost
				if (CardToPlay.ModifierParam == EModifierParam::Attack)
				{
					for (FTbfCardInfoSim Element : GameState.CardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Attack += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 10.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Attack *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 10.f;
							}
						}
					}
				}
				else if (CardToPlay.ModifierParam == EModifierParam::Defence)
				{
					for (FTbfCardInfoSim Element : GameState.CardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Defence += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue;
								CardToPlay.Rank += 10.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Defence *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 10.f;
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
					CardToPlay.Rank += 500.f;
					GameState.CardField.Add(CardToPlay);
				}
				// increase the chances of selecting this card since it has one or more unit to boost
				if (CardToPlay.ModifierParam == EModifierParam::Attack)
				{
					for (FTbfCardInfoSim Element : GameState.OpponentCardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Attack += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue;
								CardToPlay.Rank += 30.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Attack *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 30.f;
							}
						}
					}
					
				}
				else if (CardToPlay.ModifierParam == EModifierParam::Defence)
				{
					for (FTbfCardInfoSim Element : GameState.OpponentCardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Defence += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue;
								CardToPlay.Rank += 30.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Defence *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 30.f;
							}
						}
					}
				}
			
				break;
			}
		default:
			break;
	}
}

void UAlphaBetaPruningComponent::SimulateOpponentPlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField)
{
	// Retrieve the card from the hand
	FTbfCardInfoSim& CardToPlay = bIsField ? GameState.OpponentCardField[CardIndex] : GameState.OpponentCardHand[CardIndex];
	switch (CardToPlay.Type)
	{
		case ECardType::Unit:
			{
				// Add the unit to the field
				CardToPlay.Rank += CardToPlay.Unit.Attack + CardToPlay.Unit.Defence + 5000;
				if (!bIsField)
				{
					GameState.OpponentCardField.Add(CardToPlay);
					// Remove the card from the AI's hand after it's been played
					GameState.OpponentCardHand.RemoveAt(CardIndex);
				}
				// Simulate Attack if Opponent UnitField has unit
				if (GameState.CardField.Num() > 0)
				{
					for (FTbfCardInfoSim& CardInfo : GameState.CardField)
					{
						float NewDefence = CardInfo.Unit.Defence - CardToPlay.Unit.Attack;
						CardInfo.Unit.Defence = NewDefence;
						// Calculate the actual damage dealt (the difference between the original defense and the new defense)
						float DamageDealt = CardInfo.Unit.Defence - NewDefence;
						
						// Update the attacker's attack value by reducing it by the amount of damage dealt
						CardToPlay.Unit.Attack -= FMath::Clamp(DamageDealt, 0, DamageDealt);
						CardInfo.Rank -= DamageDealt;
					}
				}
				else
				{
					GameState.LifePoints -= CardToPlay.Unit.Attack;
					CardToPlay.Rank += CardToPlay.Unit.Attack;
				}
				RemoveDeadUnitsFromArray(GameState.UnitField);
				break;
			}
		case ECardType::Spell:
			{
				// Apply spell effects (modify attack, defense, etc.)
				if (!bIsField)
				{
					GameState.OpponentCardField.Add(CardToPlay);
					CardToPlay.Rank += 1000.f;
				}
				// increase the chances of selecting this card since it has one or more unit to boost
				if (CardToPlay.ModifierParam == EModifierParam::Attack)
				{
					for (FTbfCardInfoSim Element : GameState.OpponentCardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Attack += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 10.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Attack *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 10.f;
							}
						}
					}
				}
				else if (CardToPlay.ModifierParam == EModifierParam::Defence)
				{
					for (FTbfCardInfoSim Element : GameState.OpponentCardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Defence += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue;
								CardToPlay.Rank += 10.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Defence *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 10.f;
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
					CardToPlay.Rank += 500.f;
					GameState.OpponentCardField.Add(CardToPlay);
				}
				// increase the chances of selecting this card since it has one or more unit to boost
				if (CardToPlay.ModifierParam == EModifierParam::Attack)
				{
					for (FTbfCardInfoSim Element : GameState.CardField	)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Attack += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue;
								CardToPlay.Rank += 30.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Attack *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 30.f;
							}
						}
					}
					
				}
				else if (CardToPlay.ModifierParam == EModifierParam::Defence)
				{
					for (FTbfCardInfoSim Element : GameState.CardField)
					{
						if (Element.Type == ECardType::Unit)
						{
							if (CardToPlay.ModifierType == EModifierType::Add)
							{
								Element.Unit.Defence += CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue;
								CardToPlay.Rank += 30.f;
							}
							else if (CardToPlay.ModifierType == EModifierType::Multiply)
							{
								Element.Unit.Defence *= CardToPlay.ModifierValue;
								Element.Rank += CardToPlay.ModifierValue  > 1 ? 500 : -500;
								CardToPlay.Rank += 30.f;
							}
						}
					}
				}
			
				break;
			}
		default:
			break;
	}
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
		Value += CardInfo.Rank;
	}

	// Add Value Based on Opponents Cards you destroyed and are discarded
	Value += GameState.OpponentDiscardedCards.Num() * 100.f;

	// Subtract value based on opponent's cards on the field
	for (const FTbfCardInfoSim& CardInfo : GameState.OpponentCardField)
	{
		Value -= CardInfo.Rank;
	}
	return Value;
}

void UAlphaBetaPruningComponent::RemoveFromCardArray(TArray<FTbfCardInfoSim>& Cards, const FTbfCardInfoSim& CardToRemove)
{
	Cards.RemoveAll([&](const FTbfCardInfoSim& Card) {
			return Card.Name == CardToRemove.Name;
	});
}

void UAlphaBetaPruningComponent::RemoveDeadUnitsFromArray(TArray<FTbfUnitInfoSim>& UnitInfoSim)
{
	UnitInfoSim.RemoveAll([&](const FTbfUnitInfoSim& Unit) {
		if (Unit.Attack <= 0 || Unit.Defence <= 0)
		{
			return true;
		}
		return false;
	});
}
