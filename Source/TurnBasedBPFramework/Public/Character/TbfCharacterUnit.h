// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interactions/SelectionInterface.h"
#include "TbfCharacterUnit.generated.h"

UENUM(BlueprintType)
enum class ETbfUnitState: uint8
{
	Idle,
	Attack,
	Defence,
};

USTRUCT(BlueprintType)
struct FTbfUnitInfo : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = FName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETbfUnitState UnitState = ETbfUnitState::Idle;
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
	ETbfUnitState UnitState = ETbfUnitState::Idle;
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
	UFUNCTION()
	virtual void HighlightActor() override;
	UFUNCTION()
	virtual void UnHighlightActor() override;
	UFUNCTION(BlueprintCallable, Category="Unit Actions")
	virtual void SelectActor() override;
	UFUNCTION(BlueprintCallable, Category="Unit Actions")
	virtual void UnSelectActor() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void SetToDefenseMode();
	UFUNCTION()
	void FindUnitToBattle();
	UFUNCTION()
	void BattleOpponentUnit();
	UFUNCTION()
	void BattleOpponent();

	void SetTargetUnit(ATbfCharacterUnit* NewTargetUnit);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modifier")
	bool bUnitIsSelected = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Cell Tracker")
	AActor* CellOccupied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Param")
	FTbfUnitInfo UnitInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Param")
	ATbfCharacterUnit* TargetUnit;
};
