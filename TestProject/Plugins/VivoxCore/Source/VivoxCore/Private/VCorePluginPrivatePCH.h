// Copyright (C) 2017-2021 eelDev

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/ConfigCacheIni.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Async/TaskGraphInterfaces.h"
#include "Containers/Ticker.h"
#include "TimerManager.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Engine/LocalPlayer.h>
#include <Misc/MessageDialog.h>
#include <Engine/GameInstance.h>
#include <Interfaces/IPluginManager.h>

#if !DISABLE_VIVOXCORE
#include <VoiceChatResult.h>
#include <VivoxVoiceChat.h>
#include <VoiceChatResult.h>
#include <Online.h>
#include <vivoxclientapi/util.h>
#endif