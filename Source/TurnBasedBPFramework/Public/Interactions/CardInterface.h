// Copyright Chukwuyenum Opone @officialyenum


#pragma once

#include "CoreMinimal.h"
#include "Character/TbfCharacterBase.h"
#include "UObject/Interface.h"
#include "CardInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCardInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDBPFRAMEWORK_API ICardInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	bool bIsSelected;
	bool bIsOnBoard;
	virtual void SetUpCard() = 0;
	virtual void ActivateCard() = 0;
	virtual void MoveCardToBoard() = 0;
	virtual void AddCardToHand(ATbfCharacterBase* PlayerToGive) = 0;
};
