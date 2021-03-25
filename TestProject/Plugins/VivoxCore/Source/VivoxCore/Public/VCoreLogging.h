// Copyright (C) 2017-2021 eelDev

DECLARE_LOG_CATEGORY_EXTERN(LogVivoxCore, Log, All);

#define LogDebug(format, ...) UE_LOG(LogVivoxCore, Log, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVerbose(format, ...) UE_LOG(LogVivoxCore, Verbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVeryVerbose(format, ...) UE_LOG(LogVivoxCore, VeryVerbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOG(LogVivoxCore, Error, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOG(LogVivoxCore, Warning, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)