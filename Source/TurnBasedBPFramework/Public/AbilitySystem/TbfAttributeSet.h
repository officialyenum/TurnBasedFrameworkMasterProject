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
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, MaxHealth);
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	/** Primary Attributes */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Attack, Category = "Primary Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, Attack);

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxAttack, Category = "Primary Attributes")
	FGameplayAttributeData MaxAttack;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, MaxAttack);
	
	UFUNCTION()
	void OnRep_MaxAttack(const FGameplayAttributeData& OldMaxAttack) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Defence, Category = "Primary Attributes")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, Defence);

	UFUNCTION()
	void OnRep_Defence(const FGameplayAttributeData& OldDefence) const;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxDefence, Category = "Primary Attributes")
	FGameplayAttributeData MaxDefence;
	ATTRIBUTE_ACCESSORS(UTbfAttributeSet, MaxDefence);
	
	UFUNCTION()
	void OnRep_MaxDefence(const FGameplayAttributeData& OldMaxDefence) const;
	
};
