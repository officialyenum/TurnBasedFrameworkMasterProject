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
	LastActor = nullptr;
	ThisActor = nullptr;
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
	EnhancedInputComponent->BindAction(NextStateAction, ETriggerEvent::Started, this, &ATbfPlayerController::NextState);
	EnhancedInputComponent->BindAction(LMBAction, ETriggerEvent::Started, this, &ATbfPlayerController::LeftMousePressedAction);
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

	bIsPaused = !bIsPaused;
	UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);
	//TODO: if bIsPaused is true, open the pause widget, else close the pause widget
	
}

void ATbfPlayerController::Draw(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("Draw Pressed"));
	if (UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UE_LOG(LogTemp, Error, TEXT("Game Instance Found"));
		if (GI->bIsPlayerOneTurn)
		{
			//if player is in Draw State Call Draw Function on Player Character
			// Get Player Character
			if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
			{
				UE_LOG(LogTemp, Error, TEXT("Player Found"));
				// if player is in Draw State Call Draw Function on Player Character
				if (PlayerCharacter->CurrentState == ETbfPlayerState::Draw)
				{
					UE_LOG(LogTemp, Error, TEXT("Player State In Draw"));
					PlayerCharacter->DrawCard();
				}
			}
		}
	}
}

void ATbfPlayerController::Move(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("Move Pressed"));
	if (UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UE_LOG(LogTemp, Error, TEXT("Game Instance Found"));
		if (GI->bIsPlayerOneTurn)
		{
			//Get Player Character
			if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
			{
				// if player is in Main One Or Main Two State Call Move Card To Cell Location Function on Player Character
				if (PlayerCharacter->CurrentState == ETbfPlayerState::Main)
				{
					// Set the card to bOpponentCanSee to true
					if (PlayerCharacter->SelectedCard && PlayerCharacter->TargetedCell)
					{
						PlayerCharacter->PlaySelectedCard();
					}
				}
			}
		}
	}
}

void ATbfPlayerController::Activate(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("Activate Pressed"));
	if (UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UE_LOG(LogTemp, Error, TEXT("Game Instance Found"));
		if (GI->bIsPlayerOneTurn)
		{
			//Get Player Character
			////if player is in Main One Or Main Two State Call Activate Function to Activate the Selected Card On Field
			if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
			{
				// if player is in Main One Or Main Two State Call Activate Function to Activate the Selected Card On Field
				if (PlayerCharacter->CurrentState == ETbfPlayerState::Main)
				{
					if (PlayerCharacter->SelectedCard)
					{
						PlayerCharacter->ActivateSelectedCard();
					}
				}
			}
		}
	}
}

void ATbfPlayerController::Attack(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Attack Pressed"));
				
	//Get Player Character
	if (UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UE_LOG(LogTemp, Error, TEXT("Game Instance Found"));
		if (GI->bIsPlayerOneTurn)
		{
			//if player is in Battle State Call Attack Function on the SelectedUnit and pass the Selected TargetUnit as a Parameter to Attack
			if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
			{
				// if player is in Battle State Call Attack Function on the SelectedUnit and pass the Selected TargetUnit as a Parameter to Attack
				if (PlayerCharacter->CurrentState == ETbfPlayerState::Battle)
				{
					if (PlayerCharacter->SelectedUnit)
					{
						//PlayerCharacter->SelectedUnit->Attack(PlayerCharacter->SelectedTargetUnit);
						GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Attacking Targeted Unit/Boss"));
						PlayerCharacter->PlaySelectedUnitBattle();
				
					}
				}
			}
		}
	}
	
}

void ATbfPlayerController::NextState(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Next State Pressed"));
	if (UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UE_LOG(LogTemp, Error, TEXT("Game Instance Found"));
		if (GI->bIsPlayerOneTurn)
		{
			if (ATbfCharacterPlayer* PlayerCharacter = Cast<ATbfCharacterPlayer>(GetCharacter()))
			{
				UE_LOG(LogTemp, Error, TEXT("Player Found"));
				PlayerCharacter->GoToNextState();
			}
		}
	}
}

void ATbfPlayerController::LeftMousePressedAction(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("LMB Pressed"));
	// Select any actor that is Highlighted
	if (ThisActor)
	{
		Cast<ISelectionInterface>(ThisActor)->SelectActor();
	}
}
