// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TbfCharacterBase.generated.h"

UCLASS(Abstract)
class TURNBASEDBPFRAMEWORK_API ATbfCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATbfCharacterBase();

protected:
	virtual void BeginPlay() override;
};
