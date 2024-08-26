// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterAI.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "AI/TbfAIController.h"
#include "AI/Algo/AlphaBetaPruningComponent.h"
#include "AI/Algo/MonteCarloComponent.h"
#include "Character/TbfCharacter.h"
#include "Character/TbfCharacterPlayer.h"
#include "Game/TbfGameInstance.h"
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
		ConstructorHelpers::FObjectFinder<UDataTable> CardDataTable_BP(TEXT("/Game/FrameworkV2/Data/CardDeckSim/DT_CardDeckSim"));
		if (CardDataTable_BP.Succeeded())
		{
			DeckDTSim = CardDataTable_BP.Object;
		}
	}
	
	
	AlphaBetaPruningComponent = CreateDefaultSubobject<UAlphaBetaPruningComponent>(TEXT("AlphaBetaPruningComponent"));
	MonteCarloComponent = CreateDefaultSubobject<UMonteCarloComponent>(TEXT("MonteCarloComponent"));

}

UBehaviorTree* ATbfCharacterAI::GetBehaviorTree() const
{
	return Tree;
}

// Called when the game starts or when spawned
void ATbfCharacterAI::BeginPlay()
{
	Super::BeginPlay();
	// AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Set AI Player in Game Instance Player Two
	if (UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI->PlayerTwo = this;
		SelectedCardAlgorithm = GI->GetCardAlgorithm();
		SelectedUnitAlgorithm = GI->GetUnitAlgorithm();
		CurrentState = GI->bIsPlayerOneTurn ?  ETbfPlayerState::Waiting : ETbfPlayerState::Draw;
	}
	GenerateAndSpawnStartingCard();
	GameStateSim.Deck = UTbfCardFunctionLibrary::GetDeckSim(DeckDTSim);
	GameStateSim.GeneralDeck = UTbfCardFunctionLibrary::GetDeckSim(DeckDTSim);
	GameStateSim.OpponentCardDeck = UTbfCardFunctionLibrary::GetDeckSim(DeckDTSim);

}


int32 ATbfCharacterAI::ChooseCardInHand() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card In Hand With AlphaBetaPruning"));
	if (Hand.Num() > 0)
	{
		if (SelectedCardAlgorithm == ECardAlgo::AlphaBeta_Random || SelectedCardAlgorithm == ECardAlgo::AlphaBeta_AlphaBeta)
		{
			FName CardName = AlphaBetaPruningComponent->ChooseBestCard(GameStateSim, 3, false);
			for (int i = 0; i < Hand.Num(); ++i)
			{
				if (Hand[i]->CardInfo.Name.Compare(CardName) == 0)
				{
					return i;
				}
			}
		} else
		{
			// Always pick out unit card first for activation
			for (int i = 0; i < Hand.Num(); ++i)
			{
				if(Hand[i]->CardInfo.Type == ECardType::Unit)
				{
					return i;
				}
			}
			int32 RandomCardIndex = FMath::RandRange(0,Hand.Num()-1);
			return RandomCardIndex;
		}
		
		
	}
	return INDEX_NONE;
}

int32 ATbfCharacterAI::ChooseCardOnField() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card On Board AlphaBeta Pruning"));
	// Alpha Beta Pruning heuristic for choosing a card to play
	if (CardOnField.Num() > 0)
	{
		if (SelectedCardAlgorithm == ECardAlgo::Random_AlphaBeta || SelectedCardAlgorithm == ECardAlgo::AlphaBeta_AlphaBeta)
		{
			FName CardName = AlphaBetaPruningComponent->ChooseBestCard(GameStateSim, 3, true);
			for (int i = 0; i < CardOnField.Num() - 1; ++i)
			{
				if (CardOnField[i]->CardInfo.Name.Compare(CardName) == 0)
				{
					return i;
				}
			}
		}
		// Always pick out unit card first for activation
		for (int i = 0; i < CardOnField.Num() - 1; ++i)
		{
			if(CardOnField[i]->CardInfo.Type == ECardType::Unit)
			{
				return i;
			}
		}
		int32 RandomCardIndex = FMath::RandRange(0,CardOnField.Num()-1);
		return RandomCardIndex;
	}
	return INDEX_NONE;
}

ATbfGridCell* ATbfCharacterAI::ChooseCell() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Cell"));
	return UTbfGameFunctionLibrary::GetRandomCellForPlayer(this);
}

void ATbfCharacterAI::UpdateGameState()
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("Updating Game State"));
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI) return;  // Early return if GI is not valid
	ATbfCharacterPlayer* Opponent = Cast<ATbfCharacterPlayer>(GI->PlayerOne);
	if (!Opponent) return;  // Early return if Opponent is not valid
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Updating Game State Opponent Check Passed"));
	
	// Get the player states and attribute sets
	ATbfPlayerState* OpponentPS = Cast<ATbfPlayerState>(Opponent->GetPlayerState());
	UTbfAttributeSet* OpponentAS = OpponentPS ? Cast<UTbfAttributeSet>(OpponentPS->GetAttributeSet()) : nullptr;
	UTbfAttributeSet* AS = Cast<UTbfAttributeSet>(AttributeSet);

	// Validate attribute sets
	if (!AS || !OpponentAS) return;
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Game State Attributes Validated"));
	
	// Save game state variables
	GameStateSim.LifePoints = AS->GetHealth();
	GameStateSim.OpponentLifePoints = OpponentAS->GetHealth();
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Life Points Populated"));
	
	// Clear and populate the player's Deck
	GameStateSim.Deck.Empty();
	for (FTbfCardInfo Element : Deck)
	{
		GameStateSim.Deck.Add(UTbfCardFunctionLibrary::ConvertToCardSim(GameStateSim, Element.Name));;
	}
	// Clear and populate the player's hand
	GameStateSim.Hand.Empty();
	for (auto* Element : Hand)
	{
		GameStateSim.Hand.Add(UTbfCardFunctionLibrary::ConvertToCardSim(GameStateSim, Element->CardInfo.Name));;
	}
	// Clear and populate the player's card field
	GameStateSim.CardField.Empty();
	for (auto* Element : CardOnField)
	{
		GameStateSim.CardField.Add(UTbfCardFunctionLibrary::ConvertToCardSim(GameStateSim, Element->CardInfo.Name));
	}
	GameStateSim.UnitField.Empty();
	for (auto Element : UnitOnField)
	{
		GameStateSim.CardField.Add(UTbfCardFunctionLibrary::ConvertToCardSim(GameStateSim, Element->UnitInfo.Name));
		GameStateSim.UnitField.Add(UTbfCardFunctionLibrary::ConvertToUnitSim(GameStateSim,Element->UnitInfo.Name));
	}
	

	// Clear and populate the opponent's card field
	GameStateSim.OpponentCardField.Empty();
	for (auto* Element : Opponent->CardOnField)
	{
		GameStateSim.OpponentCardField.Add(UTbfCardFunctionLibrary::ConvertToCardSim(GameStateSim, Element->CardInfo.Name));
	}
	GameStateSim.OpponentUnitField.Empty();
	for (auto Element : Opponent->UnitOnField)
	{
		FTbfCardInfoSim Card = UTbfCardFunctionLibrary::ConvertToCardSim(GameStateSim, Element->UnitInfo.Name);
		FTbfUnitInfoSim Unit = UTbfCardFunctionLibrary::ConvertToUnitSim(GameStateSim, Element->UnitInfo.Name);
		Card.bIsActive = true;
		GameStateSim.OpponentCardField.Add(Card);
		GameStateSim.OpponentUnitField.Add(Unit);
	}
	
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Game State Update Ended"));
	
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


