// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterUnit.h"

#include "TurnBasedBPFramework/TurnBasedBPFramework.h"


// Sets default values
ATbfCharacterUnit::ATbfCharacterUnit()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATbfCharacterUnit::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
}

void ATbfCharacterUnit::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}
