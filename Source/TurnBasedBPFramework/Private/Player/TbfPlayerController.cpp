// Copyright Chukwuyenum Opone @officialyenum


#include "Player/TbfPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interactions/SelectionInterface.h"

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
}

void ATbfPlayerController::Draw(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Draw Pressed"));
}

void ATbfPlayerController::Move(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Move Pressed"));
}

void ATbfPlayerController::Activate(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Activate Pressed"));
}

void ATbfPlayerController::Attack(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Error, TEXT("Attack Pressed"));
}
