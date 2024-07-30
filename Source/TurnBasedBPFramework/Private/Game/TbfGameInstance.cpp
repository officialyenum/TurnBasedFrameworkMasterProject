// Copyright Chukwuyenum Opone @officialyenum


#include "Game/TbfGameInstance.h"

#include "Actor/CardBase.h"
#include "Character/TbfCharacterBase.h"

UTbfGameInstance::UTbfGameInstance()
{
	bIsPlayerOneTurn = true; // Initialize the turn
	PlayerOne = nullptr;
	PlayerTwo = nullptr;
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
