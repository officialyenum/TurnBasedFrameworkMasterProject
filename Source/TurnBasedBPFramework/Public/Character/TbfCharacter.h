// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "TbfCharacterBase.h"
#include "TbfCharacter.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacter : public ATbfCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATbfCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return  AttributeSet; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	virtual void InitAbilityActorInfo();
};
