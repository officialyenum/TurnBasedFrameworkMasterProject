// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "TbfCharacterBase.generated.h"

// Forward declaration
class ACardBase;
class ATbfCharacterUnit;

UCLASS(Abstract)
class TURNBASEDBPFRAMEWORK_API ATbfCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATbfCharacterBase();
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void DrawCard();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void PlayCard(int32 CardIndex);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Actions")
	void RepositionCardInHand();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<ACardBase*> Deck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<ACardBase*> Hand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<ACardBase*> CardOnField;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<ATbfCharacterUnit*> UnitOnField;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT;
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
