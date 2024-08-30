// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Actor/CardBase.h"
#include "UObject/Object.h"
#include "MonteCarloNode.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDBPFRAMEWORK_API UMonteCarloNode : public UObject
{
	GENERATED_BODY()
public:
	FGameStateSim State;
	TArray<UMonteCarloNode*> Children;
	int32 Wins;
	int32 Simulations;
	UMonteCarloNode* Parent;

	UMonteCarloNode() : Wins(0), Simulations(0), Parent(nullptr) {}

	float GetUCB1(float explorationParameter) const
	{
		if (Simulations == 0)
			return FLT_MAX; // Infinite value for unexplored nodes
		return (float)Wins / Simulations + explorationParameter * FMath::Sqrt(FMath::Loge((float)Parent->Simulations) / Simulations);
	}

	UMonteCarloNode* BestChild() const
	{
		float MaxValue = -FLT_MAX;
		UMonteCarloNode* BestChildNode = nullptr;
		for (auto Child : Children	)
		{
			float ChildValue = Child->GetUCB1(1.414);
			if (MaxValue < ChildValue)
			{
				MaxValue = FMath::Max(MaxValue, ChildValue);
				BestChildNode = Child;
			}
		}
		return BestChildNode;
	}

	bool IsLeaf() const { return Children.Num() == 0; }

	// Update method for backpropagation
	void Update(int32 Result)
	{
		Simulations++;
		if (Result > 0)
		{
			Wins++; // Increment wins if the result is favorable
		}
	}
};
