// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "Game/TbfGameInstance.h"
#include "Game/TbfGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TbfPlayerState.h"

// Sets default values
ATbfCharacterPlayer::ATbfCharacterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ATbfCharacterPlayer::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATbfCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// For Server
	InitAbilityActorInfo();
}

void ATbfCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// For Client
	InitAbilityActorInfo();
}

void ATbfCharacterPlayer::InitAbilityActorInfo()
{
	//Init ability actor info for the server
	ATbfPlayerState* TbfPlayerState = GetPlayerState<ATbfPlayerState>();
	check(TbfPlayerState);
	TbfPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TbfPlayerState, this);

	AbilitySystemComponent = TbfPlayerState->GetAbilitySystemComponent();
	AttributeSet = TbfPlayerState->GetAttributeSet();
}

// Called when the game starts or when spawned
void ATbfCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	// Set User Player in Game Instance Player One
	//UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI->PlayerOne)
	//{
		//GI->PlayerOne = this;
	//}
}