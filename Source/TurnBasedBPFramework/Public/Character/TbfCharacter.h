// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "TbfCharacterBase.h"
#include "TbfCharacter.generated.h"

UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfCharacter : public ATbfCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATbfCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
