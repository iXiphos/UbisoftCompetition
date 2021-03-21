// Copyright (C) 2017-2021 eelDev

#include "VCoreModule.h"
#include "VCorePluginPrivatePCH.h"

DEFINE_LOG_CATEGORY(LogVivoxCore);

#define LOCTEXT_NAMESPACE "FVivoxCoreModule"

void FVivoxCoreModule::StartupModule()
{
	TSharedPtr<IPlugin> PluginPtr = IPluginManager::Get().FindPlugin("VivoxCore");

	FString PluginVersion;

	if (PluginPtr)
	{
		PluginVersion = PluginPtr->GetDescriptor().VersionName;
	}

	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("Using VivoxCore Version: %s"), *PluginVersion);
	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));
}

void FVivoxCoreModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVivoxCoreModule, VivoxCore) 