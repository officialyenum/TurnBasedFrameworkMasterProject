// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfEffectActor.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Interactions/SelectionInterface.h"
#include "TbfGridCell.generated.h"

UENUM(BlueprintType)
enum class ECellType: uint8
{
	PlayerOneUnit,
	PlayerOneSpell,
	PlayerTwoUnit,
	PlayerTwoSpell
};

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfGridCell : public ATbfEffectActor, public ISelectionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATbfGridCell();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual void SelectActor() override;
	virtual void UnSelectActor() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(EditAnywhere, Category = "Cell Grid Components")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, Category = "Cell Grid Components")
	UTextRenderComponent* TextRender;

	UPROPERTY(EditAnywhere, Category = "Cell Grid Components")
	UDecalComponent* SelectionDecal;

	UPROPERTY(EditAnywhere, Category = "Cell Grid Components")
	UArrowComponent* SpawnDirectionArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell Grid Params", meta=(ExposeOnSpawn="true"))
	ECellType CellType;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
