// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacter.h"

#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "Components/ArrowComponent.h"
#include "Game/TbfGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Library/TbfCardFunctionLibrary.h"


// Sets default values
ATbfCharacter::ATbfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CardSpawnDirectionArea = CreateDefaultSubobject<UArrowComponent>("CardSpawnDirectionArea");
	CardSpawnDirectionArea->SetRelativeLocation(FVector(190.0f,-200.0f,100.0f));
	CardSpawnDirectionArea->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));
	CardSpawnDirectionArea->SetArrowSize(1.0f);
	CardSpawnDirectionArea->SetArrowLength(80.0f);
}

void ATbfCharacter::DrawCard()
{
	if (Deck.Num() > 0 && DrawCountPerTurn > 0)
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
		FTransform SpawnTransform = CardSpawnDirectionArea->GetComponentTransform();

		// Update the Y position
		FVector NewLocation = SpawnTransform.GetLocation();
		NewLocation.Y += Hand.Num() * CardSpace;
		SpawnTransform.SetLocation(NewLocation);
		// Spawn the card actor deferred
		// Set default class for the card
		if (!CardClass)
		{
			static ConstructorHelpers::FClassFinder<ACardBase> CardBaseBPClass(TEXT("Blueprint'/Game/FrameWork/Blueprint/Card/BP_CardBase.BP_CardBase'"));
			if (CardBaseBPClass.Succeeded())
			{
				CardClass = CardBaseBPClass.Class;
			}
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

void ATbfCharacter::PlaySelectedCard()
{
	if (MoveCountPerTurn > 0)
	{
		if (!SelectedCard)
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Select A Card"));
			return;
		}
		if (!TargetedCell)
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Select A Cell for Card to Move To"));
			return;
		}
		if(Hand.Contains(SelectedCard))
		{
			FVector CellLocation = TargetedCell->SpawnDirectionArrow->GetComponentLocation();
			FRotator CellRotator = TargetedCell->SpawnDirectionArrow->GetComponentRotation();
			SelectedCard->SetActorRotation(CellRotator);
			SelectedCard->MoveCardToLocation(CellLocation);
			Hand.Remove(SelectedCard);
			CardOnField.Add(SelectedCard);
			SelectedCard->bOpponentCanSee = true;
			SelectedCard = nullptr;
		};
		MoveCountPerTurn -= 1;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("You Are Out of Card Moves"));
		if (CurrentState == EPlayerState::MainOne)
		{
			CurrentState = EPlayerState::Battle;
		}
		else
		{
			if (CurrentState == EPlayerState::MainTwo)
			{
				if (bIsPlayer)
				{
					Cast<UTbfGameInstance>(GetGameInstance())->bIsPlayerOneTurn = false;
				}
				else
				{
					Cast<UTbfGameInstance>(GetGameInstance())->bIsPlayerOneTurn = true;
				}
				CurrentState = EPlayerState::Waiting;
			}
		}
	}
}

void ATbfCharacter::RepositionCardInHand()
{
	// Define spawn transform
	FTransform SpawnTransform = CardSpawnDirectionArea->GetComponentTransform();
	for (int i = 0; i < Hand.Num(); ++i)
	{
		ACardBase* Card = Hand[i];
		
		// Update the Y position
		FVector NewLocation = SpawnTransform.GetLocation();
		NewLocation.Y += i * CardSpace;
		Card->MoveCardToLocation(NewLocation);
	}
}


// Called when the game starts or when spawned
void ATbfCharacter::BeginPlay()
{
	Super::BeginPlay();
}



