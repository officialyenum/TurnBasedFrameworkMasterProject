// Copyright Chukwuyenum Opone @officialyenum


#include "Library/TbfGameFunctionLibrary.h"

#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "Character/TbfCharacter.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


ATbfGridCell* UTbfGameFunctionLibrary::GetRandomCellForPlayer(const ATbfCharacter* Player)
{
	if (!Player)
	{
		return nullptr;
	}
	if (!Player->SelectedCard)
	{
		return nullptr;
	}
	// Get one Random Cell from this list and return
	TArray<AActor*> AllGridCells;
	UGameplayStatics::GetAllActorsOfClass(Player->GetWorld(), ATbfGridCell::StaticClass(), AllGridCells);

	TArray<ATbfGridCell*> FilteredCells;
	ECardType CardType = Player->SelectedCard->CardInfo.Type; 
	for (AActor* Actor : AllGridCells)
	{
		ATbfGridCell* GridCell = Cast<ATbfGridCell>(Actor);
		if (GridCell)
		{
			if (Player->bIsPlayer)
			{
				// Player One
				if (CardType == ECardType::Unit && GridCell->CellType == ECellType::PlayerOneUnit)
				{
					// If Card type == Unit 
					// Filter for cell with type PlayerOneUnit
					FilteredCells.Add(GridCell);
				}
				else if (CardType != ECardType::Unit && GridCell->CellType == ECellType::PlayerOneSpell)
				{
					// If Card type != Unit 
					// Filter for cell with type PlayerOneSpell
					FilteredCells.Add(GridCell);
				}
			}
			else
			{
				// Player Two
				if (CardType == ECardType::Unit && GridCell->CellType == ECellType::PlayerTwoUnit)
				{
					FilteredCells.Add(GridCell);
				}
				else if (CardType != ECardType::Unit && GridCell->CellType == ECellType::PlayerTwoSpell)
				{
					FilteredCells.Add(GridCell);
				}
			}
		}
	}

	if (FilteredCells.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, FilteredCells.Num() - 1);
		return FilteredCells[RandomIndex];
	}

	return nullptr;
}

ATbfCharacterUnit* UTbfGameFunctionLibrary::GetRandomUnitForPlayer(const ATbfCharacter* Player)
{
	int32 RandomIndex = FMath::RandRange(0, Player->UnitOnField.Num() - 1);
	return Player->UnitOnField[RandomIndex];
}

TArray<ATbfCharacterUnit*> UTbfGameFunctionLibrary::GetAllUnitForPlayer(const ATbfCharacter* Player)
{
	return Player->UnitOnField;
}

FString UTbfGameFunctionLibrary::PlayerStateToString(EPlayerState State)
{
	switch (State)
	{
	case EPlayerState::Waiting:
		return TEXT("Waiting");
	case EPlayerState::Draw:
		return TEXT("Draw");
	case EPlayerState::MainOne:
		return TEXT("Main One");
	case EPlayerState::Battle:
		return TEXT("Battle");
	case EPlayerState::MainTwo:
		return TEXT("Main Two");
	default:
		return TEXT("Unknown");
	}
}

