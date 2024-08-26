// Copyright Chukwuyenum Opone @officialyenum


#include "Game/TbfGameInstance.h"

#include "AbilitySystem/TbfAttributeSet.h"
#include "Actor/CardBase.h"
#include "Character/TbfCharacter.h"
#include "Components/AudioComponent.h"
#include "Game/TbfGameMode.h"
#include "Kismet/GameplayStatics.h"

UTbfGameInstance::UTbfGameInstance()
{
	bIsPlayerOneTurn = true; // Initialize the turn
	PlayerOne = nullptr;
	PlayerTwo = nullptr;
}

void UTbfGameInstance::SetPlayerOne(ATbfCharacter* Player)
{
	PlayerOne = Player;
}

void UTbfGameInstance::SetPlayerTwo(ATbfCharacter* Player)
{
	PlayerTwo = Player;
}

void UTbfGameInstance::PlayMusicAction() const
{
	MusicComponent = Cast<UAudioComponent>(UGameplayStatics::SpawnSound2D(this,BgMusic,1,1,0,nullptr,true));
}

void UTbfGameInstance::CheckWinner()
{
	auto CheckAndDeclareWinner = [&](ATbfCharacter* Player, ATbfCharacter* Opponent) {
		if (Player && Opponent)
		{
			UTbfAttributeSet* PlayerAttributeSet = Cast<UTbfAttributeSet>(Player->GetAttributeSet());
			if (PlayerAttributeSet && PlayerAttributeSet->GetHealth() <= 0)
			{
				if (ATbfGameMode* GM = Cast<ATbfGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					WinningMessage = FText::Format(
						FText::FromString(TEXT("{0} Won this round")), 
						FText::FromName(Opponent->Name)
					);
					GM->GameOverEvent();
				}
			}
		}
	};

	// Check if either player has won
	CheckAndDeclareWinner(PlayerOne, PlayerTwo);
	CheckAndDeclareWinner(PlayerTwo, PlayerOne);
}

void UTbfGameInstance::SetCardAlgorithm(ECardAlgo Algo)
{
	CardAlgorithm = Algo;
}

void UTbfGameInstance::SetUnitAlgorithm(EUnitAlgo Algo)
{
	UnitAlgorithm = Algo;
}

TArray<ACardBase*> UTbfGameInstance::PlayerOneFieldedCards()
{
	TArray<ACardBase*> FilteredCard;
	if (!PlayerOne)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("Player One Does not Exist in Instance"));
		return FilteredCard;
	}
	for (ACardBase* Card : PlayerOne->CardOnField)
	{
		if (Card->bOpponentCanSee)
		{
			FilteredCard.Add(Card);
		}
	}
	if (FilteredCard.Num() < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("Player One has no Visible Cards"));
	}
	return FilteredCard;
}

TArray<ACardBase*> UTbfGameInstance::PlayerTwoFieldedCards()
{
	TArray<ACardBase*> FilteredCard;
	if (!PlayerOne)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("Player Two Does not Exist in Instance"));
		return FilteredCard;
	}
	for (ACardBase* Card : PlayerTwo->CardOnField)
	{
		if (Card->bOpponentCanSee)
		{
			FilteredCard.Add(Card);
		}
	}
	if (FilteredCard.Num() < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("Player Two has no Visible Cards"));
	}
	return FilteredCard;
}

TArray<ATbfCharacterUnit*> UTbfGameInstance::PlayerOneFieldedUnits()
{
	return PlayerOne->UnitOnField;
}

TArray<ATbfCharacterUnit*> UTbfGameInstance::PlayerTwoFieldedUnits()
{
	return PlayerTwo->UnitOnField;
}
