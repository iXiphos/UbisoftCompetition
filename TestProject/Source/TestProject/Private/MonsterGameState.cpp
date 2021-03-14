// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGameState.h"
#include "Net/UnrealNetwork.h"


AMonsterGameState::AMonsterGameState()
{

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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

}


void AMonsterGameState::UpdateTimer()
{
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AMonsterGameState::CountDown, 1.0f, true);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
}


void AMonsterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterGameState, sec);
	DOREPLIFETIME(AMonsterGameState, min);
}