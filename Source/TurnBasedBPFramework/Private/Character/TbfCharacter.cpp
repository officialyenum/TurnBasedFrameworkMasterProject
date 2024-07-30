// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacter.h"

#include "Player/TbfPlayerState.h"


// Sets default values
ATbfCharacter::ATbfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ATbfCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATbfCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// For Server
	InitAbilityActorInfo();

	
}

void ATbfCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// For Client
	InitAbilityActorInfo();
}

// Called when the game starts or when spawned
void ATbfCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATbfCharacter::InitAbilityActorInfo()
{//Init ability actor info for the server
	ATbfPlayerState* TbfPlayerState = GetPlayerState<ATbfPlayerState>();
	check(TbfPlayerState);
	TbfPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TbfPlayerState, this);

	AbilitySystemComponent = TbfPlayerState->GetAbilitySystemComponent();
	AttributeSet = TbfPlayerState->GetAttributeSet();
}

