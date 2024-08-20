// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "TbfCharacter.h"
#include "TbfCharacterBase.h"
#include "TbfCharacterPlayer.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacterPlayer : public ATbfCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATbfCharacterPlayer();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override { return  AttributeSet; }
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	virtual void InitAbilityActorInfo() override;
};
