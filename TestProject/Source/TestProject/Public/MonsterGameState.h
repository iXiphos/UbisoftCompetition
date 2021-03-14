// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MonsterGameState.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AMonsterGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMonsterGameState();

	UPROPERTY(BlueprintReadOnly, Replicated)
	int min;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int sec;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	float countDownRate = 1.0;


	FTimerHandle UnusedHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateTimer();

	UFUNCTION(Server, Reliable)
	void CountDown();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void IncrementCountDownRate();

};
