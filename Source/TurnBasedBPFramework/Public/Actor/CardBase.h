// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfEffectActor.h"
#include "Interactions/CardInterface.h"
#include "Interactions/SelectionInterface.h"
#include "CardBase.generated.h"


class UWidgetComponent;
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
struct FTbfCardInfo : public FTableRowBase
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
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void AddCardToHand(ATbfCharacterBase* PlayerToGive) override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void SetUpCard() override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void ActivateCard() override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void MoveCardToBoard() override;
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void MoveCardToLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void HighlightActor() override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void UnHighlightActor() override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void SelectActor() override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void UnSelectActor() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modifier")
	bool bOpponentCanSee = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modifier")
	bool bCardIsSelected = false;
	
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	FTbfCardInfo CardInfo;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> CardMesh;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UWidgetComponent> FrontWidget;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> SpawnDirectionArrow;
	

};
