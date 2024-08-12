// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterAI.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "AI/TbfAIController.h"
#include "AI/Algo/AlphaBetaPruningComponent.h"
#include "AI/Algo/MonteCarloComponent.h"
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
	}
	InitAbilityActorInfo();
}


int32 ATbfCharacterAI::ChooseCardInHand() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card In Hand"));
	// Simple heuristic for choosing a card to play: choose the first card in hand
	if (Hand.Num() > 0)
	{
		// Always pick out unit card first for activation
		for (int i = 0; i < Hand.Num(); ++i)
		{
			if(Hand[i]->CardInfo.Type == ECardType::Unit)
			{
				return i;
			}
		}
		return 0;
	}
	return INDEX_NONE;
}

int32 ATbfCharacterAI::ChooseCardOnField() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Card On Board"));
	// Simple heuristic for choosing a card to play: choose the first card in field index
	if (CardOnField.Num() > 0)
	{
		// Always pick out unit card first for activation
		for (int i = 0; i < CardOnField.Num(); ++i)
		{
			if(CardOnField[i]->CardInfo.Type == ECardType::Unit)
			{
				return i;
			}
		}
		return 0;
	}
	return INDEX_NONE;
}

ATbfGridCell* ATbfCharacterAI::ChooseCell() const
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("AI Choosing a Cell"));
	return UTbfGameFunctionLibrary::GetRandomCellForPlayer(this);
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


