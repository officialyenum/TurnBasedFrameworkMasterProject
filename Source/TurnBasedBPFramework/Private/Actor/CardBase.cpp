// Copyright Chukwuyenum Opone @officialyenum


#include "Actor/CardBase.h"


// Sets default values
ACardBase::ACardBase(): CardInfo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACardBase::BeginPlay()
{
	Super::BeginPlay();
	InfiniteGameplayEffectClass = CardInfo.GameplayEffectClass;
}

void ACardBase::AddCardToHand(ATbfCharacterBase* PlayerToGive)
{

}

void ACardBase::MoveCardToHandLocation(FVector& Location)
{
}

void ACardBase::HighlightActor()
{
	HighlightMesh_Implementation();
}

void ACardBase::UnHighlightActor()
{
	UnHighlightMesh_Implementation();
}

void ACardBase::HighlightMesh_Implementation()
{
	//OVERRIDE THIS IN Blueprint
}

void ACardBase::UnHighlightMesh_Implementation()
{
	//OVERRIDE THIS IN Blueprint
}


void ACardBase::SetUpCard()
{
}

void ACardBase::ActivateCard()
{
}

void ACardBase::MoveCardToBoard()
{
}

