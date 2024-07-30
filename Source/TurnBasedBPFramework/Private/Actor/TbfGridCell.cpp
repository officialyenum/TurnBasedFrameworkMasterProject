// Copyright Chukwuyenum Opone @officialyenum


#include "Actor/TbfGridCell.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/DecalComponent.h"
#include "Components/ArrowComponent.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TurnBasedBPFramework/TurnBasedBPFramework.h"

// Sets default values
ATbfGridCell::ATbfGridCell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// TileMesh Setup
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
	TileMesh->SetupAttachment(RootComponent);

	// Find and set the static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshClass(TEXT("/Game/Assets/Dungeon/SM_Tile1x1_A"));
	if (MeshClass.Succeeded())
	{
		TileMesh->SetStaticMesh(MeshClass.Object);
	}

	// Enable input and set the mesh to be clickable
	TileMesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	TileMesh->SetGenerateOverlapEvents(true);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCanEverAffectNavigation(false);

	// Text Render Setup
	TextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRender->SetHorizontalAlignment(EHTA_Center);
	TextRender->SetVerticalAlignment(EVRTA_TextCenter);
	TextRender->SetupAttachment(TileMesh);

	// Decal Setup
	SelectionDecal = CreateDefaultSubobject<UDecalComponent>("SelectionDecal");
	SelectionDecal->SetupAttachment(TileMesh);
	SelectionDecal->SetRelativeLocation(FVector(100.0f, 100.0f, 0.0f));
	SelectionDecal->SetRelativeRotation(FRotator3d(90.0f, 0.0f, 0.0f));
	SelectionDecal->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialClass(TEXT("Material'/Game/Cursor/M_Target.M_Target'"));
	if (MaterialClass.Succeeded())
	{
		SelectionDecal->SetDecalMaterial(MaterialClass.Object);
	}
	SelectionDecal->SetVisibility(false);

	SpawnDirectionArrow = CreateDefaultSubobject<UArrowComponent>("SpawnDirectionArrow");
	SpawnDirectionArrow->SetupAttachment(TileMesh);
	SpawnDirectionArrow->SetWorldLocation(FVector(100.0f, 100.0f, 50.0f));
	SpawnDirectionArrow->SetArrowSize(1.0f);
	SpawnDirectionArrow->SetArrowLength(100.0f);

	// Enable input for this actor
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ATbfGridCell::HighlightActor()
{
	if (bIsActorSelected)
	{
		TileMesh->SetRenderCustomDepth(true);
		TileMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void ATbfGridCell::UnHighlightActor()
{
	if (bIsActorSelected)
	{
		TileMesh->SetRenderCustomDepth(false);
	}
}

void ATbfGridCell::SelectActor()
{
	bIsActorSelected = true;
	SelectionDecal->SetVisibility(true);
}

void ATbfGridCell::UnSelectActor()
{
	bIsActorSelected = false;
	SelectionDecal->SetVisibility(false);
}

void ATbfGridCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ATbfGridCell::BeginPlay()
{
	Super::BeginPlay();
	
}


