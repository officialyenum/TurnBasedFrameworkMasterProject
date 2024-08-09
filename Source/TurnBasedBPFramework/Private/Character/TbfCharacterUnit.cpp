// Copyright Chukwuyenum Opone @officialyenum


#include "Character/TbfCharacterUnit.h"

#include "AbilitySystem/TbfAbilitySystemComponent.h"
#include "AbilitySystem/TbfAttributeSet.h"
#include "Actor/CardBase.h"
#include "Actor/TbfGridCell.h"
#include "AI/TbfUnitAIController.h"
#include "Character/TbfCharacter.h"
#include "Game/TbfGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TurnBasedBPFramework/TurnBasedBPFramework.h"


// Sets default values
ATbfCharacterUnit::ATbfCharacterUnit()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTbfAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UTbfAttributeSet>("AttributeSet");

	
	// Set AIControllerClass to ATbfAIController
	AIControllerClass = ATbfUnitAIController::StaticClass();
}

UBehaviorTree* ATbfCharacterUnit::GetBehaviorTree() const
{
	return Tree;
}

void ATbfCharacterUnit::HighlightActor()
{
	if (!bUnitIsSelected)
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	}
}

void ATbfCharacterUnit::UnHighlightActor()
{
	if (!bUnitIsSelected)
	{
		GetMesh()->SetRenderCustomDepth(false);
	}
}

void ATbfCharacterUnit::SelectActor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT(" %s Clicked"), *UnitInfo.Name.ToString()));
		
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI->bIsPlayerOneTurn && !bUnitIsSelected)
	{
		ECellType CellType = Cast<ATbfGridCell>(CellOccupied)->CellType;
		if (CellType == ECellType::PlayerOneUnit || CellType == ECellType::PlayerOneSpell)
		{
			if (GI->PlayerOne)
			{
				if (GI->PlayerOne->SelectedUnit)
				{
					GI->PlayerOne->SelectedUnit->UnSelectActor();
				}
				GI->PlayerOne->SelectedUnit = this;
			}
			bUnitIsSelected = true;
			GetMesh()->SetRenderCustomDepth(true);
			GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Actor Selected %s"), *UnitInfo.Name.ToString()));
		}else
		{
			if (GI->PlayerOne)
			{
				if (GI->PlayerOne->TargetedUnit)
				{
					GI->PlayerOne->TargetedUnit->UnSelectActor();
				}
				GI->PlayerOne->TargetedUnit = this;
			}
			bUnitIsSelected = true;
			GetMesh()->SetRenderCustomDepth(true);
			GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Actor Selected %s"), *UnitInfo.Name.ToString()));
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Player One Selected Unit Name %s"), *UnitInfo.Name.ToString()));
	}
}

void ATbfCharacterUnit::UnSelectActor()
{
	bUnitIsSelected = false;
	GetMesh()->SetRenderCustomDepth(false);
	UTbfGameInstance* GI = Cast<UTbfGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ECellType CellType = Cast<ATbfGridCell>(CellOccupied)->CellType;
	if (CellType == ECellType::PlayerOneUnit || CellType == ECellType::PlayerOneSpell)
	{
		if (GI->PlayerOne->SelectedUnit == this)
		{
			GI->PlayerOne->SelectedUnit = nullptr;
		}
	}
	else
	{
		if (GI->PlayerOne->TargetedUnit)
		{
			GI->PlayerOne->TargetedUnit = this;
		}
	}
}

void ATbfCharacterUnit::UpdateAttributeSet()
{
	Cast<UTbfAttributeSet>(AttributeSet)->SetAttack(UnitInfo.Attack);
	Cast<UTbfAttributeSet>(AttributeSet)->SetDefence(UnitInfo.Defence);
}

void ATbfCharacterUnit::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!AttributeSet)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
										 FString::Printf(TEXT("%s Recieved Damage %f"), *UnitInfo.Name.ToString(), Damage));
	
	UTbfAttributeSet* MyAttributes = Cast<UTbfAttributeSet>(AttributeSet);
	ATbfCharacterUnit* UnitCauser = Cast<ATbfCharacterUnit>(DamageCauser);
	ACardBase* CardCauser = Cast<ACardBase>(DamageCauser);

	if (UnitCauser && UnitCauser->AttributeSet && MyAttributes)
	{
		UTbfAttributeSet* CauserAttributes = Cast<UTbfAttributeSet>(UnitCauser->AttributeSet);

		if (UnitInfo.UnitState == ETbfUnitState::Attack)
		{
			float MyAttack = MyAttributes->GetAttack();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Applying Damage Value %f"), Damage));
	
			MyAttributes->SetAttack(Damage > MyAttack ? 0.f : MyAttack);
			CauserAttributes->SetAttack(Damage < MyAttack ? 0.f : Damage);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("New Causer Attack Value %f"), Damage));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("New Attacked Unit Value %f"), MyAttributes->GetAttack()));
		}
		else
		{
			float MyDefence = MyAttributes->GetDefence();
			MyAttributes->SetDefence(Damage > MyDefence ? 0.f : MyDefence);
			CauserAttributes->SetAttack(Damage < MyDefence ? 0.f : Damage);
		}
		// Check if UnitCauser health is less or zero, call a destroy function to kill
		// Check if My Attack or Defence is less or zero, call my destroy function to kill me
	}
	else if (CardCauser)
	{
		CardCauser->ApplyEffectToTarget(this, CardCauser->CardInfo.GameplayEffectClass);
	}
}


void ATbfCharacterUnit::SetToDefenseMode()
{
	UnitInfo.UnitState = ETbfUnitState::Defence;
}

void ATbfCharacterUnit::FindUnitToBattle()
{
	// If Unit is not in Attack Mode When this is Initiated return
	if(UnitInfo.UnitState == ETbfUnitState::Attack)
	{
		TargetUnit ? BattleOpponentUnit() : BattleOpponent();
	}
}

void ATbfCharacterUnit::BattleOpponent()
{
	// call apply effect or damage on targeted unit
	const UTbfGameInstance* GI = Cast<UTbfGameInstance>(GetGameInstance());
	ATbfCharacter* CharacterToAttack;
	if (GI->bIsPlayerOneTurn)
	{
		CharacterToAttack = GI->PlayerTwo;
	}
	else
	{
		CharacterToAttack = GI->PlayerOne;
	}
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Attacking Boss"));
	
					
	UTbfAttributeSet* MyAttributes = Cast<UTbfAttributeSet>(AttributeSet);
	AController* InstigatorController = GetInstigatorController();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Attacking Boss with Unit Info Damage Value %f"), UnitInfo.Attack));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Attacking Boss with Attribute Damage Value %f"), MyAttributes->GetAttack()));
	CharacterToAttack->HandleTakeAnyDamage(CharacterToAttack,MyAttributes->GetAttack(),DamageTypeClass,InstigatorController,this);
	//UGameplayStatics::ApplyDamage(
	//	CharacterToAttack, MyAttributes->GetAttack(),InstigatorController,this,DamageTypeClass);
	
}

void ATbfCharacterUnit::SetTargetUnit(ATbfCharacterUnit* NewTargetUnit)
{
	TargetUnit = NewTargetUnit;
}

void ATbfCharacterUnit::BattleOpponentUnit()
{
	// call apply effect or damage on opposing player
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("Attacking Targeted Unit"));
					
	UTbfAttributeSet* MyAttributes = Cast<UTbfAttributeSet>(AttributeSet);
	AController* InstigatorController = GetInstigatorController();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Attacking Targeted Unit with Damage Value %f"), MyAttributes->GetAttack()));
	
	TargetUnit->HandleTakeAnyDamage(TargetUnit,MyAttributes->GetAttack(),DamageTypeClass,InstigatorController,this);
	
	//UGameplayStatics::ApplyDamage(
	//	TargetUnit, MyAttributes->GetAttack(),InstigatorController,this,DamageTypeClass);
}

void ATbfCharacterUnit::HandleUnitDestroyed(AActor* DestroyedActor)
{
	if (ATbfCharacterUnit* DestroyedUnit = Cast<ATbfCharacterUnit>(DestroyedActor))
	{
		// Get the Unit Owner Either AI or Player, as they both are children of ATbfCharacter
		// Remove the destroyed unit from the UnitOnField array
		Cast<ATbfCharacter>(GetInstigator())->UnitOnField.Remove(DestroyedUnit);
		
		// update the UI or perform other necessary actions
		Cast<ATbfCharacter>(GetInstigator())->UpdateUIStat();
	}
}

void ATbfCharacterUnit::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	UpdateAttributeSet();
	OnTakeAnyDamage.AddDynamic(this, &ATbfCharacterUnit::HandleTakeAnyDamage);
}
