// Copyright Chukwuyenum Opone @officialyenum


#include "Actor/CardBase.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
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
	
	FrontWidget = CreateDefaultSubobject<UWidgetComponent>("FrontWidget");
	FrontWidget->SetRelativeLocation(FVector(0.0f,0.0f,3.0f));
	FrontWidget->SetRelativeRotation(FRotator3d(90.0f,0.0f,0.0));
	FrontWidget->SetRelativeScale3D(FVector(0.35,0.35,0.35f));
	FrontWidget->SetupAttachment(CardMesh);
	
	
	
	// Optionally, set the widget class here if you have a specific widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FrameWork/UI/Widgets/WB_Card"));
	if (WidgetClass.Succeeded())
	{
		FrontWidget->SetWidgetClass(WidgetClass.Class);
	}
	// Set the size of the widget
	FrontWidget->SetDrawSize(FVector2D(250, 400)); // Set the desired size
	
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
}

void ACardBase::AddCardToHand(ATbfCharacterBase* PlayerToGive)
{

}

void ACardBase::MoveCardToHandLocation(FVector& Location)
{
}

void ACardBase::HighlightActor()
{
	CardMesh->SetRenderCustomDepth(true);
	CardMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ACardBase::UnHighlightActor()
{
	CardMesh->SetRenderCustomDepth(false);
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

