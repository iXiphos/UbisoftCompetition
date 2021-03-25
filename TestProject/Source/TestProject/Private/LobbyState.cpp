// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyState.h"

void ALobbyState::NextScene_Implementation(const FString& URL, bool bAbsolute)
{
    UE_LOG(LogGameMode, Log, TEXT("ProcessServerTravel: %s"), *URL);
    UWorld* World = GetWorld();
    check(World);
    ENetMode NetMode = GetNetMode();

    World->ServerTravel(URL, true, true);
}