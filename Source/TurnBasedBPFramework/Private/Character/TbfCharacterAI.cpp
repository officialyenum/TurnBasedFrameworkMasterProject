// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterAI.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "AI/TbfAIController.h"
#include "Character/TbfCharacter.h"
#include "Game/TbfGameInstance.h"
#include "Game/TbfGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Library/TbfCardFunctionLibrary.h"
#include "Library/TbfGameFunctionLibrary.h"
#include "Player/TbfPlayerState.h"


// Sets default values
ATbfCharacterAI::ATbfCharacterAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UTbfAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UTbfAttributeSet>("AttributeSet");

	// Set AIControllerClass to ATbfAIController
	AIControllerClass = ATbfAIController::StaticClass();

	if (!DeckDTSim)
	{
		ConstructorHelpers::FObjectFinder<UDataTable> CardDataTable_BP(TEXT("/Game/FrameworkV2/Data/DT_CardDeckSim"));
		if (CardDataTable_BP.Succeeded())
		{
			DeckDTSim = CardDataTable_BP.Object;
			GameStateSim.Deck = UTbfCardFunctionLibrary::GetDeckSim(DeckDTSim);
		}
	}
}

UBehaviorTree* ATbfCharacterAI::GetBehaviorTree() const
{
	return Tree;
}

// Called when the game starts or when spawned
void ATbfCharacterAI::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Set AI Player in Game Instance Player Two
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->PlayerTwo = this;
}

void ATbfCharacterAI::MakeMove()
{
	PerformDrawPhase();
	PerformMainPhase();
	PerformBattlePhase();
	PerformEndPhase();
}

void ATbfCharacterAI::PerformDrawPhase()
{
	DrawCard();
	// Additional logic for the AI's draw phase
}

void ATbfCharacterAI::PerformMainPhase()
{
	int32 CardIndex = ChooseCardInHand();
	if (CardIndex != INDEX_NONE)
	{
		SelectedCard = Hand[CardIndex];
		PlaySelectedCard();
	}
	// Additional logic for the AI's main phase
}

void ATbfCharacterAI::PerformBattlePhase()
{
	int32 CardIndex = ChooseCardToAttack();
	if (CardIndex != INDEX_NONE)
	{
		// Implement attack logic here
	}
	// Additional logic for the AI's battle phase
}

void ATbfCharacterAI::PerformEndPhase()
{
	// Implement logic for the AI's end phase
}

int32 ATbfCharacterAI::ChooseCardInHand() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card In Hand"));
	// Simple heuristic for choosing a card to play: choose the first card in hand
	if (Hand.Num() > 0)
	{
		return 0;
	}
	return INDEX_NONE;
}

int32 ATbfCharacterAI::ChooseCardOnField() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card On Board"));
	// Simple heuristic for choosing a card to play: choose the first card in hand
	if (CardOnField.Num() > 0)
	{
		return 0;
	}
	return INDEX_NONE;
}

ATbfGridCell* ATbfCharacterAI::ChooseCell() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Cell"));
	return UTbfGameFunctionLibrary::GetRandomCellForPlayer(this);
}


int32 ATbfCharacterAI::ChooseCardToAttack()
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card On Field index"));
	// Simple heuristic for choosing a card to attack: choose the first card on the field
	if (CardOnField.Num() > 0)
	{
		//return 0;
		const int32 BestCardIndex = MonteCarloSimulation(100);
		return BestCardIndex;
	}
	return INDEX_NONE;
}

int32 ATbfCharacterAI::AlphaBetaPruning(int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer)
{
	if (Depth == 0)
	{
		return EvaluateBoardState();
	}

	if (bIsMaximizingPlayer)
	{
		int32 MaxEval = INT32_MIN;
		for (int32 i = 0; i < Hand.Num(); ++i)
		{
			// Hypothetical play card from hand
			SelectedCard = Hand[i];
			PlaySelectedCard();
			int32 Eval = AlphaBetaPruning(Depth - 1, Alpha, Beta, false);
			MaxEval = FMath::Max(MaxEval, Eval);
			Alpha = FMath::Max(Alpha, Eval);
			if (Beta <= Alpha)
			{
				break;
			}
			// Undo hypothetical play
			// ...
		}
		return MaxEval;
	}
	else
	{
		int32 MinEval = INT32_MAX;
		for (int32 i = 0; i < Hand.Num(); ++i)
		{
			// Hypothetical opponent plays card
			SelectedCard = Hand[i];
			PlaySelectedCard();
			int32 Eval = AlphaBetaPruning(Depth - 1, Alpha, Beta, true);
			MinEval = FMath::Min(MinEval, Eval);
			Beta = FMath::Min(Beta, Eval);
			if (Beta <= Alpha)
			{
				break;
			}
			// Undo hypothetical play
			// ...
		}
		return MinEval;
	}
}

int32 ATbfCharacterAI::EvaluateBoardState()
{
	//TODO: Implement evaluation function here
	// For example,
	// --calculate the difference in life points,
	// --the number of units on the field,
	// --the number of cards on the field,
	return 0;
}

int32 ATbfCharacterAI::MonteCarloSimulation(int32 Simulations)
{
	TArray<int32> Wins;
	Wins.SetNum(Hand.Num());

	for (int32 i = 0; i < Simulations; ++i)
	{
		for (int32 j = 0; j < Hand.Num(); ++j)
		{
			// Simulate playing card j
			SelectedCard = Hand[i];
			PlaySelectedCard();

			// Simulate the rest of the game randomly
			// ...

			// Determine if this simulation resulted in a win
			bool bWin = false; // Replace with actual win condition check
			if (bWin)
			{
				Wins[j]++;
			}

			// Undo simulation
			// ...
		}
	}

	// Find the card with the highest win rate
	int32 BestCardIndex = 0;
	int32 MaxWins = Wins[0];
	for (int32 i = 1; i < Wins.Num(); ++i)
	{
		if (Wins[i] > MaxWins)
		{
			MaxWins = Wins[i];
			BestCardIndex = i;
		}
	}

	return BestCardIndex;

}

void ATbfCharacterAI::SaveGameState(const UObject* WorldContextObject)
{
	ATbfGameMode* TbfGameMode = Cast<ATbfGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	ATbfCharacter* Opponent = Cast<ATbfCharacter>(TbfGameMode->PlayerOne);
	ATbfPlayerState* OpponentPS = Cast<ATbfPlayerState>(Opponent->GetPlayerState());
	UTbfAttributeSet* OpponentAS =  Cast<UTbfAttributeSet>(OpponentPS->GetAttributeSet());
	UTbfAttributeSet* AS = Cast<UTbfAttributeSet>(AttributeSet);
	InitialGameState.LifePoints = AS->GetHealth();
	InitialGameState.OpponentLifePoints = OpponentAS->GetHealth();
	InitialGameState.Hand = Hand;
	InitialGameState.CardField = CardOnField;
	InitialGameState.OpponentCardField = Opponent->CardOnField;
	InitialGameState.UnitField = UnitOnField;
	InitialGameState.OpponentUnitField = Opponent->UnitOnField;
	
	// Save other game state variables as needed
}

void ATbfCharacterAI::RestoreGameState(const UObject* WorldContextObject)
{
	ATbfGameMode* TbfGameMode = Cast<ATbfGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	ATbfCharacter* Opponent = Cast<ATbfCharacter>(TbfGameMode->PlayerOne);
	ATbfPlayerState* OpponentPS = Cast<ATbfPlayerState>(Opponent->GetPlayerState());
	UTbfAttributeSet* OpponentAS =  Cast<UTbfAttributeSet>(OpponentPS->GetAttributeSet());
	UTbfAttributeSet* AS = Cast<UTbfAttributeSet>(AttributeSet);
	AS->SetHealth(InitialGameState.LifePoints);
	OpponentAS->SetHealth(InitialGameState.OpponentLifePoints);
	Hand = InitialGameState.Hand;
	CardOnField = InitialGameState.CardField;
	Opponent->CardOnField = InitialGameState.OpponentCardField;
	UnitOnField = InitialGameState.UnitField;
	Opponent->UnitOnField = InitialGameState.OpponentUnitField;
	// Restore other game state variables as needed
}

// Simulation Area


void ATbfCharacterAI::PopulateDeck_Sim()
{
}

void ATbfCharacterAI::SelectCardAndCell_Sim()
{
}

void ATbfCharacterAI::MoveCardToCell_Sim()
{
}

void ATbfCharacterAI::ActivateCard_Sim()
{
}

void ATbfCharacterAI::SelectUnitAndTarget_Sim()
{
}

void ATbfCharacterAI::AttackUnitTarget_Sim()
{
}

void ATbfCharacterAI::AttackPlayer_Sim()
{
}

void ATbfCharacterAI::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeDefaultAttributes();
}


