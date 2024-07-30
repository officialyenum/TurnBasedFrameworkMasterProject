// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfCharacterBase.h"
#include "Interactions/SelectionInterface.h"
#include "TbfCharacterUnit.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacterUnit : public ATbfCharacterBase, public ISelectionInterface
{
	GENERATED_BODY()

public:
	ATbfCharacterUnit();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SelectActor() override;
	virtual void UnSelectActor() override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modifier")
	bool bUnitIsSelected = false;
};
