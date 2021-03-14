// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGameState.h"
#include "Net/UnrealNetwork.h"


AMonsterGameState::AMonsterGameState()
{
	sec = 59;
	min = 9;
}

void AMonsterGameState::BeginPlay()
{
	Super::BeginPlay();
	UpdateTimer();

}

// Called every frame
void AMonsterGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}


void AMonsterGameState::UpdateTimer()
{
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AMonsterGameState::CountDown, countDownRate, true);
}

void AMonsterGameState::CountDown_Implementation()
{
	sec--;
	if (sec <= 0)
	{
		sec = 59;
		min--;
	}

	if (min == -1)
	{
		GetWorld()->GetTimerManager().ClearTimer(UnusedHandle);
	}
	
}


void AMonsterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterGameState, sec);
	DOREPLIFETIME(AMonsterGameState, min);
}

void AMonsterGameState::IncrementCountDownRate_Implementation()
{
	countDownRate += 6;

	GetWorld()->GetTimerManager().ClearTimer(UnusedHandle);
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AMonsterGameState::CountDown, countDownRate, true);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT(countDownRate));

}