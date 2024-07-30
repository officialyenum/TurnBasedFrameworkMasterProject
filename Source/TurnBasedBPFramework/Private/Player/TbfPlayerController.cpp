// Copyright Chukwuyenum Opone @officialyenum


#include "Player/TbfPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "Character/TbfCharacterPlayer.h"
#include "Character/TbfCharacterUnit.h"
#include "Game/TbfGameInstance.h"
#include "Interactions/SelectionInterface.h"
#include "Kismet/GameplayStatics.h"

ATbfPlayerController::ATbfPlayerController()
{
	bReplicates = true;
}

void ATbfPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(TbfContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(TbfContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ATbfPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	
}

void ATbfPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATbfPlayerController::PauseGame);
	EnhancedInputComponent->BindAction(DrawAction, ETriggerEvent::Started, this, &ATbfPlayerController::Draw);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ATbfPlayerController::Move);
	EnhancedInputComponent->BindAction(ActivateAction, ETriggerEvent::Started, this, &ATbfPlayerController::Activate);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATbfPlayerController::Attack);
}

void ATbfPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false,CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<ISelectionInterface>(CursorHit.GetActor());
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void ATbfPlayerController::PauseGame(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("PauseGame Pressed"));
	// Call the UE5 Pause Game Function to Pause the Game
	
	// Call the UE5 Pause Game Function to Pause the Game
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ATbfPlayerController::Draw(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Draw Pressed"));
	//if player is in Draw State Call Draw Function on Player Character
	// Get Player Character
	if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
	{
		// if player is in Draw State Call Draw Function on Player Character
		if (PlayerCharacter->CurrentState == EPlayerState::Draw)
		{
			PlayerCharacter->DrawCard();
		}
	}
}

void ATbfPlayerController::Move(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Move Pressed"));
	//Get Player Character
	if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
	{
		// if player is in Main One Or Main Two State Call Move Card To Cell Location Function on Player Character
		if (PlayerCharacter->CurrentState == EPlayerState::MainOne || PlayerCharacter->CurrentState == EPlayerState::MainTwo)
		{
			// Set the card to bOpponentCanSee to true
			if (PlayerCharacter->SelectedCard && PlayerCharacter->TargetedCell)
			{
				PlayerCharacter->PlaySelectedCard();
			}
		}
	}
}

void ATbfPlayerController::Activate(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Activate Pressed"));
	//Get Player Character
	////if player is in Main One Or Main Two State Call Activate Function to Activate the Selected Card On Field
	if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
	{
		// if player is in Main One Or Main Two State Call Activate Function to Activate the Selected Card On Field
		if (PlayerCharacter->CurrentState == EPlayerState::MainOne || PlayerCharacter->CurrentState == EPlayerState::MainTwo)
		{
			if (PlayerCharacter->SelectedCard)
			{
				PlayerCharacter->SelectedCard->ActivateCard();
			}
		}
	}
}

void ATbfPlayerController::Attack(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Attack Pressed"));
	//Get Player Character

	//if player is in Battle State Call Attack Function on the SelectedUnit and pass the Selected TargetUnit as a Parameter to Attack
	if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
	{
		// if player is in Battle State Call Attack Function on the SelectedUnit and pass the Selected TargetUnit as a Parameter to Attack
		if (PlayerCharacter->CurrentState == EPlayerState::Battle)
		{
			if (PlayerCharacter->SelectedUnit && PlayerCharacter->TargetedUnit)
			{
				UE_LOG(LogTemp, Error, TEXT("Attack Not Implemented Yet: See TbfPlayerController.cpp line 152"));
				//PlayerCharacter->SelectedUnit->Attack(PlayerCharacter->SelectedTargetUnit);
			}
		}
	}
}
