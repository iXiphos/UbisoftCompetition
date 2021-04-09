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

		if (min <= 0)
		{
			UWorld* World = GetWorld();
			check(World);

			while (World->GetFirstPlayerController() != nullptr)
			{
				World->GetFirstPlayerController()->ClientTravel(FString("EndLevel"),ETravelType::TRAVEL_Absolute);
			}

			NextScene(FString("LobbyLevel"), true);
		}
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
	countDownRate -= 0.15;

	GetWorld()->GetTimerManager().ClearTimer(UnusedHandle);
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AMonsterGameState::CountDown, countDownRate, true);

	FString debugString = FString::SanitizeFloat(countDownRate);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *debugString);

}

void AMonsterGameState::NextScene_Implementation(const FString& URL, bool bAbsolute)
{
	UE_LOG(LogGameMode, Log, TEXT("ProcessServerTravel: %s"), *URL);
	UWorld* World = GetWorld();
	check(World);
	ENetMode NetMode = GetNetMode();

	World->ServerTravel(URL, true, true);
}