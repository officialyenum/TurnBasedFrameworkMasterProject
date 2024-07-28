// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfEffectActor.h"
#include "Interactions/CardInterface.h"
#include "Interactions/SelectionInterface.h"
#include "CardBase.generated.h"


// Forward declaration
class ATbfCharacterUnit;

UENUM(BlueprintType)
enum class ECardType: uint8
{
	Unit,
	Spell,
	Trap
};

USTRUCT(BlueprintType)
struct FTbfCardInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = FName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Image;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor BackgroundImage = FLinearColor();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Info = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECardType Type = ECardType::Unit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATbfCharacterUnit> UnitClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Health = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Defence = 0.f;
};

UCLASS()
class TURNBASEDBPFRAMEWORK_API ACardBase : public ATbfEffectActor, public ISelectionInterface, public ICardInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardBase();
	virtual void AddCardToHand(ATbfCharacterBase* PlayerToGive) override;
	virtual void SetUpCard() override;
	virtual void ActivateCard() override;
	virtual void MoveCardToBoard() override;
	virtual void MoveCardToHandLocation(FVector& Location) override;

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UFUNCTION(BlueprintNativeEvent, Category="Selection")
	void HighlightMesh();
	UFUNCTION(BlueprintNativeEvent, Category="Selection")
	void UnHighlightMesh();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	FTbfCardInfo CardInfo;
};
