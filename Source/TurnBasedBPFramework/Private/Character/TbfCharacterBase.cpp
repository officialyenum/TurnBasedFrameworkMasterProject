// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterBase.h"

#include "Actor/CardBase.h"


ATbfCharacterBase::ATbfCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATbfCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATbfCharacterBase::DrawCard()
{
	if (Deck.Num() > 0)
	{
		ACardBase* DrawnCard = Deck.Pop();
		DrawnCard->AddCardToHand(this);
		// Play Animation to move card to hand
		Hand.Add(DrawnCard);
	}
}

void ATbfCharacterBase::PlayCard(int32 CardIndex)
{
	if (Hand.IsValidIndex(CardIndex))
	{
		ACardBase* CardToPlay = Hand[CardIndex];
		Hand.RemoveAt(CardIndex);
		//Move Card to Cell Area
		CardOnField.Add(CardToPlay);
	}
}

