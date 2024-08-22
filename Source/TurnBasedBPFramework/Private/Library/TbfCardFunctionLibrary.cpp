// Copyright Chukwuyenum Opone @officialyenum


#include "Library/TbfCardFunctionLibrary.h"

#include "Actor/CardBase.h"
#include "Character/TbfCharacter.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

TArray<FTbfCardInfoSim> UTbfCardFunctionLibrary::GetDeckSim(UDataTable* DataTable)
{
    TArray<FTbfCardInfoSim> Cards;
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable"));
        return Cards;
    }

    // Get all row names in the DataTable
    TArray<FName> RowNames = DataTable->GetRowNames();
    
    if (RowNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows in DataTable"));
        return Cards;
    }

    // Generate a Deck
    for (int i = 0; i < RowNames.Num(); ++i)
    {
        static const FString ContextString(TEXT("CardDataTableFunctionLibrary"));
        Cards.Add(*DataTable->FindRow<FTbfCardInfoSim>(RowNames[i], ContextString));
    }
    return Cards;
}

TArray<FTbfCardInfo> UTbfCardFunctionLibrary::GetDeck(UDataTable* DataTable)
{
    TArray<FTbfCardInfo> Cards;
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable"));
        return Cards;
    }

    // Get all row names in the DataTable
    TArray<FName> RowNames = DataTable->GetRowNames();
    
    if (RowNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows in DataTable"));
        return Cards;
    }

    // Generate a Deck
    for (int i = 0; i < RowNames.Num(); ++i)
    {
        static const FString ContextString(TEXT("CardDataTableFunctionLibrary"));
        Cards.Add(*DataTable->FindRow<FTbfCardInfo>(RowNames[i], ContextString));
    }
    return Cards;
}

FTbfCardInfo UTbfCardFunctionLibrary::GetRandomCardFromDataTable(UDataTable* DataTable)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable"));
        return FTbfCardInfo();
    }

    // Get all row names in the DataTable
    TArray<FName> RowNames = DataTable->GetRowNames();
    
    if (RowNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows in DataTable"));
        return FTbfCardInfo();
    }

    // Generate a random index
    int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);

    // Return the random row data
    static const FString ContextString(TEXT("RandomDataTableFunctionLibrary"));
    return *DataTable->FindRow<FTbfCardInfo>(RowNames[RandomIndex], ContextString);
}

FTbfCardInfo UTbfCardFunctionLibrary::GetRandomCardByType(UDataTable* DataTable, ECardType CardType)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable"));
        return FTbfCardInfo();
    }

    // Get all rows in the DataTable
    TArray<FName> RowNames = DataTable->GetRowNames();
    TArray<FTbfCardInfo*> FilteredRows;
    
    // Iterate through each row and filter by CardType
    for (const FName& RowName : RowNames)
    {
        static const FString ContextString(TEXT("RandomDataTableFunctionLibrary"));
        FTbfCardInfo* Row = DataTable->FindRow<FTbfCardInfo>(RowName, ContextString);
        if (Row && Row->Type == CardType)
        {
            FilteredRows.Add(Row);
        }
    }

    if (FilteredRows.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows of specified type in DataTable"));
        return FTbfCardInfo();
    }

    // Generate a random index
    int32 RandomIndex = FMath::RandRange(0, FilteredRows.Num() - 1);

    // Return the random row data
    return *FilteredRows[RandomIndex];
}

FTbfCardInfo UTbfCardFunctionLibrary::GetRandomUnitCard(UDataTable* DataTable)
{
    return GetRandomCardByType(DataTable, ECardType::Unit);
}

FTbfCardInfo UTbfCardFunctionLibrary::GetRandomSpellCard(UDataTable* DataTable)
{
    return GetRandomCardByType(DataTable, ECardType::Spell);
}

FTbfCardInfo UTbfCardFunctionLibrary::GetRandomTrapCard(UDataTable* DataTable)
{
    return GetRandomCardByType(DataTable, ECardType::Trap);
}

FTbfCardInfoSim UTbfCardFunctionLibrary::ConvertToCardSim(FGameStateSim State, FName CardName)
{
    for (FTbfCardInfoSim CardInfoSim : State.GeneralDeck)
    {
        if (CardInfoSim.Name.Compare(CardName) == 0)
        {
            return CardInfoSim;
        }
    }
    FTbfCardInfoSim Sim;
    return Sim;
}
FTbfUnitInfoSim UTbfCardFunctionLibrary::ConvertToUnitSim(FGameStateSim State, FName UnitName)
{
    for (FTbfCardInfoSim CardInfoSim : State.GeneralDeck)
    {
        if (CardInfoSim.Name.Compare(UnitName) == 0)
        {
            return CardInfoSim.Unit;
        }
    }
    FTbfUnitInfoSim Sim;
    return Sim;
}

ACardBase* UTbfCardFunctionLibrary::GetRandomCardForPlayer(ATbfCharacter* Player)
{
    int32 RandomIndex = FMath::RandRange(0, Player->CardOnField.Num() - 1);
    return Player->CardOnField[RandomIndex];
}

TArray<ACardBase*> UTbfCardFunctionLibrary::GetAllCardForPlayer(ATbfCharacter* Player)
{
    return Player->CardOnField;
}

