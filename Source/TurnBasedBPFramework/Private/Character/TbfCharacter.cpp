// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacter.h"

#include "AbilitySystem/TbfAttributeSet.h"
#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "Components/ArrowComponent.h"
#include "Game/TbfGameInstance.h"
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

	if (!DeckDT)
	{
		ConstructorHelpers::FObjectFinder<UDataTable> CardDataTable_BP(TEXT("/Game/FrameWork/Blueprint/Data/DT_CardDeckList"));
		if (CardDataTable_BP.Succeeded())
		{
			DeckDT = CardDataTable_BP.Object;
			Deck = UTbfCardFunctionLibrary::GetDeck(DeckDT);
		}
	}
}

void ATbfCharacter::DrawCard()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("%s Drawing Card"), *Name.ToString()));
	if (Deck.Num() > 0 && DrawCountPerTurn > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("%s Drawing Card Deck Check Passed"), *Name.ToString()));
		// Get Card Datatable List
		int32 CardIndex = FMath::RandRange(0, Deck.Num() - 1);
		const FTbfCardInfo* CardInfoStruct = &Deck[CardIndex];
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Found Card Name %s"), *CardInfoStruct->Name.ToString()));
        
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
			static ConstructorHelpers::FClassFinder<ACardBase> CardBaseBPClass(TEXT("/Game/FrameworkV2/Blueprints/Actors/BP_TbfCard.BP_TbfCard"));
			if (CardBaseBPClass.Succeeded())
			{
				CardClass = CardBaseBPClass.Class;
			}
		}
		if (ACardBase* DrawnCard = GetWorld()->SpawnActorDeferred<ACardBase>(
			CardClass,
			SpawnTransform,
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		))
		{
			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("%s Spawning Card"), *Name.ToString()));
			// Set the CardInfo property which is marked ExposeOnSpawn
			DrawnCard->CardInfo = *CardInfoStruct;

			// Finish spawning the card actor
			DrawnCard->FinishSpawning(SpawnTransform);

			// Add DrawnCard to Hand Array
			Hand.Add(DrawnCard);

			// Reposition Cards in Hand
			RepositionCardInHand();
		}
		Deck.RemoveAt(CardIndex);
		DrawCountPerTurn--;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Cards Left in Deck"), Deck.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Draw Left for this turn"), DrawCountPerTurn));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Draw Left Switch to Main State to Proceed"), DrawCountPerTurn));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d Cards Left in Deck, Switch to Main State to Proceed"), Deck.Num()));
	}
	UpdateAttributeSet();
	UpdateUIStat();
	if(DrawCountPerTurn <= 0)
	{
		GoToNextState();
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("%s Drawing Card Ends"), *Name.ToString()));
}

void ATbfCharacter::PlaySelectedCard()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
	                                 FString::Printf(TEXT("%s Choosing a Card To Move"), *Name.ToString()));
	if (MoveCountPerTurn > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("%s Move Count Check Passed"), *Name.ToString()));
		if (!SelectedCard)
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Did Not Select Card To Move"), *Name.ToString()));
			return;
		}
		if (!TargetedCell)
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Did Not Select A Cell for Card to Move To"), *Name.ToString()));
			return;
		}
		if(Hand.Contains(SelectedCard))
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,FString::Printf(TEXT("%s Selected Card In Hand Check Passed"), *Name.ToString()));
			FVector CellLocation = TargetedCell->SpawnDirectionArrow->GetComponentLocation();
			FRotator CellRotator = TargetedCell->SpawnDirectionArrow->GetComponentRotation();
			SelectedCard->SetActorRotation(CellRotator);
			SelectedCard->MoveCardToLocation(CellLocation);
			Hand.Remove(SelectedCard);
			CardOnField.Add(SelectedCard);
			// Track Card Above Cell
			SelectedCard->CellOccupied = TargetedCell;
			// Track Cell Below Card
			TargetedCell->OccupyingActor = SelectedCard;
			// Make Card Visible To Opponent
			SelectedCard->bOpponentCanSee = true;
			// Unselect the Card
			SelectedCard->UnSelectActor();
		};
		MoveCountPerTurn -= 1;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Is Out Of Moves"), *Name.ToString()));
		GoToNextState();
	}
	UpdateAttributeSet();
	UpdateUIStat();
	if (MoveCountPerTurn <= 0 && ActivateCountPerTurn <= 0)
	{
		GoToNextState();
	}
}

void ATbfCharacter::PlaySelectedUnitBattle()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
									 FString::Printf(TEXT("%s Choosing a Unit"), *Name.ToString()));
	if (BattleCountPerTurn > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("%s Battle Count Check Passed"), *Name.ToString()));
		if (!SelectedUnit)
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Did Not Select Your Unit"), *Name.ToString()));
			return;
		}
		TArray<ATbfCharacterUnit*> OpponentUnitCount = UTbfGameFunctionLibrary::GetOpponentUnits(this);
		if (OpponentUnitCount.Num() <= 0)
		{
			SelectedUnit->BattleOpponent();
				GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,FString::Printf(TEXT("%s Attacked Opponent"), *Name.ToString()));
		}
		else
		{
			if (!TargetedUnit)
			{
				GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Did Not Select A Target To Attack"), *Name.ToString()));
				return;
			}
			if(UnitOnField.Contains(SelectedUnit))
			{
				SelectedUnit->SetTargetUnit(TargetedUnit);
				SelectedUnit->BattleOpponentUnit();
				GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,FString::Printf(TEXT("%s Attacked a Unit"), *Name.ToString()));
			};
			BattleCountPerTurn -= 1;
			Cast<UTbfAttributeSet>(AttributeSet)->SetBattlePoints(BattleCountPerTurn);
		}
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Is Out Of Moves"), *Name.ToString()));
		GoToNextState();
	}
	UpdateAttributeSet();
	UpdateUIStat();
}

void ATbfCharacter::ActivateSelectedCard()
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,FString::Printf(TEXT("%s Card Activation Starts"), *Name.ToString()));
	if (ActivateCountPerTurn <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s is Out Of Activation Moves"), *Name.ToString()));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("Activation Triggered")));
	if (Hand.Contains(SelectedCard) || CardOnField.Contains(SelectedCard))
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Selected Card to Activate in Hand Or Board Chaeck Passed"), *Name.ToString()));
		SelectedCard->ActivateCard();
		ActivateCountPerTurn--;
		Cast<UTbfAttributeSet>(AttributeSet)->SetActivatePoints(ActivateCountPerTurn);
		if (ActivateCountPerTurn <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Switch to Next State Due Activation Cost Exceeded"), *Name.ToString()));
			GoToNextState();
		}
	}
	UpdateAttributeSet();
}

void ATbfCharacter::ResetCounters()
{
	MoveCountPerTurn += 2;
	ActivateCountPerTurn += 2;
	BattleCountPerTurn += 2;
	DrawCountPerTurn += 1;
	UpdateAttributeSet();
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
		case ETbfPlayerState::Waiting:
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s Switched Waiting To Draw"), *Name.ToString()));
			CurrentState = ETbfPlayerState::Draw;
			break;
		case ETbfPlayerState::Draw:
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Switched Draw To MainOne"), *Name.ToString()));
			CurrentState = ETbfPlayerState::MainOne;
			break;
		case ETbfPlayerState::MainOne:
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Switched MainOne To Battle"), *Name.ToString()));
			CurrentState = ETbfPlayerState::Battle;
			break;
		case ETbfPlayerState::Battle:
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Switched Battle To MainTwo"), *Name.ToString()));
			CurrentState = ETbfPlayerState::MainTwo;
			break;
		case ETbfPlayerState::MainTwo:
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,FString::Printf(TEXT("%s Switched MainTwo To Waiting"), *Name.ToString()));
			CurrentState = ETbfPlayerState::Waiting;
			if (bIsPlayer)
			{
				//Player One Ends Turn and Waits
				Cast<UTbfGameInstance>(GetGameInstance())->bIsPlayerOneTurn = false;
				//Player Two Switches from Waiting to Draw State
				Cast<UTbfGameInstance>(GetGameInstance())->PlayerTwo->GoToNextState();
			}
			else
			{
				//Player Two Ends Turn and Waits
				Cast<UTbfGameInstance>(GetGameInstance())->bIsPlayerOneTurn = true;
				//Player One Switches from Waiting to Draw State
				Cast<UTbfGameInstance>(GetGameInstance())->PlayerOne->GoToNextState();
			}
			ResetCounters();
			break;
	}
	FText Message = FText::Format(
		FText::FromString("{0} {1} State"),
		FText::FromName(Name),
		FText::FromString(UTbfGameFunctionLibrary::PlayerStateToString(CurrentState)));
	ShowMessageInUI(Message);
}

void ATbfCharacter::UpdateAttributeSet()
{
	Cast<UTbfAttributeSet>(AttributeSet)->SetBattlePoints(BattleCountPerTurn);
	Cast<UTbfAttributeSet>(AttributeSet)->SetDrawPoints(DrawCountPerTurn);
	Cast<UTbfAttributeSet>(AttributeSet)->SetMovePoints(MoveCountPerTurn);
	Cast<UTbfAttributeSet>(AttributeSet)->SetActivatePoints(ActivateCountPerTurn);
	Cast<UTbfAttributeSet>(AttributeSet)->SetCardInDeck(Deck.Num());
	Cast<UTbfAttributeSet>(AttributeSet)->SetCardInField(CardOnField.Num());
	Cast<UTbfAttributeSet>(AttributeSet)->SetCardInHand(Hand.Num());
	Cast<UTbfAttributeSet>(AttributeSet)->SetUnitInField(UnitOnField.Num());
	
}


void ATbfCharacter::HandleCardDestroyed(AActor* DestroyedActor)
{
	if (ACardBase* DestroyedCard = Cast<ACardBase>(DestroyedActor))
	{
		if (CardOnField.Contains(DestroyedCard))
		{
			// Remove the destroyed card from the CardOnField array
			CardOnField.Remove(DestroyedCard);
		}
		if (Hand.Contains(DestroyedCard))
		{
			// Remove the destroyed card from the Hand array
			Hand.Remove(DestroyedCard);
		}
		// update the UI or perform other necessary actions
		UpdateAttributeSet();
		UpdateUIStat();
	}
}

void ATbfCharacter::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Handling Take Any Damage"));
		
	ATbfCharacterUnit* UnitCauser = Cast<ATbfCharacterUnit>(DamageCauser);
	ACardBase* CardCauser = Cast<ACardBase>(DamageCauser);

	UTbfAttributeSet* MyAttributes = Cast<UTbfAttributeSet>(AttributeSet);
	// If the damage is caused by a unit
	if (UnitCauser)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Handling Unit Causer Damage"));
		
		const float NewHealth = FMath::Clamp(MyAttributes->GetHealth() - Damage, 0, MyAttributes->GetHealth());
		MyAttributes->SetHealth(NewHealth);
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Finish Handling Unit Causer Damage"));
	}
	// If the damage is caused by a card
	if (CardCauser)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Handling Card Causer Damage"));
		CardCauser->ApplyEffectToTarget(this, CardCauser->CardInfo.GameplayEffectClass);
		
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("Finish Handling Card Causer Damage"));;
	}
}


// Called when the game starts or when spawned
void ATbfCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ATbfCharacter::HandleTakeAnyDamage);
}



