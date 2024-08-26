// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfEffectActor.h"
#include "Character/TbfCharacterUnit.h"
#include "Interactions/CardInterface.h"
#include "Interactions/SelectionInterface.h"
#include "CardBase.generated.h"

class UWidgetComponent;
// Forward declaration
// class ATbfCharacterUnit;

UENUM(BlueprintType)
enum class ECardType: uint8
{
	Unit,
	Spell,
	Trap
};

UENUM(BlueprintType)
enum class ECardAlgo: uint8
{
	Random_Random,
	AlphaBeta_Random,
	Random_AlphaBeta,
	AlphaBeta_AlphaBeta
};

UENUM(BlueprintType)
enum class EUnitAlgo: uint8
{
	Random_Random,
	MonteCarlo_Random,
	Random_MonteCarlo,
	MonteCarlo_MonteCarlo
};

UENUM(BlueprintType)
enum class EModifierType: uint8
{
	Add,
	Override,
	Multiply,
};

UENUM(BlueprintType)
enum class EModifierParam: uint8
{
	None,
	Both,
	Health,
	Attack,
	Defence,
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

USTRUCT(BlueprintType)
struct FTbfCardInfoSim : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = FName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Info = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Rank = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECardType Type = ECardType::Unit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTbfUnitInfoSim Unit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EModifierParam ModifierParam = EModifierParam::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EModifierType ModifierType = EModifierType::Add;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ModifierValue = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsDead = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsActive = false;
};

USTRUCT(BlueprintType)
struct FGameStateSim
{
	GENERATED_BODY()

	// AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	int32 LifePoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> Deck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> Hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> CardField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfUnitInfoSim> UnitField;

	// Opponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	int32 OpponentLifePoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> OpponentCardDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> OpponentCardHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> OpponentCardField;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfUnitInfoSim> OpponentUnitField;

	// Additional Game State Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> OpponentDiscardedCards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<FTbfCardInfoSim> GeneralDeck;
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
	UFUNCTION(BlueprintImplementableEvent, Category="Card Actions")
	void SetUpCard();
	UFUNCTION(BlueprintImplementableEvent, Category="Card Actions")
	void ShowCardInUI();
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void ActivateCard() override;
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	void SpawnCardUnit();
	
	UFUNCTION(BlueprintCallable)
	void ActivateSpellWithGameplayEffect();
	
	UFUNCTION(BlueprintCallable)
	void ActivateTrapWithGameplayEffect();
	
	UFUNCTION(BlueprintCallable, Category="Card Actions")
	virtual void MoveCardToBoard() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Card Actions")
	void ImplementDestroyActor();
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Cell Tracker")
	AActor* CellOccupied;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Param", meta=(ExposeOnSpawn="true"))
	FTbfCardInfo CardInfo;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> CardMesh;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	// TObjectPtr<UWidgetComponent> FrontWidget;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> SpawnDirectionArrow;
	

};
