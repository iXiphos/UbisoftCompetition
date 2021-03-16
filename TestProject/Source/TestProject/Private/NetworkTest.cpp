// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkTest.h"

// Sets default values
ANetworkTest::ANetworkTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetworkTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetworkTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANetworkTest::Test()
{
	auto world = GetWorld();

	if (world != nullptr)
	{
		FString ip = FString("127.0.0.1");

		GEngine->SetClientTravel(world, *ip, ETravelType::TRAVEL_Relative);
	}
}

