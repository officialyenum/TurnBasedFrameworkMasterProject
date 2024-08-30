// Copyright Chukwuyenum Opone @officialyenum

#include "AI/Algo/AlphaBetaPruningComponent.h"

#include "Algo/RandomShuffle.h"
#include "Character/TbfCharacterAI.h"

// Sets default values for this component's properties
UAlphaBetaPruningComponent::UAlphaBetaPruningComponent()
{
    // Set this component to be initialized when the game starts, and to not be ticked every frame.
    PrimaryComponentTick.bCanEverTick = false;
    BestCardIndex = -1;
}

void UAlphaBetaPruningComponent::BeginPlay()
{
    Super::BeginPlay();
}

/**
 * Chooses the best card to play using the Alpha-Beta Pruning algorithm.
 */
FName UAlphaBetaPruningComponent::ChooseBestCard(const FGameStateSim& GameState, int32 Depth, bool bIsField)
{
    // Get the AI owner of this component
    ATbfCharacterAI* OwnerCharacter = Cast<ATbfCharacterAI>(GetOwner());
    OwnerCharacter->UpdateGameState();  // Ensure the game state is up-to-date

    // Start the Alpha-Beta Pruning process
    FGameStateSim SimulatedStateSim = GameState;
    // Generate possible hands for the current game state
    GeneratePossibleHands(SimulatedStateSim);
    int32 Score = AlphaBetaPruning(SimulatedStateSim, Depth, INT32_MIN, INT32_MAX, true, bIsField);
    // Retrieve the card array based on whether it's from the field or hand
    const TArray<FTbfCardInfoSim>& Cards = bIsField ? GameState.CardField : GameState.Hand;
    // Return the best card's name if a valid index was found
    if (BestCardIndex >= 0 && BestCardIndex < Cards.Num())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%s Returned as Best Card"), *Cards[BestCardIndex].Name.ToString()));
        return Cards[BestCardIndex].Name;
    }
    return FName();
}

/**
 * Performs the Alpha-Beta Pruning algorithm to evaluate game states.
 */
int32 UAlphaBetaPruningComponent::AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer, bool bIsField)
{
    // Base case: If depth is 0, return the evaluated score of the board
    if (Depth == 0)
    {
        return EvaluateBoardState(GameState);
    }
    // Determine the number of cards based on whether it's the player's or opponent's turn
    int32 NoOfCards = bIsField ? (bIsMaximizingPlayer ? GameState.CardField.Num() : GameState.OpponentCardField.Num())
                                : (bIsMaximizingPlayer ? GameState.Hand.Num() : GameState.OpponentCardHand.Num());
    if (bIsMaximizingPlayer)
    {
        // Maximizing player's turn (AI)
        int32 MaxEval = INT32_MIN;
        for (int32 i = 0; i < NoOfCards; ++i)
        {
            // Simulate the game state after playing the card
            FGameStateSim SimulatedState = GameState;
            SimulatePlay(SimulatedState, i, bIsField, true);
            // Recursively call Alpha-Beta Pruning for the opponent's turn
            int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, false, bIsField);
            if (Eval > MaxEval)
            {
                MaxEval = Eval;
                BestCardIndex = i;  // Only update BestCardIndex when bIsMaximizingPlayer is true
            }
            Alpha = FMath::Max(Alpha, Eval);
            // Beta cutoff
            if (Beta <= Alpha)
            {
                break;
            }
        }
        return MaxEval;
    }
    else
    {
        // Minimizing player's turn (Opponent)
        int32 MinEval = INT32_MAX;
        for (int32 i = 0; i < NoOfCards; ++i)
        {
            // Simulate the game state after playing the card
            FGameStateSim SimulatedState = GameState;
            SimulatePlay(SimulatedState, i, bIsField, false);
            // Recursively call Alpha-Beta Pruning for the AI's turn
            int32 Eval = AlphaBetaPruning(SimulatedState, Depth - 1, Alpha, Beta, true, bIsField);
            // Update the minimum evaluation score and beta value
            MinEval = FMath::Min(MinEval, Eval);
            Beta = FMath::Min(Beta, Eval);
            // Alpha cutoff
            if (Beta <= Alpha)
            {
                break;
            }
        }
        return MinEval;
    }
}

/**
 * Generates possible hands for the opponent by simulating their deck.
 */
void UAlphaBetaPruningComponent::GeneratePossibleHands(FGameStateSim& GameState)
{
    // Create a list of known cards (field and discarded)
    TArray<FTbfCardInfoSim> KnownCards;
    KnownCards.Append(GameState.OpponentCardField);
    KnownCards.Append(GameState.OpponentCardHand); // Include current hand cards as well

    // Remove known cards from the opponent's deck
    for (const FTbfCardInfoSim& KnownCard : KnownCards)
    {
        RemoveFromCardArray(GameState.OpponentCardDeck, KnownCard);
    }

    // Shuffle the remaining deck using Algo::Shuffle
    Algo::RandomShuffle(GameState.OpponentCardDeck);

    // Generate possible hand with 5 to 7 cards
    int32 NumCardsToDraw = FMath::RandRange(5, 7);
    NumCardsToDraw = FMath::Min(NumCardsToDraw, GameState.OpponentCardDeck.Num());

    GameState.OpponentCardHand.Empty(); // Clear existing hand
    for (int32 i = 0; i < NumCardsToDraw; i++)
    {
        GameState.OpponentCardHand.Add(GameState.OpponentCardDeck[i]);
    }
}

/**
 * Simulates playing a card, updating the game state accordingly.
 */
void UAlphaBetaPruningComponent::SimulatePlay(FGameStateSim& GameState, int32 CardIndex, bool bIsField, bool bIsPlayer)
{
    // Select the card to play based on whether it's from the field or hand
    FTbfCardInfoSim& CardToPlay = bIsField ? (bIsPlayer ? GameState.CardField[CardIndex] : GameState.OpponentCardField[CardIndex])
                                           : (bIsPlayer ? GameState.Hand[CardIndex] : GameState.OpponentCardHand[CardIndex]);
    // Handle the card based on its type
    switch (CardToPlay.Type)
    {
    case ECardType::Unit:
        // Update the rank based on unit attributes
        CardToPlay.Rank += CardToPlay.Unit.Attack + CardToPlay.Unit.Defence + 5000;
        // Move the card from hand to field if necessary
        if (!bIsField)
        {
            if (bIsPlayer)
            {
                GameState.CardField.Add(CardToPlay);
                GameState.Hand.RemoveAt(CardIndex);
            }
            else
            {
                GameState.OpponentCardField.Add(CardToPlay);
                GameState.OpponentCardHand.RemoveAt(CardIndex);
            }
        }
        // Simulate combat with the opponent's units
        SimulateCombat(GameState, CardToPlay, bIsPlayer);
        break;
    case ECardType::Spell:
    case ECardType::Trap:
        // Apply the card's effects
        ApplyCardEffects(GameState, CardToPlay, CardToPlay.Type == ECardType::Trap, bIsPlayer);
        // Move the card from hand to field if necessary
        if (!bIsField)
        {
            if (bIsPlayer)
            {
                GameState.CardField.Add(CardToPlay);
                GameState.Hand.RemoveAt(CardIndex);
            }
            else
            {
                GameState.OpponentCardField.Add(CardToPlay);
                GameState.OpponentCardHand.RemoveAt(CardIndex);
            }
        }
        break;

    default:
        break;
    }
}



/**
 * Applies effects of a card to the game state.
 */
void UAlphaBetaPruningComponent::ApplyCardEffects(FGameStateSim& GameState, FTbfCardInfoSim& Card, bool bIsTrap, bool bIsPlayer)
{
    // Check if the card modifies attack or defense
    if (Card.ModifierParam == EModifierParam::Attack || Card.ModifierParam == EModifierParam::Defence)
    {
        // Determine the target field based on card type and player
        TArray<FTbfCardInfoSim>& TargetField = bIsPlayer ? (bIsTrap ? GameState.OpponentCardField : GameState.CardField)
                                                         : (bIsTrap ? GameState.CardField : GameState.OpponentCardField);

        if (TargetField.Num() > 0)
        {
            // Apply the effect to each card in the target field
            for (FTbfCardInfoSim& TargetCard : TargetField)
            {
                float* AttributeToModify = (Card.ModifierParam == EModifierParam::Attack)
                                                ? &TargetCard.Unit.Attack
                                                : &TargetCard.Unit.Defence;
                *AttributeToModify = Card.ModifierValue * (Card.ModifierType == EModifierType::Add ? *AttributeToModify + Card.ModifierValue : *AttributeToModify * Card.ModifierValue);
                // increase the rank by 500 of the modified attribute
                TargetCard.Rank += 5;
            }
        }
        else
        {
            //reduce card by 20% since it has no units it can affect
            Card.Rank *= 0.8;
        }
    }
    // Additional effects can be added here
}

/**
 * Simulates combat between the played card and the opponent's units.
 */
void UAlphaBetaPruningComponent::SimulateCombat(FGameStateSim& GameState, FTbfCardInfoSim& PlayedCard, bool bIsPlayer)
{
    // Define the fields for both players
    TArray<FTbfCardInfoSim>& PlayerField = bIsPlayer ? GameState.CardField : GameState.OpponentCardField;
    TArray<FTbfCardInfoSim>& OpponentField = bIsPlayer ? GameState.OpponentCardField : GameState.CardField;

    // Iterate over the opponent's field to resolve combat
    if (OpponentField.Num() <= 0)
    {
        PlayedCard.Rank += 1000;
        if (bIsPlayer)
        {
            GameState.OpponentLifePoints -= 500;
        }
        else
        {
            GameState.LifePoints -= 500;
        }
    }
    for (int32 i = 0; i < OpponentField.Num(); ++i)
    {
        FTbfCardInfoSim& OpponentCard = OpponentField[i];
        // Combat resolution based on attack values
        if (PlayedCard.Unit.Attack > OpponentCard.Unit.Attack)
        {
            OpponentCard.bIsDead = true;
            PlayedCard.Rank += 500;  // Increase rank after successful combat
        }
        else if (PlayedCard.Unit.Attack < OpponentCard.Unit.Attack)
        {
            PlayedCard.bIsDead = true;
            //RemoveFromCardArray(PlayerField,PlayedCard);
            break;  // Stop further combat if the played card is destroyed
        }
    }
}

/**
 * Removes a specified card from the card array.
 */
void UAlphaBetaPruningComponent::RemoveFromCardArray(TArray<FTbfCardInfoSim>& CardArray, const FTbfCardInfoSim& CardToRemove)
{
    CardArray.RemoveAll([&CardToRemove](const FTbfCardInfoSim& Card)
    {
        return Card.Name == CardToRemove.Name;
    });
}

/**
 * Evaluates the current board state to assign a score.
 */
int32 UAlphaBetaPruningComponent::EvaluateBoardState(const FGameStateSim& GameState) const
{
    int32 Score = 0;
    // Use Both AI and Player Life Points after simulation for evaluation
    Score += GameState.LifePoints;
    Score -= GameState.OpponentLifePoints;
    // Calculate the score based on the AI's selected card impact on the fields
    for (const FTbfCardInfoSim& Card : GameState.CardField)
    {
        Score += Card.Rank;
    }
    for (const FTbfCardInfoSim& Card : GameState.OpponentCardField)
    {
        Score -= Card.Rank;
    }

    return Score;
}
