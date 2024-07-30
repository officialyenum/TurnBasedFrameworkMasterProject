// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterBase.h"

#include "Actor/CardBase.h"
#include "Kismet/GameplayStatics.h"
#include "Library/TbfCardFunctionLibrary.h"


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
		// Get Card Datatable List
		if (!DT)
		{
			ConstructorHelpers::FObjectFinder<UDataTable> CardDataTable_BP(TEXT("DataTable'/Game/FrameWork/Blueprint/Data/DT_CardDeckList.DT_CardDeckList'"));
			if (CardDataTable_BP.Succeeded())
			{
				DT = CardDataTable_BP.Object;
			}
		}

		FTbfCardInfo CardInfoStruct = UTbfCardFunctionLibrary::GetRandomCardFromDataTable(DT);

		// Define spawn transform
		FTransform SpawnTransform;
		FVector MyLocation = GetActorLocation();
		FVector CardLocation = FVector(MyLocation.X + 100.f, MyLocation.Z + 100.f, MyLocation.Z + 100.f);
		SpawnTransform.SetLocation(CardLocation); // Set appropriate location
		SpawnTransform.SetRotation(FQuat(FRotator::ZeroRotator)); // Set appropriate rotation
		// Spawn the card actor deferred
		// Set default class for the card
		TSubclassOf<ACardBase*> CardClass;
		static ConstructorHelpers::FClassFinder<ACardBase> CardBaseBPClass(TEXT("Blueprint'/Game/FrameWork/Blueprint/Card/BP_CardBase.BP_CardBase'"));
		if (CardBaseBPClass.Succeeded())
		{
			CardClass = CardBaseBPClass.Class;
		}

		if (ACardBase* DrawnCard = GetWorld()->SpawnActorDeferred<ACardBase>(
			CardClass, // Ensure this is set to a valid class
			SpawnTransform,
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		))
		{
			// Set the CardInfo property which is marked ExposeOnSpawn
			DrawnCard->CardInfo = CardInfoStruct;

			// Finish spawning the card actor
			DrawnCard->FinishSpawning(SpawnTransform);

			// Add DrawnCard to Hand Array
			Hand.Add(DrawnCard);

			// Reposition Cards in Hand
			RepositionCardInHand();
		}
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

void ATbfCharacterBase::RepositionCardInHand()
{
}

