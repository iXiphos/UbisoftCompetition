// Copyright (C) 2017-2021 eelDev

#include "VCoreAsync.h"
#include "VCoreModule.h"
#include "VCorePluginPrivatePCH.h"

void UVivoxCoreAsyncAction::Activate()
{
	LogVerbose("");

	Super::Activate();
}

void UVivoxCoreAsyncAction::SetReadyToDestroy()
{
	LogVerbose("Task Duration: %f", m_TaskDuration);

	Super::SetReadyToDestroy();
}

bool UVivoxCoreAsyncAction::Tick(float deltaTime)
{
	if (HasAnyFlags(RF_ClassDefaultObject) || !HasAnyFlags(RF_StrongRefOnFrame))
	{
		return false;
	}

	m_TaskDuration += deltaTime;

	if (m_TaskDuration >= 10.0f)
	{
		SetReadyToDestroy();
	}

	return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxConnect
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UVivoxConnect* UVivoxConnect::Connect(UObject* WorldContextObject)
{
	LogVerbose("");

	auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	auto* AsyncObject = NewObject<UVivoxConnect>();
	AsyncObject->RegisterWithGameInstance(GameInstance);
	AsyncObject->m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(GameInstance);

	return AsyncObject;
}

void UVivoxConnect::Activate()
{
	LogVerbose("");
#if !DISABLE_VIVOXCORE
	m_VivoxSubsystem->m_VivoxChat->Connect(FOnVoiceChatConnectCompleteDelegate::CreateUObject(this, &UVivoxConnect::HandleCallback));
#endif
}

template <typename... TArgs>
void UVivoxConnect::HandleCallback(TArgs&&... args)
{
	LogVerbose("");

	OnCallback.Broadcast(Forward<TArgs>(args)...);
	SetReadyToDestroy();
}

UVivoxLogin* UVivoxLogin::Login(UObject* WorldContextObject, int32 userId, FString playerName, FString credentials)
{
	LogVerbose("");

	auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	auto* AsyncObject = NewObject<UVivoxLogin>();
	AsyncObject->RegisterWithGameInstance(GameInstance);
	AsyncObject->m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(GameInstance);
	AsyncObject->m_UserId = userId;
	AsyncObject->m_PlayerName = playerName;
	AsyncObject->m_Credentials = credentials;

	return AsyncObject;
}

void UVivoxLogin::Activate()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	m_VivoxSubsystem->m_VivoxChat->Login(m_UserId, m_PlayerName, m_Credentials, FOnVoiceChatLoginCompleteDelegate::CreateUObject(this, &UVivoxLogin::HandleCallback));
#endif
}

template <typename... TArgs>
void UVivoxLogin::HandleCallback(TArgs&&... args)
{
	LogVerbose("");

	OnCallback.Broadcast(Forward<TArgs>(args)...);
	SetReadyToDestroy();
}

UVivoxDisconnect* UVivoxDisconnect::Disconnect(UObject* WorldContextObject)
{
	LogVerbose("");

	auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	auto* AsyncObject = NewObject<UVivoxDisconnect>();
	AsyncObject->RegisterWithGameInstance(GameInstance);
	AsyncObject->m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(GameInstance);

	return AsyncObject;
}

void UVivoxDisconnect::Activate()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	m_VivoxSubsystem->m_VivoxChat->Disconnect(FOnVoiceChatDisconnectCompleteDelegate::CreateUObject(this, &UVivoxDisconnect::HandleCallback));
#endif
}

template <typename... TArgs>
void UVivoxDisconnect::HandleCallback(TArgs&&... args)
{
	LogVerbose("");

	OnCallback.Broadcast(Forward<TArgs>(args)...);
	SetReadyToDestroy();
}

UVivoxLogout* UVivoxLogout::Logout(UObject* WorldContextObject)
{
	LogVerbose("");

	auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	auto* AsyncObject = NewObject<UVivoxLogout>();
	AsyncObject->RegisterWithGameInstance(GameInstance);
	AsyncObject->m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(GameInstance);

	return AsyncObject;
}

void UVivoxLogout::Activate()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	m_VivoxSubsystem->m_VivoxChat->Logout(FOnVoiceChatLogoutCompleteDelegate::CreateUObject(this, &UVivoxLogout::HandleCallback));
#endif
}

template <typename... TArgs>
void UVivoxLogout::HandleCallback(TArgs&&... args)
{
	LogVerbose("");

	OnCallback.Broadcast(Forward<TArgs>(args)...);
	SetReadyToDestroy();
}

UVivoxJoinChannel* UVivoxJoinChannel::JoinChannel(UObject* WorldContextObject, FString channelName, FString channelCredentials, EVivoxVoiceChatChannelType channelType, FVivoxVoiceChatChannel3dProperties channel3dProperties)
{
	LogVerbose("");

	auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	auto* AsyncObject = NewObject<UVivoxJoinChannel>();
	AsyncObject->RegisterWithGameInstance(GameInstance);
	AsyncObject->m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(GameInstance);
	AsyncObject->m_ChannelName = channelName;
	AsyncObject->m_ChannelCredentials = channelCredentials;
	AsyncObject->m_ChannelType = channelType;
	AsyncObject->m_Channel3dProperties = channel3dProperties;

	return AsyncObject;
}

void UVivoxJoinChannel::Activate()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	FVoiceChatChannel3dProperties Properties;
	Properties.AttenuationModel = static_cast<EVoiceChatAttenuationModel>(m_Channel3dProperties.AttenuationModel);
	Properties.MaxDistance = m_Channel3dProperties.MaxDistance;
	Properties.MinDistance = m_Channel3dProperties.MinDistance;
	Properties.Rolloff = m_Channel3dProperties.Rolloff;

	m_VivoxSubsystem->m_VivoxChat->JoinChannel(m_ChannelName, m_ChannelCredentials, static_cast<EVoiceChatChannelType>(m_ChannelType), FOnVoiceChatChannelJoinCompleteDelegate::CreateUObject(this, &UVivoxJoinChannel::HandleCallback), Properties);
#endif
}

template <typename... TArgs>
void UVivoxJoinChannel::HandleCallback(TArgs&&... args)
{
	LogVerbose("");

	OnCallback.Broadcast(Forward<TArgs>(args)...);
	SetReadyToDestroy();
}

UVivoxLeaveChannel* UVivoxLeaveChannel::LeaveChannel(UObject* WorldContextObject, FString channelName)
{
	LogVerbose("");

	auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	auto* AsyncObject = NewObject<UVivoxLeaveChannel>();
	AsyncObject->RegisterWithGameInstance(GameInstance);
	AsyncObject->m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(GameInstance);
	AsyncObject->m_ChannelName = channelName;

	return AsyncObject;
}

void UVivoxLeaveChannel::Activate()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	m_VivoxSubsystem->m_VivoxChat->LeaveChannel(m_ChannelName, FOnVoiceChatChannelLeaveCompleteDelegate::CreateUObject(this, &UVivoxLeaveChannel::HandleCallback));
#endif
}

template <typename... TArgs>
void UVivoxLeaveChannel::HandleCallback(TArgs&&... args)
{
	LogVerbose("");

	OnCallback.Broadcast(Forward<TArgs>(args)...);
	SetReadyToDestroy();
}