// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TbfAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UTbfAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UTbfAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Vital Attributes */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, Health);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	/** Game Attributes */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_DrawPoints, Category = "Game Attributes")
	FGameplayAttributeData DrawPoints;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, DrawPoints);

	UFUNCTION()
	void OnRep_DrawPoints(const FGameplayAttributeData& OldDrawPoints) const;
	/** -------------- */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MovePoints, Category = "Game Attributes")
	FGameplayAttributeData MovePoints;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, MovePoints);
	
	UFUNCTION()
	void OnRep_MovePoints(const FGameplayAttributeData& OldMovePoints) const;
	
	/** -------------- */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ActivatePoints, Category = "Game Attributes")
	FGameplayAttributeData ActivatePoints;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, ActivatePoints);

	UFUNCTION()
	void OnRep_ActivatePoints(const FGameplayAttributeData& OldActivatePoints) const;
	
	/** -------------- */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BattlePoints, Category = "Game Attributes")
	FGameplayAttributeData BattlePoints;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, BattlePoints);
	
	UFUNCTION()
	void OnRep_BattlePoints(const FGameplayAttributeData& OldBattlePoints) const;
	
	/** Unit Attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Attack, Category = "Unit Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, Attack);
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;
	
	/** -------------- */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Defence, Category = "Unit Attributes")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, Defence);
	
	UFUNCTION()
	void OnRep_Defence(const FGameplayAttributeData& OldDefence) const;
	
	/** Game Player Attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_UnitInField, Category = "Game Player Attributes")
	FGameplayAttributeData UnitInField;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, UnitInField);
	
	UFUNCTION()
	void OnRep_UnitInField(const FGameplayAttributeData& OldUnitInField) const;

	/** -------------- */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CardInHand, Category = "Game Player Attributes")
	FGameplayAttributeData CardInHand;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, CardInHand);
	
	UFUNCTION()
	void OnRep_CardInHand(const FGameplayAttributeData& OldCardInHand) const;
	
	/** -------------- */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CardInDeck, Category = "Game Player Attributes")
	FGameplayAttributeData CardInDeck;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, CardInDeck);
	
	UFUNCTION()
	void OnRep_CardInDeck(const FGameplayAttributeData& OldCardInDeck) const;
	
	/** -------------- */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CardInField, Category = "Game Player Attributes")
	FGameplayAttributeData CardInField;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, CardInField);
	
	UFUNCTION()
	void OnRep_CardInField(const FGameplayAttributeData& OldCardInField) const;
	
	/** -------------- */
	
	
};
