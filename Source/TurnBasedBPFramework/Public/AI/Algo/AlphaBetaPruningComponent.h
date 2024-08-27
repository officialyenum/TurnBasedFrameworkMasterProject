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
	
	FName ChooseBestCard(const FGameStateSim& GameState, int32 Depth, bool bIsField);
	
protected:
	virtual void BeginPlay() override;
	int32 GetBestCardIndex() { return BestCardIndex; };
	
private:
	int32 AlphaBetaPruning(FGameStateSim& GameState, int32 Depth, int32 Alpha, int32 Beta, bool bIsMaximizingPlayer, bool bIsField);
	
	void GeneratePossibleHands(FGameStateSim& GameState);
	
	void SimulatePlay(FGameStateSim& GameState, int32 CardIndex, bool bIsField, bool bIsPlayer);
	int32 EvaluateBoardState(const FGameStateSim& GameState) const;

	void RemoveFromCardArray(TArray<FTbfCardInfoSim>& Cards, const FTbfCardInfoSim& CardToRemove);
	
	void ApplyCardEffects(FGameStateSim& GameState, FTbfCardInfoSim& Card, bool bIsTrap, bool bIsPlayer);
	void SimulateCombat(FGameStateSim& GameState, FTbfCardInfoSim& Card, bool bIsPlayer);
	int32 BestCardIndex;
};
