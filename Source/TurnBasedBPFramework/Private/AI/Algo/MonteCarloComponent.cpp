// Copyright Chukwuyenum Opone @officialyenum


#include "AI/Algo/MonteCarloComponent.h"

#include "Character/TbfCharacterAI.h"

UMonteCarloComponent::UMonteCarloComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMonteCarloComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMonteCarloComponent::ChooseBestAttack(FGameStateSim& GameState, int32 NumSimulations)
{
    int32 BestScore = INT32_MIN;
    OutAttackingUnitIndex = INDEX_NONE;
    OutTargetUnitIndex = INDEX_NONE;

    // Iterate over all units on the player's field
    for (int32 AttackingUnitIndex = 0; AttackingUnitIndex < GameState.UnitField.Num(); ++AttackingUnitIndex)
    {
        FTbfUnitInfoSim& AttackingUnit = GameState.UnitField[AttackingUnitIndex];

        // If there are no opponent units, attack opponent's life points
        if (GameState.OpponentUnitField.Num() == 0)
        {
            int32 Score = AttackingUnit.Attack * NumSimulations; // Direct attack to life points (simulated NumSimulations times)

            if (Score > BestScore)
            {
                BestScore = Score;
                OutAttackingUnitIndex = AttackingUnitIndex;
                OutTargetUnitIndex = INDEX_NONE; // Indicate attack on life points
            }
        }
        else
        {
            // Iterate over all units on the opponent's field
            for (int32 TargetUnitIndex = 0; TargetUnitIndex < GameState.OpponentUnitField.Num(); ++TargetUnitIndex)
            {
                FGameStateSim SimulatedState = GameState;

                // Run Monte Carlo Simulation for the current attack choice
                int32 Score = MonteCarloSimulation(SimulatedState, AttackingUnitIndex, TargetUnitIndex, NumSimulations);

                if (Score > BestScore)
                {
                    BestScore = Score;
                    OutAttackingUnitIndex = AttackingUnitIndex;
                    OutTargetUnitIndex = TargetUnitIndex;
                }
            }
        }
    }
}

int32 UMonteCarloComponent::MonteCarloSimulation(FGameStateSim& SimulatedState, int32 AttackingUnitIndex, int32 TargetUnitIndex, int32 NumSimulations)
{
    int32 TotalScore = 0;

    for (int32 i = 0; i < NumSimulations; ++i)
    {
        FGameStateSim SimulationCopy = SimulatedState;

        // If TargetUnitIndex is -1, attack life points
        if (TargetUnitIndex == INDEX_NONE)
        {
            SimulationCopy.OpponentLifePoints -= SimulationCopy.UnitField[AttackingUnitIndex].Attack;
        }
        else
        {
            // Simulate the attack on the target unit
            SimulateAttack(SimulationCopy, AttackingUnitIndex, TargetUnitIndex);
        }

        // Evaluate the board state after the simulated attack
        TotalScore += EvaluateBoardState(SimulationCopy);
    }

    // Return the average score from all simulations
    return TotalScore / NumSimulations;
}

void UMonteCarloComponent::SimulateAttack(FGameStateSim& GameState, int32 AttackingUnitIndex, int32 TargetUnitIndex)
{
    FTbfUnitInfoSim& AttackingUnit = GameState.UnitField[AttackingUnitIndex];
    FTbfUnitInfoSim& TargetUnit = GameState.OpponentUnitField[TargetUnitIndex];

    // Check if the target unit is already dead (shouldn't happen normally)
    if (TargetUnit.bIsDead || AttackingUnit.bIsDead)
    {
        return; // Skip further processing if the target is dead
    }

    // Determine the result of the attack
    if (AttackingUnit.Attack > TargetUnit.Defence)
    {
        // Target unit is destroyed, no retaliation
        TargetUnit.bIsDead = true;

        // Reduce the attacker's attack value by the target's defence
        AttackingUnit.Attack -= TargetUnit.Defence;
        AttackingUnit.Attack = FMath::Max(0, AttackingUnit.Attack); // Ensure attack doesn't go below 0

        // Target's defense is reduced to zero since it was destroyed
        TargetUnit.Defence = 0;
    }
    else
    {
        // Target survives, reduce target's defense
        TargetUnit.Defence -= AttackingUnit.Attack;
        TargetUnit.Defence = FMath::Max(0, TargetUnit.Defence); // Ensure defence doesn't go below 0

        // Reduce the attacker's attack value by the target's initial defense
        AttackingUnit.Attack = FMath::Max(0, AttackingUnit.Attack - TargetUnit.Defence); // Ensure attack doesn't go below 0

        // Retaliation: Target unit attacks back only if it is still alive
        if (TargetUnit.Attack > 0 && !AttackingUnit.bIsDead)
        {
            AttackingUnit.Defence -= TargetUnit.Attack;
            AttackingUnit.Defence = FMath::Max(0, AttackingUnit.Defence); // Ensure defence doesn't go below 0

            // If the attacker survives, optionally reduce the target's attack after retaliation
            if (AttackingUnit.Defence > 0)
            {
                // Optional: reduce the target's attack if needed based on your game mechanics
                TargetUnit.Attack = FMath::Max(0, TargetUnit.Attack - AttackingUnit.Defence);
            }

            // Check if the attacking unit dies after the retaliation
            if (AttackingUnit.Defence <= 0)
            {
                AttackingUnit.bIsDead = true;
            }
        }
    }
}

int32 UMonteCarloComponent::EvaluateBoardState(const FGameStateSim& GameState) const
{
    int32 Value = 0;

    // Add value based on life points
    Value += GameState.LifePoints - GameState.OpponentLifePoints;

    // Add value based on the number and strength of units on the field
    for (const FTbfUnitInfoSim& Unit : GameState.UnitField)
    {
        if (!Unit.bIsDead)
        {
            Value += Unit.Attack + Unit.Defence;
        }
    }

    // Subtract value based on opponent's units
    for (const FTbfUnitInfoSim& OpponentUnit : GameState.OpponentUnitField)
    {
        if (!OpponentUnit.bIsDead)
        {
            Value -= OpponentUnit.Attack + OpponentUnit.Defence;
        }
    }

    return Value;
}
