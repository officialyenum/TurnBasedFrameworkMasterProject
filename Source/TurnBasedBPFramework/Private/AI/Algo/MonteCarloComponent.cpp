// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/MonteCarloComponent.h"

#include "Character/TbfCharacterAI.h"

UMonteCarloComponent::UMonteCarloComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    OutAttackingUnitIndex = INDEX_NONE;
    OutTargetUnitIndex = INDEX_NONE;
}

void UMonteCarloComponent::BeginPlay()
{
    Super::BeginPlay();
}

FName UMonteCarloComponent::ChooseBestAttackingUnit(const FGameStateSim& GameState, int32 NumSimulations)
{
    UMonteCarloNode* Root = NewObject<UMonteCarloNode>();
    Root->State = GameState;

    for (int32 i = 0; i < NumSimulations; ++i)
    {
        UMonteCarloNode* SelectedNode = TreePolicy(Root);
        int32 SimulationResult = DefaultPolicy(SelectedNode->State);
        Backpropagate(SelectedNode, SimulationResult);
    }

    UMonteCarloNode* BestChild = Root->BestChild();
    if (BestChild && BestChild->Parent)
    {
        for (int32 i = 0; i < GameState.UnitField.Num(); ++i)
        {
            if (!GameState.UnitField[i].bIsDead && !BestChild->State.UnitField[i].bIsDead)
            {
                OutAttackingUnitIndex = i;
                break;
            }
        }

        const FTbfUnitInfoSim& BestUnit = GameState.UnitField[OutAttackingUnitIndex];
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Best Unit: %s"), *BestUnit.Name.ToString()));
        return BestUnit.Name;
    }
    return FName();
}

FName UMonteCarloComponent::ChooseBestTargetUnit(const FGameStateSim& GameState, int32 NumSimulations)
{
    UMonteCarloNode* Root = NewObject<UMonteCarloNode>();
    Root->State = GameState;

    for (int32 i = 0; i < NumSimulations; ++i)
    {
        UMonteCarloNode* SelectedNode = TreePolicy(Root);
        int32 SimulationResult = DefaultPolicy(SelectedNode->State);
        Backpropagate(SelectedNode, SimulationResult);
    }

    UMonteCarloNode* BestChild = Root->BestChild();
    if (BestChild && BestChild->Parent)
    {
        for (int32 i = 0; i < GameState.OpponentUnitField.Num(); ++i)
        {
            if (!GameState.OpponentUnitField[i].bIsDead && !BestChild->State.OpponentUnitField[i].bIsDead)
            {
                OutTargetUnitIndex = i;
                break;
            }
        }

        const FTbfUnitInfoSim& BestTarget = GameState.OpponentUnitField[OutTargetUnitIndex];
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Best Target Unit: %s"), *BestTarget.Name.ToString()));
        return BestTarget.Name;
    }
    return FName();
}

UMonteCarloNode* UMonteCarloComponent::TreePolicy(UMonteCarloNode* Root)
{
    UMonteCarloNode* Node = Root;
    while (!Node->IsLeaf())
    {
        if (Node->Children.Num() < Node->State.UnitField.Num())
        {
            return Expand(Node);
        }
        else
        {
            Node = Node->BestChild();
        }
    }
    return Node;
}

UMonteCarloNode* UMonteCarloComponent::Expand(UMonteCarloNode* Node)
{
    for (int32 AttackingUnitIndex = 0; AttackingUnitIndex < Node->State.UnitField.Num(); ++AttackingUnitIndex)
    {
        FTbfUnitInfoSim& AttackingUnit = Node->State.UnitField[AttackingUnitIndex];
        if (AttackingUnit.bIsDead)
        {
            continue; // Skip dead units
        }

        for (int32 TargetUnitIndex = 0; TargetUnitIndex < Node->State.OpponentUnitField.Num(); ++TargetUnitIndex)
        {
            FTbfUnitInfoSim& TargetUnit = Node->State.OpponentUnitField[TargetUnitIndex];
            if (TargetUnit.bIsDead)
            {
                continue; // Skip dead units
            }

            // Create a new child node for this move
            UMonteCarloNode* ChildNode = NewObject<UMonteCarloNode>();
            ChildNode->State = Node->State;

            // Simulate the attack using the centralized function
            SimulateAttack(ChildNode->State, AttackingUnitIndex, TargetUnitIndex, false);

            ChildNode->Parent = Node;
            Node->Children.Add(ChildNode);

            return ChildNode;
        }

        // Direct attack on life points
        if (AllUnitsDead(Node->State.OpponentUnitField))
        {
            UMonteCarloNode* ChildNode = NewObject<UMonteCarloNode>();
            ChildNode->State = Node->State;

            SimulateAttack(ChildNode->State, AttackingUnitIndex, INDEX_NONE, false);

            ChildNode->Parent = Node;
            Node->Children.Add(ChildNode);

            return ChildNode;
        }
    }

    return nullptr;
}

bool UMonteCarloComponent::AllUnitsDead(const TArray<FTbfUnitInfoSim>& UnitField)
{
    for (const FTbfUnitInfoSim& Unit : UnitField)
    {
        if (!Unit.bIsDead)
        {
            return false;
        }
    }
    return true;
}

int32 UMonteCarloComponent::DefaultPolicy(FGameStateSim& State)
{
    while (!IsTerminalState(State))
    {
        int32 AttackingUnitIndex = GetRandomLivingUnitIndex(State.UnitField);
        int32 TargetUnitIndex = (State.OpponentUnitField.Num() > 0) 
            ? GetRandomLivingUnitIndex(State.OpponentUnitField) 
            : INDEX_NONE;

        SimulateAttack(State, AttackingUnitIndex, TargetUnitIndex, false);

        if (!IsTerminalState(State))
        {
            int32 OpponentAttackingUnitIndex = GetRandomLivingUnitIndex(State.OpponentUnitField);
            int32 OpponentTargetUnitIndex = (State.UnitField.Num() > 0) 
                ? GetRandomLivingUnitIndex(State.UnitField) 
                : INDEX_NONE;

            SimulateAttack(State, OpponentAttackingUnitIndex, OpponentTargetUnitIndex, true);
        }
    }
    return (State.OpponentLifePoints <= 0) ? 1 : 0;
}

void UMonteCarloComponent::Backpropagate(UMonteCarloNode* Node, int32 Result)
{
    while (Node != nullptr)
    {
        Node->Simulations++;
        Node->Wins += Result;
        Node = Node->Parent;
    }
}

void UMonteCarloComponent::SimulateAttack(FGameStateSim& GameState, int32 AttackingUnitIndex, int32 TargetUnitIndex, bool isOpponent)
{
    if (AttackingUnitIndex == INDEX_NONE)
    {
        return; // No attacker
    }

    FTbfUnitInfoSim& AttackingUnit = isOpponent ? GameState.OpponentUnitField[AttackingUnitIndex] : GameState.UnitField[AttackingUnitIndex];

    if (TargetUnitIndex == INDEX_NONE)
    {
        if (isOpponent)
        {
            GameState.LifePoints -= AttackingUnit.Attack;
        }else
        {
            GameState.OpponentLifePoints -= AttackingUnit.Attack;
        }
    }
    else
    {

        FTbfUnitInfoSim& TargetUnit = isOpponent ? GameState.UnitField[TargetUnitIndex] : GameState.OpponentUnitField[TargetUnitIndex];
        if (TargetUnit.bIsDead || AttackingUnit.bIsDead)
        {
            return; // Skip dead units
        }

        int32 Damage = FMath::Max(0, AttackingUnit.Attack - TargetUnit.Defence);
        TargetUnit.Defence = FMath::Max(0, TargetUnit.Defence - AttackingUnit.Attack);

        if (Damage > 0)
        {
            TargetUnit.bIsDead = true;
        }
    }
}

bool UMonteCarloComponent::IsTerminalState(const FGameStateSim& State)
{
    return (State.LifePoints <= 0 || State.OpponentLifePoints <= 0);
}

int32 UMonteCarloComponent::GetRandomLivingUnitIndex(const TArray<FTbfUnitInfoSim>& UnitField)
{
    TArray<int32> LivingUnits;
    for (int32 i = 0; i < UnitField.Num(); ++i)
    {
        if (!UnitField[i].bIsDead)
        {
            LivingUnits.Add(i);
        }
    }

    if (LivingUnits.Num() > 0)
    {
        return LivingUnits[FMath::RandRange(0, LivingUnits.Num() - 1)];
    }

    return INDEX_NONE; // No living units
}


