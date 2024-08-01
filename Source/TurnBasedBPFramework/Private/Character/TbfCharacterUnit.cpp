// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterUnit.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "AI/TbfUnitAIController.h"
#include "TurnBasedBPFramework/TurnBasedBPFramework.h"


// Sets default values
ATbfCharacterUnit::ATbfCharacterUnit()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTbfAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UTbfAttributeSet>("AttributeSet");

	
	// Set AIControllerClass to ATbfAIController
	AIControllerClass = ATbfUnitAIController::StaticClass();
}

void ATbfCharacterUnit::HighlightActor()
{
	if (!bUnitIsSelected)
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	}
}

void ATbfCharacterUnit::UnHighlightActor()
{
	if (!bUnitIsSelected)
	{
		GetMesh()->SetRenderCustomDepth(false);
	}
}

void ATbfCharacterUnit::SelectActor()
{
	bUnitIsSelected = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
}

void ATbfCharacterUnit::UnSelectActor()
{
	bUnitIsSelected = false;
	GetMesh()->SetRenderCustomDepth(false);
}

void ATbfCharacterUnit::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
