// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "TbfCharacterBase.generated.h"

class UAttributeSet;
class UNiagaraSystem;

UCLASS(Abstract)
class TURNBASEDBPFRAMEWORK_API ATbfCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATbfCharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const { return  AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	UFUNCTION(BlueprintCallable, Category="Action Functions")
	void PlayHitAction() const;
	
	UFUNCTION(BlueprintCallable, Category="Action Functions")
	void PlayDeathAction() const;
	
	UPROPERTY(EditAnywhere, Category="Character Damage")
	TObjectPtr<UDamageType> DamageTypeClass;
	
	UPROPERTY(EditAnywhere, Category="Character Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere, Category="Character Effects")
	TObjectPtr<UNiagaraSystem> DeathEffect;

	UPROPERTY(EditAnywhere, Category="Character Sounds")
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere, Category="Character Sounds")
	TObjectPtr<USoundBase> DeathSound;

};
