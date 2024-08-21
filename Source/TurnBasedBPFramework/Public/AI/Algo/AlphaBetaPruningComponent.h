// Copyright Chukwuyenum Opone @officialyenum

#pragma once

#include "CoreMinimal.h"
#include "Character/TbfCharacterAI.h"
#include "Components/ActorComponent.h"
#include "AlphaBetaPruningComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURNBASEDBPFRAMEWORK_API UAlphaBetaPruningComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAlphaBetaPruningComponent();
	
	FName ChooseBestCardInHand(const FGameStateSim& GameState, int32 Depth);
	FName ChooseBestCardInField(const FGameStateSim& GameState, int32 Depth);

protected:
	virtual void BeginPlay() override;

private:
	int32 AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer, bool bIsField);

	void GeneratePossibleHands(FGameStateSim& GameState);
	void SimulatePlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField);
	
	void ApplySpellOrTrapEffect(FGameStateSim& GameState, const FTbfCardInfoSim& CardToPlay, bool bIsOpponent);
	void SimulateOpponentPlayCard(FGameStateSim& GameState, int32 CardIndex, bool bIsField);
	int32 EvaluateBoardState(const FGameStateSim& GameState) const;
};
