// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/AlphaBetaPruningComponent.h"
#include "Character/TbfCharacterAI.h"

// Sets default values for this component's properties
UAlphaBetaPruningComponent::UAlphaBetaPruningComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.
    PrimaryComponentTick.bCanEverTick = false;
}

void UAlphaBetaPruningComponent::BeginPlay()
{
    Super::BeginPlay();
}

FName UAlphaBetaPruningComponent::ChooseBestCard(const FGameStateSim& GameState, int32 Depth, bool bIsField)
{
    int32 BestCardIndex = INDEX_NONE;
    int32 BestValue = INT32_MIN;
    ATbfCharacterAI* OwnerCharacter = Cast<ATbfCharacterAI>(GetOwner());
    OwnerCharacter->UpdateGameState();
    const TArray<FTbfCardInfoSim>& Cards = bIsField ? GameState.CardField : GameState.Hand;
    for (int32 i = 0; i < Cards.Num(); ++i)
    {
        
        FGameStateSim SimulatedState = GameState;

        // Simulate playing the card
        SimulatePlayCard(SimulatedState, i, bIsField);

        // Perform Alpha-Beta Pruning on the simulated state
        int32 Value = AlphaBetaPruning(SimulatedState, Depth, INT32_MIN, INT32_MAX, false, bIsField);

        if (Value > BestValue)
        {
            BestValue = Value;
            BestCardIndex = i;
        }
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,FString::Printf(TEXT("%s Generated a Value of %i"), *Cards[i].Name.ToString(), Value));
        
    }
    if (BestCardIndex >= 0)
    {
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("%s Returned Best Value of %i"), *Cards[BestCardIndex].Name.ToString(), BestValue));
        return Cards[BestCardIndex].Name;
    }
    return FName();
}

int32 UAlphaBetaPruningComponent::AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta,
    bool bIsMaximizingPlayer, bool bIsField)
{
    if (Depth == 0)
    {
        return EvaluateBoardState(GameState);
    }
    GeneratePossibleHands(GameState);
    int32 NoOfCards = bIsField ? (bIsMaximizingPlayer ? GameState.CardField.Num() : GameState.OpponentCardField.Num()) : 
                                  (bIsMaximizingPlayer ? GameState.Hand.Num() : GameState.OpponentCardHand.Num());

    if (bIsMaximizingPlayer)
    {
        int32 MaxEval = INT32_MIN;
        for (int32 i = 0; i < NoOfCards; ++i)
        {
            FGameStateSim SimulatedState = GameState;
            SimulatePlayCard(SimulatedState, i, bIsField);

            int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, false, bIsField);
            MaxEval = FMath::Max(MaxEval, Eval);
            Alpha = FMath::Max(Alpha, Eval);

            if (Beta <= Alpha)
            {
                break; // Beta cutoff
            }
        }
        return MaxEval;
    }
    else
    {
        int32 MinEval = INT32_MAX;

        for (int32 i = 0; i < NoOfCards; ++i)
        {
            FGameStateSim SimulatedState = GameState;
            SimulateOpponentPlayCard(SimulatedState, i, bIsField);

            int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, true, bIsField);
            MinEval = FMath::Min(MinEval, Eval);
            Beta = FMath::Min(Beta, Eval);

            if (Beta <= Alpha)
            {
                break; // Alpha cutoff
            }
        }
        return MinEval;
    }
}

void UAlphaBetaPruningComponent::GeneratePossibleHands(FGameStateSim& GameState)
{
    if(GameState.OpponentCardDeck.IsEmpty())
    {
        GameState.OpponentCardDeck = GameState.GeneralDeck;
    }

    TArray<FTbfCardInfoSim> KnownCards;
    KnownCards.Append(GameState.OpponentCardField);
    KnownCards.Append(GameState.OpponentDiscardedCards);

    for (FTbfCardInfoSim& KnownCard : KnownCards)
    {
        RemoveFromCardArray(GameState.OpponentCardDeck, KnownCard);
    }
    // Sort the remaining cards in the deck by Rank in descending order
    GameState.OpponentCardDeck.Sort([](const FTbfCardInfoSim& A, const FTbfCardInfoSim& B)
    {
        return A.Rank > B.Rank;
    });

    // Take the top 5 cards (or fewer if there are less than 5 cards)
    GameState.OpponentCardHand = GameState.OpponentCardDeck;
    if (GameState.OpponentCardHand.Num() > 5)
    {
        GameState.OpponentCardHand.SetNum(5);
    }
    // Remove from Deck
    for (FTbfCardInfoSim& HandCard : GameState.OpponentCardHand)
    {
        RemoveFromCardArray(GameState.OpponentCardDeck, HandCard);
    }
    
}

void UAlphaBetaPruningComponent::SimulatePlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField)
{
    FTbfCardInfoSim& CardToPlay = bIsField ? GameState.CardField[CardIndex] : GameState.Hand[CardIndex];

    switch (CardToPlay.Type)
    {
        case ECardType::Unit:
            CardToPlay.Rank += CardToPlay.Unit.Attack + CardToPlay.Unit.Defence + 5000;
            if (!bIsField)
            {
                GameState.CardField.Add(CardToPlay);
                GameState.Hand.RemoveAt(CardIndex);
            }
            SimulateCombat(GameState, CardToPlay, true);
            break;
        case ECardType::Spell:
            ApplyCardEffects(GameState, CardToPlay, false, true);
            if (!bIsField)
            {
                GameState.CardField.Add(CardToPlay);
                GameState.Hand.RemoveAt(CardIndex);
            }
            break;
        case ECardType::Trap:
            ApplyCardEffects(GameState, CardToPlay, true, true);
            if (!bIsField)
            {
                GameState.CardField.Add(CardToPlay);
                GameState.Hand.RemoveAt(CardIndex);
            }
            break;

        default:
            break;
    }
}

void UAlphaBetaPruningComponent::SimulateOpponentPlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField)
{
    FTbfCardInfoSim& CardToPlay = bIsField ? GameState.OpponentCardField[CardIndex] : GameState.OpponentCardHand[CardIndex];

    switch (CardToPlay.Type)
    {
        case ECardType::Unit:
            CardToPlay.Rank += CardToPlay.Unit.Attack + CardToPlay.Unit.Defence + 5000;
            if (!bIsField)
            {
                GameState.OpponentCardField.Add(CardToPlay);
                GameState.OpponentCardHand.RemoveAt(CardIndex);
            }
            SimulateCombat(GameState, CardToPlay, false);
            break;

        case ECardType::Spell:
            ApplyCardEffects(GameState, CardToPlay, false, false);
            if (!bIsField)
            {
                GameState.OpponentCardField.Add(CardToPlay);
                GameState.OpponentCardHand.RemoveAt(CardIndex);
            }
            break;
        case ECardType::Trap:
            ApplyCardEffects(GameState, CardToPlay, true, false);
            if (!bIsField)
            {
                GameState.OpponentCardField.Add(CardToPlay);
                GameState.OpponentCardHand.RemoveAt(CardIndex);
            }
            else
            {
                GameState.OpponentDiscardedCards.Add(CardToPlay);
                GameState.OpponentCardField.RemoveAt(CardIndex);
            }
            break;
        default:
            break;
    }
}

void UAlphaBetaPruningComponent::ApplyCardEffects(FGameStateSim& GameState, FTbfCardInfoSim& Card, bool bIsTrap, bool bIsPlayer)
{
    if (Card.ModifierParam == EModifierParam::Attack || Card.ModifierParam == EModifierParam::Defence)
    {
        TArray<FTbfCardInfoSim>& TargetField = bIsPlayer ? (bIsTrap ? GameState.OpponentCardField : GameState.CardField) : 
                                      (bIsTrap ? GameState.CardField : GameState.OpponentCardField);
        
        for (FTbfCardInfoSim& TargetCard : TargetField)
        {
            if (TargetCard.Type == ECardType::Unit)
            {
                if (Card.ModifierType == EModifierType::Add)
                {
                    if (Card.ModifierParam == EModifierParam::Attack)
                    {
                        TargetCard.Unit.Attack += Card.ModifierValue;
                    }
                    else
                    {
                        TargetCard.Unit.Defence += Card.ModifierValue;
                    }
                }
                else if (Card.ModifierType == EModifierType::Multiply)
                {
                    if (Card.ModifierParam == EModifierParam::Attack)
                    {
                        TargetCard.Unit.Attack *= Card.ModifierValue;
                    }
                    else
                    {
                        TargetCard.Unit.Defence *= Card.ModifierValue;
                    }
                }
                TargetCard.Rank += Card.ModifierValue * (Card.ModifierType == EModifierType::Multiply ? 500 : 10);
            }
            if (TargetCard.Unit.Attack <= 0 || TargetCard.Unit.Defence == 0)
            {
                RemoveFromCardArray(GameState.OpponentCardDeck, TargetCard);
            }
        }
        Card.Rank += 1000.f;
    }
}

void UAlphaBetaPruningComponent::SimulateCombat(FGameStateSim& GameState, FTbfCardInfoSim& Card, bool bIsPlayer)
{
    TArray<FTbfCardInfoSim>& OpponentField = bIsPlayer ? GameState.OpponentCardField : GameState.CardField;

    if (OpponentField.Num() > 0)
    {
        for (FTbfCardInfoSim& OpponentCard : OpponentField)
        {
            if (OpponentCard.Type == ECardType::Unit && !OpponentCard.bIsDead)
            {
                float NewDefence = OpponentCard.Unit.Defence - Card.Unit.Attack;
                OpponentCard.Unit.Defence = NewDefence;

                if (NewDefence <= 0.f)
                {
                    OpponentCard.bIsDead = true;
                    OpponentCard.Unit.bIsDead = true;
                    OpponentCard.Rank = 0;
                    RemoveFromCardArray(GameState.OpponentCardDeck, OpponentCard);
                }
                else
                {
                    OpponentCard.Rank -= NewDefence;
                }
            }
        }
    }
    else
    {
        if (bIsPlayer)
        {
            GameState.OpponentLifePoints -= Card.Unit.Attack;
        }
        else
        {
            GameState.LifePoints -= Card.Unit.Attack;
        }
    }
}

void UAlphaBetaPruningComponent::RemoveDeadUnitsFromArray(TArray<FTbfCardInfoSim>& UnitField)
{
    UnitField.RemoveAll([](const FTbfCardInfoSim& Card)
    {
        return Card.bIsDead || Card.Unit.bIsDead;
    });
}

int32 UAlphaBetaPruningComponent::EvaluateBoardState(const FGameStateSim& GameState) const
{
    float PlayerScore = GameState.LifePoints;
    float OpponentScore = GameState.OpponentLifePoints;

    for (const FTbfCardInfoSim& Card : GameState.CardField)
    {
        PlayerScore += Card.Rank;
    }
    for (const FTbfCardInfoSim& Card : GameState.Hand)
    {
        PlayerScore += Card.Rank;
    }

    for (const FTbfCardInfoSim& Card : GameState.OpponentCardField)
    {
        OpponentScore += Card.Rank;
    }

    for (const FTbfCardInfoSim& Card : GameState.OpponentCardHand)
    {
        OpponentScore += Card.Rank;
    }

    return PlayerScore - OpponentScore;
}

void UAlphaBetaPruningComponent::RemoveFromCardArray(TArray<FTbfCardInfoSim>& CardArray, const FTbfCardInfoSim& Card)
{
    CardArray.RemoveAll([&](const FTbfCardInfoSim& Element)
    {
        return Element.Name == Card.Name;
    });
}
