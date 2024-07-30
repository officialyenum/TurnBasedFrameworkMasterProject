// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interactions/SelectionInterface.h"
#include "TbfPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API ATbfPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATbfPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> TbfContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> PauseAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DrawAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ActivateAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttackAction;

	void PauseGame(const struct FInputActionValue& InputActionValue);
	void Draw(const struct FInputActionValue& InputActionValue);
	void Move(const struct FInputActionValue& InputActionValue);
	void Activate(const struct FInputActionValue& InputActionValue);
	void Attack(const struct FInputActionValue& InputActionValue);

	ISelectionInterface* LastActor;
	ISelectionInterface* ThisActor;

	FVector CachedDestination = FVector::Zero();
	void CursorTrace();
	FHitResult CursorHit;
};
