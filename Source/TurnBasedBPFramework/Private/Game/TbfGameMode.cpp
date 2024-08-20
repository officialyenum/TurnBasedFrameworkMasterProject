// Copyright Chukwuyenum Opone @officialyenum


#include "Game/TbfGameMode.h"

#include "Game/TbfGameInstance.h"
#include "Kismet/GameplayStatics.h"

ATbfGameMode::ATbfGameMode()
{
	// Set default game instance class
	GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

ATbfCharacter* ATbfGameMode::GetPlayerOne() const
{
	if (GI)
	{
		ATbfCharacter* Player = GI->PlayerOne;
		return Player;
	}
	return nullptr;
}

ATbfCharacter* ATbfGameMode::GetPlayerTwo() const
{
	if (GI)
	{
		ATbfCharacter* Player = GI->PlayerTwo;
		return Player;
	}
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("GameInstance not Initialized in C++ GameMode"));
	return nullptr;
}

bool ATbfGameMode::GetIsPlayerOneTurn() const
{
	if (GI)
	{
		return GI->bIsPlayerOneTurn;
	}
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("GameInstance not Initialized in C++ GameMode"));
	return false;
}

void ATbfGameMode::SwitchTurn()
{
	if (GI)
	{
		GI->bIsPlayerOneTurn = !GI->bIsPlayerOneTurn;
	}
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("GameInstance not Initialized in C++ GameMode"));
}
