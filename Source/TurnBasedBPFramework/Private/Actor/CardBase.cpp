// Copyright Chukwuyenum Opone @officialyenum


#include "Actor/CardBase.h"

#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Character/TbfCharacter.h"
#include "Components/ArrowComponent.h"
#include "Game/TbfGameInstance.h"
#include "Game/TbfGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Library/TbfGameFunctionLibrary.h"
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
	FrontWidget->SetRelativeLocation(FVector(0.0f,0.0f,100.0f));
	FrontWidget->SetRelativeRotation(FRotator3d(90.0f,0.0f,0.0));
	FrontWidget->SetRelativeScale3D(FVector(0.35,0.35,7.0f));
	FrontWidget->SetupAttachment(CardMesh);
	
	
	
	// Optionally, set the widget class here if you have a specific widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FrameworkV2/Blueprints/UI/Widgets/WBP_TbfCard"));
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
	SetUpCard();
	check(GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Card Name %s"), *CardInfo.Name.ToString()));

}

void ACardBase::AddCardToHand(ATbfCharacterBase* PlayerToGive)
{

}

void ACardBase::HighlightActor()
{
	
	CardMesh->SetRenderCustomDepth(true);
	CardMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ACardBase::UnHighlightActor()
{
	if (!bCardIsSelected)
	{
		CardMesh->SetRenderCustomDepth(false);
	}
}

void ACardBase::SelectActor()
{
	bCardIsSelected = true;
	CardMesh->SetRenderCustomDepth(true);
	CardMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI->bIsPlayerOneTurn)
	{
		GI->PlayerOne->SelectedCard = this;
	}
	else
	{
		GI->PlayerTwo->SelectedCard = this;
	}
}

void ACardBase::UnSelectActor()
{
	bCardIsSelected = false;
	CardMesh->SetRenderCustomDepth(false);
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI->bIsPlayerOneTurn)
	{
		GI->PlayerOne->SelectedCard = nullptr;
	}
	else
	{
		GI->PlayerTwo->SelectedCard = nullptr;
	}
}

void ACardBase::ActivateCard()
{
}

void ACardBase::MoveCardToBoard()
{
	ATbfCharacter* CurrentPlayer;
	ATbfGameMode* GM = Cast<ATbfGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM->GetIsPlayerOneTurn())
	{
		CurrentPlayer = GM->GetPlayerOne();
	}else
	{
		CurrentPlayer = GM->GetPlayerTwo();
	}

	if (CurrentPlayer->MoveCountPerTurn > 0)
	{
		CurrentPlayer->SelectedCard = this;
		CurrentPlayer->TargetedCell = UTbfGameFunctionLibrary::GetRandomCellForPlayer(CurrentPlayer);
		
	}
}



