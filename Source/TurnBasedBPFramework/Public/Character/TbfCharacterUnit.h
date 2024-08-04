// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interactions/SelectionInterface.h"
#include "TbfCharacterUnit.generated.h"

USTRUCT(BlueprintType)
struct FTbfUnitInfo : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = FName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Defence = 0.f;
};

USTRUCT(BlueprintType)
struct FTbfUnitInfoSim : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = FName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Defence = 0.f;
};

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacterUnit : public ATbfCharacterBase, public ISelectionInterface
{
	GENERATED_BODY()

public:
	ATbfCharacterUnit();
	
	UBehaviorTree* GetBehaviorTree() const;
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SelectActor() override;
	virtual void UnSelectActor() override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modifier")
	bool bUnitIsSelected = false;
};
