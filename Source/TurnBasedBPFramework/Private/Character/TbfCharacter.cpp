// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacter.h"

#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "Components/ArrowComponent.h"
#include "Game/TbfGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Library/TbfCardFunctionLibrary.h"
#include "Library/TbfGameFunctionLibrary.h"


// Sets default values
ATbfCharacter::ATbfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CardSpawnDirectionArea = CreateDefaultSubobject<UArrowComponent>("CardSpawnDirectionArea");
	CardSpawnDirectionArea->SetRelativeLocation(FVector(50.0f,250.0f,100.0f));
	CardSpawnDirectionArea->SetRelativeRotation(FRotator(320.0f,-90.0f,0.0f));
	CardSpawnDirectionArea->SetArrowSize(1.0f);
	CardSpawnDirectionArea->SetArrowLength(150.0f);
	CardSpawnDirectionArea->SetHiddenInGame(false);
	CardSpawnDirectionArea->SetVisibility(true);
	CardSpawnDirectionArea->SetupAttachment(GetMesh());
}

void ATbfCharacter::DrawCard()
{
	if (Deck > 0 && DrawCountPerTurn > 0)
	{
		// Get Card Datatable List
		if (!DT)
		{
			ConstructorHelpers::FObjectFinder<UDataTable> CardDataTable_BP(TEXT("/Game/FrameWork/Blueprint/Data/DT_CardDeckList"));
			if (CardDataTable_BP.Succeeded())
			{
				DT = CardDataTable_BP.Object;
			}
		}

		FTbfCardInfo CardInfoStruct = UTbfCardFunctionLibrary::GetRandomCardFromDataTable(DT);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Found Card Name %s"), *CardInfoStruct.Name.ToString()));
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
			static ConstructorHelpers::FClassFinder<ACardBase> CardBaseBPClass(TEXT("Blueprint'/Game/rameworkV2/Blueprints/Actors/Card/BP_TbfCard.BP_TbfCard'"));
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
		Deck--;
		DrawCountPerTurn--;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Cards Left in Deck"), Deck));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Draw Left for this turn"), DrawCountPerTurn));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Draw Left Switch to Main State to Proceed"), DrawCountPerTurn));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Cards Left in Deck, Switch to Main State to Proceed"), Deck));
	}
	UpdateUIStat();
	UE_LOG(LogTemp, Error, TEXT("No Deck"));
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
		}
		GoToNextState();
	}
	UpdateUIStat();
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

void ATbfCharacter::GoToNextState()
{
	switch (CurrentState)
	{
		case EPlayerState::Waiting:
			CurrentState = EPlayerState::Draw;// Show Message
			break;
		case EPlayerState::Draw:
			CurrentState = EPlayerState::MainOne;
			break;
		case EPlayerState::MainOne:
			CurrentState = EPlayerState::Battle;
			break;
		case EPlayerState::Battle:
			CurrentState = EPlayerState::MainTwo;
			break;
		case EPlayerState::MainTwo:
			CurrentState = EPlayerState::Waiting;
			break;
	}
	FText Message = FText::Format(
		FText::FromString("{0} {1} State"),
		FText::FromName(Name),
		FText::FromString(UTbfGameFunctionLibrary::PlayerStateToString(CurrentState)));
	ShowMessageInUI(Message);
}



// Called when the game starts or when spawned
void ATbfCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateUIStat();
	FText Message = FText::Format(
		FText::FromString("{0} {1} State"),
		FText::FromName(Name),
		FText::FromString(UTbfGameFunctionLibrary::PlayerStateToString(CurrentState)));
	ShowMessageInUI(Message);
}



