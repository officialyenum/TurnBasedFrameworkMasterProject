// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USelectionInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class TURNBASEDBPFRAMEWORK_API ISelectionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
	virtual void SelectActor() = 0;
	virtual void UnSelectActor() = 0;
};
