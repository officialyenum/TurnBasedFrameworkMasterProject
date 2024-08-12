// Copyright Chukwuyenum Opone @officialyenum


#include "Actor/CardBase.h"

#include "UObject/ConstructorHelpers.h"
#include "Character/TbfCharacter.h"
#include "Character/TbfCharacterUnit.h"
#include "Components/ArrowComponent.h"
#include "Game/TbfGameInstance.h"
#include "Game/TbfGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Library/TbfGameFunctionLibrary.h"
#include "TurnBasedBPFramework/TurnBasedBPFramework.h"

// Sets default values
ACardBase::ACardBase(): CardInfo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
	CardMesh->SetupAttachment(RootComponent);
	CardMesh->SetRelativeScale3D(FVector3d(1.5,1.0,0.05));
	// Find and set the static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshClass(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	if (MeshClass.Succeeded())
	{
		CardMesh->SetStaticMesh(MeshClass.Object);
	}

	// FrontWidget = CreateDefaultSubobject<UWidgetComponent>("FrontWidget");
	// FrontWidget = CreateDefaultSubobject<UWidgetComponent>("FrontWidget");
	// FrontWidget->SetRelativeLocation(FVector(0.0f,0.0f,100.0f));
	// FrontWidget->SetRelativeRotation(FRotator3d(90.0f,0.0f,0.0));
	// FrontWidget->SetRelativeScale3D(FVector(0.35,0.35,7.0f));
	// FrontWidget->SetupAttachment(CardMesh);
	
	
	
	// Optionally, set the widget class here if you have a specific widget class
	// static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FrameworkV2/Blueprints/UI/Widgets/WBP_TbfCard"));
	// if (WidgetClass.Succeeded())
	// {
	// 	FrontWidget->SetWidgetClass(WidgetClass.Class);
	// }
	// // Set the size of the widget
	// FrontWidget->SetDrawSize(FVector2D(250, 400)); // Set the desired size
	//
	SpawnDirectionArrow = CreateDefaultSubobject<UArrowComponent>("SpawnDirectionArrow");
	SpawnDirectionArrow->SetRelativeLocation(FVector(0.0f,0.0f,40.0f));
	SpawnDirectionArrow->SetArrowSize(1.0f);
	SpawnDirectionArrow->SetArrowLength(50.0f);
}

// Called when the game starts or when spawned
void ACardBase::BeginPlay()
{
	Super::BeginPlay();
	InfiniteGameplayEffectClass = CardInfo.GameplayEffectClass;
	SetUpCard();
	check(GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Card Name %s"), *CardInfo.Name.ToString()));

}

void ACardBase::AddCardToHand(ATbfCharacterBase* PlayerToGive)
{

}

void ACardBase::HighlightActor()
{
	
	CardMesh->SetRenderCustomDepth(true);
	CardMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ACardBase::UnHighlightActor()
{
	if (!bCardIsSelected)
	{
		CardMesh->SetRenderCustomDepth(false);
	}
}

void ACardBase::SelectActor()
{
		
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI->bIsPlayerOneTurn)
	{
		if (GI->PlayerOne->SelectedCard)
		{
			GI->PlayerOne->SelectedCard->UnSelectActor();
		}
		bCardIsSelected = true;
		GI->PlayerOne->SelectedCard = this;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Player One Selected Card Name %s"), *CardInfo.Name.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Calling Show Card In UI %s"), *CardInfo.Name.ToString()));
		ShowCardInUI();
	}
	
}

void ACardBase::UnSelectActor()
{
	bCardIsSelected = false;
	CardMesh->SetRenderCustomDepth(false);
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI->bIsPlayerOneTurn)
	{
		if (this == GI->PlayerOne->SelectedCard)
		{
			GI->PlayerOne->SelectedCard = nullptr;
		}
	}
}

void ACardBase::ActivateCard()
{
	// Card Gets Destroyed After this Action so occupying actor should be cleard
	Cast<ATbfGridCell>(CellOccupied)->OccupyingActor = this;
	switch (CardInfo.Type)
	{
		case ECardType::Unit:
			SpawnCardUnit();
			break;
		case ECardType::Spell:
			ActivateSpellWithGameplayEffect();
			break;
	case ECardType::Trap:
			ActivateTrapWithGameplayEffect();
			break;
	}
}

void ACardBase::SpawnCardUnit()
{
	// Define spawn transform
	const UTbfGameInstance* GI = Cast<UTbfGameInstance>(GetGameInstance());
	ATbfCharacter* UnitOwner;
	if (GI->bIsPlayerOneTurn)
	{
		UnitOwner = GI->PlayerOne;
	}
	else
	{
		UnitOwner = GI->PlayerTwo;
	}
	FTransform SpawnTransform = GetActorTransform();
	FVector Loc = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 10.0f);
	SpawnTransform.SetLocation(Loc);
	ATbfCharacterUnit* Unit = GetWorld()->SpawnActorDeferred<ATbfCharacterUnit>(
		CardInfo.UnitClass, // Ensure this is set to a valid class
		SpawnTransform,
		UnitOwner,
		UnitOwner,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	FTbfUnitInfo UnitInformation;
	UnitInformation.Name = CardInfo.Name;
	UnitInformation.Attack = CardInfo.Attack;
	UnitInformation.Defence = CardInfo.Defence;
	UnitInformation.UnitState = ETbfUnitState::Attack;
	Unit->UnitInfo = UnitInformation;
	// Add Spawned Unit as Cell Occupying Actor And Vice Versa
	if (CellOccupied)
	{
		Cast<ATbfGridCell>(CellOccupied)->OccupyingActor = Unit;
		Unit->CellOccupied = CellOccupied;
	}
	// Finish spawning the unit actor
	Unit->FinishSpawning(SpawnTransform);
	// Bind the unit's destruction event to a function that will handle removal from the array
	Unit->OnDestroyed.AddDynamic(Unit, &ATbfCharacterUnit::HandleUnitDestroyed);
	// Remove this card from fielded cards
	UnitOwner->CardOnField.Remove(this);
	// Add Unit to Help Player Track
	UnitOwner->UnitOnField.Add(Unit);
	// Update Character UI Stat
	UnitOwner->UpdateUIStat();
	
	// Play Animation to Destroy
	Destroy();
}

void ACardBase::ActivateSpellWithGameplayEffect()
{
	
	ATbfCharacter* CardOwner = Cast<ATbfCharacter>(GetOwner());
	if (CardOwner->SelectedUnit == nullptr)
	{
		return;
	}
	AController* InstigatorController = GetInstigatorController();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Spell Implemented On Unit %s"), *CardOwner->SelectedUnit->UnitInfo.Name.ToString()));

	UDamageType* DamageTypeClass = nullptr;
	CardOwner->SelectedUnit->HandleTakeAnyDamage(CardOwner->SelectedUnit,0.f, DamageTypeClass,InstigatorController,this);
	
}

void ACardBase::ActivateTrapWithGameplayEffect()
{
	ATbfCharacter* CardOwner = Cast<ATbfCharacter>(GetOwner());
	if (CardOwner->TargetedUnit == nullptr)
	{
		return;
	}
	AController* InstigatorController = GetInstigatorController();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Trap Implemented On Unit %s"), *CardOwner->TargetedUnit->UnitInfo.Name.ToString()));

	UDamageType* DamageTypeClass = nullptr;
	CardOwner->TargetedUnit->HandleTakeAnyDamage(CardOwner->SelectedUnit,0.f, DamageTypeClass,InstigatorController,this);
	
}

void ACardBase::MoveCardToBoard()
{
	ATbfCharacter* CurrentPlayer;
	ATbfGameMode* GM = Cast<ATbfGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM->GetIsPlayerOneTurn())
	{
		CurrentPlayer = GM->GetPlayerOne();
	}else
	{
		CurrentPlayer = GM->GetPlayerTwo();
	}

	if (CurrentPlayer->MoveCountPerTurn > 0)
	{
		CurrentPlayer->SelectedCard = this;
		CurrentPlayer->TargetedCell = UTbfGameFunctionLibrary::GetRandomCellForPlayer(CurrentPlayer);
		CurrentPlayer->TargetedCell->OccupyingActor = this;
	}
}



