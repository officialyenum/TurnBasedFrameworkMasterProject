// Copyright Chukwuyenum Opone @officialyenum


#include "Library/TbfGameFunctionLibrary.h"

#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "Character/TbfCharacter.h"

#include "Engine/World.h"
#include "Game/TbfGameInstance.h"
#include "Kismet/GameplayStatics.h"


ATbfGridCell* UTbfGameFunctionLibrary::GetRandomCellForPlayer(const ATbfCharacter* Player)
{
	if (!IsValid(Player) || !IsValid(Player->SelectedCard))
	{
		return nullptr;
	}

	// Get all grid cells in the world
	TArray<AActor*> AllGridCells;
	UGameplayStatics::GetAllActorsOfClass(Player->GetWorld(), ATbfGridCell::StaticClass(), AllGridCells);

	// Determine card and player type
	const ECardType CardType = Player->SelectedCard->CardInfo.Type;
	const bool bIsPlayerOne = Player->bIsPlayer;

	// Filter cells based on type and player
	TArray<ATbfGridCell*> FilteredCells;
	for (AActor* Actor : AllGridCells)
	{
		if (ATbfGridCell* GridCell = Cast<ATbfGridCell>(Actor))
		{
			if(GridCell->OccupyingActor == nullptr)
			{
				const ECellType DesiredType = (CardType == ECardType::Unit) 
				? (bIsPlayerOne ? ECellType::PlayerOneUnit : ECellType::PlayerTwoUnit)
				: (bIsPlayerOne ? ECellType::PlayerOneSpell : ECellType::PlayerTwoSpell);

				if (GridCell->CellType == DesiredType)
				{
					FilteredCells.Add(GridCell);
				}
			}
		}
	}

	// Return a random filtered cell
	if (FilteredCells.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, FilteredCells.Num() - 1);
		return FilteredCells[RandomIndex];
	}

	return nullptr;
}

ATbfCharacterUnit* UTbfGameFunctionLibrary::GetRandomUnitForPlayer(const ATbfCharacter* Player)
{
	if (Player->UnitOnField.Num() == 0) return nullptr;
	const int32 RandomIndex = FMath::RandRange(0, Player->UnitOnField.Num() - 1);
	return Player->UnitOnField[RandomIndex];
}

ATbfCharacterUnit* UTbfGameFunctionLibrary::GetRandomOpponentUnit(const ATbfCharacter* Player)
{
	if (!IsValid(Player)) return nullptr;

	const UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(Player));
	if (!IsValid(GI) || !IsValid(GI->PlayerOne) || !IsValid(GI->PlayerTwo)) return nullptr;

	const ATbfCharacter* Opponent = (GI->PlayerOne->id == Player->id) ? GI->PlayerTwo : GI->PlayerOne;
	
	if (Opponent->UnitOnField.Num() == 0) return nullptr;

	const int32 UnitIndex = FMath::RandRange(0, Opponent->UnitOnField.Num() - 1);
	return Opponent->UnitOnField[UnitIndex];
}


TArray<ATbfCharacterUnit*> UTbfGameFunctionLibrary::GetOpponentUnits(const ATbfCharacter* Player)
{
	if (!IsValid(Player)) return TArray<ATbfCharacterUnit*>();

	const UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(Player));
	if (!IsValid(GI) || !IsValid(GI->PlayerOne) || !IsValid(GI->PlayerTwo)) return TArray<ATbfCharacterUnit*>();

	const ATbfCharacter* Opponent = (GI->PlayerOne->id == Player->id) ? GI->PlayerTwo : GI->PlayerOne;
	return Opponent->UnitOnField;
}


TArray<ATbfCharacterUnit*> UTbfGameFunctionLibrary::GetAllUnitForPlayer(const ATbfCharacter* Player)
{
	return Player->UnitOnField;
}

FString UTbfGameFunctionLibrary::PlayerStateToString(ETbfPlayerState State)
{
	switch (State)
	{
	case ETbfPlayerState::Waiting:
		return TEXT("Waiting");
	case ETbfPlayerState::Draw:
		return TEXT("Draw");
	case ETbfPlayerState::MainOne:
		return TEXT("Main One");
	case ETbfPlayerState::Battle:
		return TEXT("Battle");
	case ETbfPlayerState::MainTwo:
		return TEXT("Main Two");
	default:
		return TEXT("Unknown");
	}
}

