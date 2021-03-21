// Copyright (C) 2017-2021 eelDev

#include "VCoreSubsystem.h"
#include "VCoreModule.h"
#include "VCorePluginPrivatePCH.h"

void UVivoxCore::Initialize(FSubsystemCollectionBase& Collection)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	m_VivoxChat = IVoiceChat::Get();

	if (m_VivoxChat)
	{
		m_OnVivoxVoiceChatAvailableAudioDevicesChanged = m_VivoxChat->OnVoiceChatAvailableAudioDevicesChanged().AddUObject(this, &UVivoxCore::OnVoiceChatAvailableAudioDevicesChanged);
		m_OnVivoxVoiceChatConnected = m_VivoxChat->OnVoiceChatConnected().AddUObject(this, &UVivoxCore::OnVoiceChatConnected);
		m_OnVivoxVoiceChatDisconnected = m_VivoxChat->OnVoiceChatDisconnected().AddUObject(this, &UVivoxCore::OnVoiceChatDisconnected);
		m_OnVivoxVoiceChatReconnected = m_VivoxChat->OnVoiceChatReconnected().AddUObject(this, &UVivoxCore::OnVoiceChatReconnected);
		m_OnVoiceChatLoggedIn = m_VivoxChat->OnVoiceChatLoggedIn().AddUObject(this, &UVivoxCore::OnVoiceChatLoggedIn);
		m_OnVoiceChatLoggedOut = m_VivoxChat->OnVoiceChatLoggedOut().AddUObject(this, &UVivoxCore::OnVoiceChatLoggedOut);
		m_OnVoiceChatChannelJoined = m_VivoxChat->OnVoiceChatChannelJoined().AddUObject(this, &UVivoxCore::OnVoiceChatChannelJoined);
		m_OnVoiceChatChannelExited = m_VivoxChat->OnVoiceChatChannelExited().AddUObject(this, &UVivoxCore::OnVoiceChatChannelExited);
		m_OnVoiceChatCallStatsUpdated = m_VivoxChat->OnVoiceChatCallStatsUpdated().AddUObject(this, &UVivoxCore::OnVoiceChatCallStatsUpdated);
		m_OnVoiceChatPlayerAdded = m_VivoxChat->OnVoiceChatPlayerAdded().AddUObject(this, &UVivoxCore::OnVoiceChatPlayerAdded);
		m_OnVoiceCHatPlayerRemoved = m_VivoxChat->OnVoiceChatPlayerRemoved().AddUObject(this, &UVivoxCore::OnVoiceChatPlayerRemoved);
		m_OnVivoxVoiceChatPlayerTalkingUpdated = m_VivoxChat->OnVoiceChatPlayerTalkingUpdated().AddUObject(this, &UVivoxCore::OnVoiceChatPlayerTalkingUpdated);
		m_OnVoiceChatPlayerMuteUpdated = m_VivoxChat->OnVoiceChatPlayerMuteUpdated().AddUObject(this, &UVivoxCore::OnVoiceChatPlayerMuteUpdated);
		m_OnVivoxVoiceChatPlayerVolumeUpdated = m_VivoxChat->OnVoiceChatPlayerVolumeUpdated().AddUObject(this, &UVivoxCore::OnVoiceChatPlayerVolumeUpdated);
	}
#endif
}

void UVivoxCore::Deinitialize()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->OnVoiceChatAvailableAudioDevicesChanged().Remove(m_OnVivoxVoiceChatAvailableAudioDevicesChanged);
		m_VivoxChat->OnVoiceChatConnected().Remove(m_OnVivoxVoiceChatConnected);
		m_VivoxChat->OnVoiceChatDisconnected().Remove(m_OnVivoxVoiceChatDisconnected);
		m_VivoxChat->OnVoiceChatReconnected().Remove(m_OnVivoxVoiceChatReconnected);
		m_VivoxChat->OnVoiceChatLoggedIn().Remove(m_OnVoiceChatLoggedIn);
		m_VivoxChat->OnVoiceChatLoggedOut().Remove(m_OnVoiceChatLoggedOut);

		m_VivoxChat->OnVoiceChatChannelJoined().Remove(m_OnVoiceChatChannelJoined);
		m_VivoxChat->OnVoiceChatChannelExited().Remove(m_OnVoiceChatChannelExited);
		m_VivoxChat->OnVoiceChatCallStatsUpdated().Remove(m_OnVoiceChatCallStatsUpdated);

		m_VivoxChat->OnVoiceChatPlayerRemoved().Remove(m_OnVoiceChatPlayerAdded);
		m_VivoxChat->OnVoiceChatPlayerRemoved().Remove(m_OnVoiceCHatPlayerRemoved);

		m_VivoxChat->OnVoiceChatPlayerTalkingUpdated().Remove(m_OnVivoxVoiceChatPlayerTalkingUpdated);
		m_VivoxChat->OnVoiceChatPlayerMuteUpdated().Remove(m_OnVoiceChatPlayerMuteUpdated);
		m_VivoxChat->OnVoiceChatPlayerVolumeUpdated().Remove(m_OnVivoxVoiceChatPlayerVolumeUpdated);
	}
#endif
}

bool UVivoxCore::ValidateVivoxSettings()
{
	LogVerbose("");

	bool bIsEnable = false;
	GConfig->GetBool(TEXT("VoiceChat.Vivox"), TEXT("bEnabled"), bIsEnable, GEngineIni);
	if (!bIsEnable)
	{
		return false;
	}

	bool bSuccess = true;

	FString String;
	GConfig->GetString(TEXT("VoiceChat.Vivox"), TEXT("ServerUrl"), String, GEngineIni);
	if (String.IsEmpty())
	{
		LogWarning("[VoiceChat.Vivox] ServerUrl is not set in DefaultEngine.ini");
		bSuccess = false;
	}
	GConfig->GetString(TEXT("VoiceChat.Vivox"), TEXT("Domain"), String, GEngineIni);
	if (String.IsEmpty())
	{
		LogWarning("[VoiceChat.Vivox] Domain is not set in DefaultEngine.ini");
		bSuccess = false;
	}
	GConfig->GetString(TEXT("VoiceChat.Vivox"), TEXT("Issuer"), String, GEngineIni);
	if (String.IsEmpty())
	{
		LogWarning("[VoiceChat.Vivox] Issuer is not set in DefaultEngine.ini");
		bSuccess = false;
	}
	GConfig->GetString(TEXT("VoiceChat.Vivox"), TEXT("InsecureSecret"), String, GEngineIni);
	if (String.IsEmpty())
	{
		LogWarning("[VoiceChat.Vivox] InsecureSecret is not set in DefaultEngine.ini");
		bSuccess = false;
	}

	return bSuccess;
}

bool UVivoxCore::InitializeVivox()
{
	LogVerbose("");
#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		if (!m_VivoxChat->IsInitialized())
		{
			if (ValidateVivoxSettings())
			{
				return m_VivoxChat->Initialize();
			}
			else
			{
				FText TitleText = FText::FromString(FString("Warning: Vivox Voice Chat not configured!"));
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString("Voice Chat will not work properly until [VoiceChat.Vivox] is enabled in DefaultEngine.ini and your credential values are set correctly.")), &TitleText);
			}
		}
		else if (m_VivoxChat->IsInitialized())
		{
			return true;
		}
	}

#endif

	return false;
}

void UVivoxCore::UninitializeVivox()
{
	LogVerbose("");
#if !DISABLE_VIVOXCORE

	if (m_VivoxChat)
	{
		m_VivoxChat->Uninitialize();
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::Connect(const FOnVivoxConnectCallback& callback)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->Connect(FOnVoiceChatConnectCompleteDelegate::CreateLambda([this, callback](const FVoiceChatResult& Result)
			{
				callback.ExecuteIfBound(Result);
			}));
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::Disconnect(const FOnVivoxDisconnectCallback& callback)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->Disconnect(FOnVoiceChatDisconnectCompleteDelegate::CreateLambda([this, callback](const FVoiceChatResult& Result)
			{
				callback.ExecuteIfBound(Result);
			}));
	}
#endif // DISABLE_VIVOXCORE
}

bool UVivoxCore::IsConnecting() const
{
	LogVeryVerbose("");

	bool bResult = false;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		bResult = m_VivoxChat->IsConnecting();
	}
#endif // DISABLE_VIVOXCORE

	return bResult;
}

bool UVivoxCore::IsConnected() const
{
	LogVeryVerbose("");

	bool bResult = false;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		bResult = m_VivoxChat->IsConnected();
	}
#endif // DISABLE_VIVOXCORE

	return bResult;
}

void UVivoxCore::Login(int32 userId, FString playerName, FString credentials, const FOnVivoxLoginCallback& callback)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->Login(userId, playerName, credentials, FOnVoiceChatLoginCompleteDelegate::CreateLambda([this, callback](const FString& PlayerName, const FVoiceChatResult& Result)
			{
				callback.ExecuteIfBound(Result.ErrorDesc, Result);
			}));
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::Logout(const FOnVivoxLogoutCallback& callback)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->Logout(FOnVoiceChatLogoutCompleteDelegate::CreateLambda([this, callback](const FString& PlayerName, const FVoiceChatResult& Result)
			{
				callback.ExecuteIfBound(Result.ErrorDesc, Result);
			}));
	}
#endif // DISABLE_VIVOXCORE
}

bool UVivoxCore::IsLoggingIn() const
{
	LogVeryVerbose("");

	bool bResult = false;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		bResult = m_VivoxChat->IsLoggingIn();
	}
#endif // DISABLE_VIVOXCORE

	return bResult;
}

bool UVivoxCore::IsLoggedIn() const
{
	LogVeryVerbose("");

	bool bResult = false;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		bResult = m_VivoxChat->IsLoggedIn();
	}
#endif // DISABLE_VIVOXCORE

	return bResult;
}

FString UVivoxCore::GetLoggedInPlayerName() const
{
	LogVeryVerbose("");

	FString Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetLoggedInPlayerName();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

void UVivoxCore::BlockPlayers(const TArray<FString>& playerNames)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->BlockPlayers(playerNames);
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::UnblockPlayers(const TArray<FString>& playerNames)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->UnblockPlayers(playerNames);
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::JoinChannel(FString channelName, FString channelCredentials, EVivoxVoiceChatChannelType channelType, const FOnVivoxJoinChannelCallback& callback, FVivoxVoiceChatChannel3dProperties channel3dProperties)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		FVoiceChatChannel3dProperties Properties;
		Properties.AttenuationModel = static_cast<EVoiceChatAttenuationModel>(channel3dProperties.AttenuationModel);
		Properties.MaxDistance = channel3dProperties.MaxDistance;
		Properties.MinDistance = channel3dProperties.MinDistance;
		Properties.Rolloff = channel3dProperties.Rolloff;

		m_VivoxChat->JoinChannel(channelName, channelCredentials, static_cast<EVoiceChatChannelType>(channelType), FOnVoiceChatChannelJoinCompleteDelegate::CreateLambda([this, callback](const FString& ChannelName, const FVoiceChatResult& Result)
			{
				callback.ExecuteIfBound(ChannelName, Result);
			}), Properties);
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::LeaveChannel(FString channelName, const FOnVivoxLeaveChannelCallback& callback)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	m_VivoxChat->LeaveChannel(channelName, FOnVoiceChatChannelLeaveCompleteDelegate::CreateLambda([this, callback](const FString& ChannelName, const FVoiceChatResult& Result)
		{
			callback.ExecuteIfBound(ChannelName, Result);
		}));
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::Set3DPosition(const FString& channelName, const FVector& speakerPosition, const FVector& listenerPosition, const FVector& listenerForwardDirection, const FVector& listenerUpDirection)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->Set3DPosition(channelName, speakerPosition, listenerPosition, listenerForwardDirection, listenerUpDirection);
	}
#endif // DISABLE_VIVOXCORE
}

TArray<FString> UVivoxCore::GetChannels() const
{
	LogVeryVerbose("");

	TArray<FString> Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetChannels();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

TArray<FString> UVivoxCore::GetPlayersInChannel(FString channelName) const
{
	LogVeryVerbose("");

	TArray<FString> Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetPlayersInChannel(channelName);
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

EVivoxVoiceChatChannelType UVivoxCore::GetChannelType(FString channelName) const
{
	LogVeryVerbose("");

	EVivoxVoiceChatChannelType Result = EVivoxVoiceChatChannelType::Positional;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = static_cast<EVivoxVoiceChatChannelType>(m_VivoxChat->GetChannelType(channelName));
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

bool UVivoxCore::IsPlayerTalking(FString playerName)
{
	LogVerbose("");

	bool bResult = false;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		bResult = m_VivoxChat->IsPlayerTalking(playerName);
	}
#endif // DISABLE_VIVOXCORE

	return bResult;
}

void UVivoxCore::SetPlayerMuted(FString pPlayerName, bool bMuted)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->SetPlayerMuted(pPlayerName, bMuted);
	}
#endif // DISABLE_VIVOXCORE
}

bool UVivoxCore::IsPlayerMuted(FString playerName) const
{
	LogVeryVerbose("");

	bool bResult = false;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		bResult = m_VivoxChat->IsPlayerMuted(playerName);
	}
#endif // DISABLE_VIVOXCORE

	return bResult;
}

void UVivoxCore::SetPlayerVolume(FString playerName, float volume)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->SetPlayerVolume(playerName, volume);
	}
#endif // DISABLE_VIVOXCORE
}

float UVivoxCore::GetPlayerVolume(FString playerName) const
{
	LogVeryVerbose("");

	float Result = 0.0f;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetPlayerVolume(playerName);
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

void UVivoxCore::TransmitToAllChannels()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->TransmitToAllChannels();
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::TransmitToNoChannels()
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->TransmitToNoChannels();
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::TransmitToSpecificChannel(FString channelName)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->TransmitToSpecificChannel(channelName);
	}
#endif // DISABLE_VIVOXCORE
}

EVivoxVoiceChatTransmitMode UVivoxCore::GetTransmitMode() const
{
	LogVeryVerbose("");

	EVivoxVoiceChatTransmitMode Result = EVivoxVoiceChatTransmitMode::All;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = static_cast<EVivoxVoiceChatTransmitMode>(m_VivoxChat->GetTransmitMode());
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

FString UVivoxCore::GetTransmitChannel() const
{
	LogVeryVerbose("");

	FString Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetTransmitChannel();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

void UVivoxCore::SetAudioInputVolume(float volume)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->SetAudioInputVolume(volume);
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::SetAudioOutputVolume(float volume)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->SetAudioOutputVolume(volume);
	}
#endif // DISABLE_VIVOXCORE
}

float UVivoxCore::GetAudioInputVolume() const
{
	LogVeryVerbose("");

	float Result = 0.0f;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetAudioInputVolume();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

float UVivoxCore::GetAudioOutputVolume() const
{
	LogVeryVerbose("");

	float Result = 0.0f;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetAudioOutputVolume();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

void UVivoxCore::SetAudioInputDeviceMuted(bool bIsMuted)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->SetAudioInputDeviceMuted(bIsMuted);
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::SetAudioOutputDeviceMuted(bool bIsMuted)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		m_VivoxChat->SetAudioOutputDeviceMuted(bIsMuted);
	}
#endif // DISABLE_VIVOXCORE
}

float UVivoxCore::GetAudioInputDeviceMuted() const
{
	LogVeryVerbose("");

	float Result = 0.0f;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetAudioInputDeviceMuted();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

float UVivoxCore::GetAudioOutputDeviceMuted() const
{
	LogVeryVerbose("");

	float Result = 0.0f;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
		Result = m_VivoxChat->GetAudioOutputVolume();
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

TArray<FVivoxVoiceChatDeviceInfo> UVivoxCore::GetAvailableInputDeviceInfos() const
{
	LogVeryVerbose("");

	TArray<FVivoxVoiceChatDeviceInfo> Result;

#if !DISABLE_VIVOXCORE

	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		TArray<FVoiceChatDeviceInfo> Data = m_VivoxChat->GetAvailableInputDeviceInfos();

		for (auto& Element : Data)
		{
			Result.Add(Element);
		}
#else
		TArray<FString> AvailableInputDevices = m_VivoxChat->GetAvailableInputDevices();

		for (auto& Element : AvailableInputDevices)
		{
			Result.Add(FVivoxVoiceChatDeviceInfo(Element));
		}
#endif
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

TArray<FVivoxVoiceChatDeviceInfo> UVivoxCore::GetAvailableOutputDeviceInfos() const
{
	LogVeryVerbose("");

	TArray<FVivoxVoiceChatDeviceInfo> Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		TArray<FVoiceChatDeviceInfo> Data;
		Data = m_VivoxChat->GetAvailableOutputDeviceInfos();

		for (auto& Element : Data)
		{
			Result.Add(Element);
		}
#else
		TArray<FString> AvailableOutputDevices = m_VivoxChat->GetAvailableOutputDevices();

		for (auto& Element : AvailableOutputDevices)
		{
			Result.Add(FVivoxVoiceChatDeviceInfo(Element));
		}
#endif
	}

#endif // DISABLE_VIVOXCORE
	return Result;
}

void UVivoxCore::SetInputDevice(FString inputDevice)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE

#if ENGINE_MINOR_VERSION < 26
	if (m_VivoxChat)
	{
		m_VivoxChat->SetInputDevice(inputDevice);
	}
#else
	LogError("This function is DEPRECATED. Use SetInputDeviceId() instead");
#endif

#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::SetInputDeviceId(FString inputDeviceId)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		m_VivoxChat->SetInputDeviceId(inputDeviceId);
#else
		LogError("This function is not supported in this Engine Version.");
#endif
	}
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::SetOutputDevice(FString outputDevice)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
#if ENGINE_MINOR_VERSION < 26
	if (m_VivoxChat)
	{
		m_VivoxChat->SetOutputDevice(outputDevice);
	}
#else
	LogError("This function is DEPRECATED. Use SetInputDeviceId() instead");
#endif
#endif // DISABLE_VIVOXCORE
}

void UVivoxCore::SetOutputDeviceId(FString outputDeviceId)
{
	LogVerbose("");

#if !DISABLE_VIVOXCORE
#if ENGINE_MINOR_VERSION > 25
	if (m_VivoxChat)
	{
		m_VivoxChat->SetOutputDeviceId(outputDeviceId);
	}
#else
	LogError("This function is DEPRECATED. Use SetInputDevice() instead");
#endif
#endif // DISABLE_VIVOXCORE
}

FVivoxVoiceChatDeviceInfo UVivoxCore::GetInputDeviceInfo() const
{
	LogVeryVerbose("");

	FVivoxVoiceChatDeviceInfo Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		Result = m_VivoxChat->GetInputDeviceInfo();
#else
		Result = m_VivoxChat->GetInputDevice();
#endif
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

FVivoxVoiceChatDeviceInfo UVivoxCore::GetOutputDeviceInfo() const
{
	LogVeryVerbose("");

	FVivoxVoiceChatDeviceInfo Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		Result = m_VivoxChat->GetOutputDeviceInfo();
#else
		Result = m_VivoxChat->GetOutputDevice();
#endif
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

FVivoxVoiceChatDeviceInfo UVivoxCore::GetDefaultInputDeviceInfo() const
{
	LogVeryVerbose("");

	FVivoxVoiceChatDeviceInfo Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		Result = m_VivoxChat->GetDefaultInputDeviceInfo();
#else
		Result = m_VivoxChat->GetDefaultInputDevice();
#endif
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

FVivoxVoiceChatDeviceInfo UVivoxCore::GetDefaultOutputDeviceInfo() const
{
	LogVeryVerbose("");

	FVivoxVoiceChatDeviceInfo Result;

#if !DISABLE_VIVOXCORE
	if (m_VivoxChat)
	{
#if ENGINE_MINOR_VERSION > 25
		Result = m_VivoxChat->GetDefaultOutputDeviceInfo();
#else
		Result = m_VivoxChat->GetDefaultOutputDevice();
#endif
	}
#endif

	return Result;
}

FString UVivoxCore::GetSafePlayerName(FString baseName)
{
	LogVeryVerbose("");

	bool bDoHash = false;
#if !DISABLE_VIVOXCORE

	int32 VivoxSafePlayerLength = 35;
	FString VivoxIssuer;
	GConfig->GetString(TEXT("VoiceChat.Vivox"), TEXT("Issuer"), VivoxIssuer, GEngineIni);

	if (!VivoxIssuer.IsEmpty())
	{
		VivoxSafePlayerLength = 60 - VivoxIssuer.Len();
	}

	if (baseName.Len() >= VivoxSafePlayerLength)
	{
		bDoHash = true;
	}
	else
	{
		FString ValidCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890=+-_.!~()";
		int32 Loc;
		auto ConstItr = baseName.CreateConstIterator();

		while (baseName.IsValidIndex(ConstItr.GetIndex()))
		{
			if (!ValidCharacters.FindChar(baseName[ConstItr++.GetIndex()], Loc))
			{
				bDoHash = true;
				break;
			}
		}

	}
#endif // DISABLE_VIVOXCORE

	if (bDoHash)
	{
		return FMD5::HashAnsiString(*baseName);
	}
	else
	{
		return baseName;
	}
}

FString UVivoxCore::InsecureGetLoginToken(UObject* worldContextObject, FString playerName)
{
	LogVerbose("");
	check(worldContextObject);

	FString Result;

#if !DISABLE_VIVOXCORE
	if (UVivoxCore* m_VivoxSubsystem = UVivoxCore::Get(worldContextObject))
	{
		if (m_VivoxSubsystem->m_VivoxChat)
		{
			Result = m_VivoxSubsystem->m_VivoxChat->InsecureGetLoginToken(playerName);
		}
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

FString UVivoxCore::InsecureGetJoinToken(UObject* worldContextObject, FString channelName, EVivoxVoiceChatChannelType channelType, FVivoxVoiceChatChannel3dProperties channel3dProperties)
{
	LogVerbose("");

	FString Result;

#if !DISABLE_VIVOXCORE
	if (UVivoxCore* m_VivoxSubsystem = UVivoxCore::Get(worldContextObject))
	{
		if (m_VivoxSubsystem->m_VivoxChat)
		{
			FVoiceChatChannel3dProperties Properties;
			Properties.AttenuationModel = static_cast<EVoiceChatAttenuationModel>(channel3dProperties.AttenuationModel);
			Properties.MaxDistance = channel3dProperties.MaxDistance;
			Properties.MinDistance = channel3dProperties.MinDistance;
			Properties.Rolloff = channel3dProperties.Rolloff;

			Result = m_VivoxSubsystem->m_VivoxChat->InsecureGetJoinToken(channelName, static_cast<EVoiceChatChannelType>(channelType), Properties);
		}
	}
#endif // DISABLE_VIVOXCORE

	return Result;
}

#if !DISABLE_VIVOXCORE
void UVivoxCore::OnVoiceChatAvailableAudioDevicesChanged()
{
	OnVivoxVoiceChatAvailableAudioDevicesChanged.Broadcast();
}

void UVivoxCore::OnVoiceChatConnected()
{
	OnVivoxVoiceChatConnected.Broadcast();
}

void UVivoxCore::OnVoiceChatDisconnected(const FVoiceChatResult& result)
{
	OnVivoxVoiceChatDisconnected.Broadcast(result);
}

void UVivoxCore::OnVoiceChatReconnected()
{
	OnVivoxVoiceChatReconnected.Broadcast();
}

void UVivoxCore::OnVoiceChatLoggedIn(const FString& playerName)
{
	OnVivoxVoiceChatLoggedIn.Broadcast(playerName);
}

void UVivoxCore::OnVoiceChatLoggedOut(const FString& playerName)
{
	OnVivoxVoiceChatLoggedOut.Broadcast(playerName);
}

void UVivoxCore::OnVoiceChatChannelJoined(const FString& playerName)
{
	OnVivoxVoiceChatChannelJoined.Broadcast(playerName);
}

void UVivoxCore::OnVoiceChatChannelExited(const FString& channelName, const FVoiceChatResult& reason)
{
	OnVivoxVoiceChatChannelExited.Broadcast(channelName, reason);
}

void UVivoxCore::OnVoiceChatCallStatsUpdated(const FVoiceChatCallStats& callstats)
{
	OnVivoxVoiceChatCallStatsUpdated.Broadcast(callstats);
}

void UVivoxCore::OnVoiceChatPlayerRemoved(const FString& channelName, const FString& playerName)
{
	OnVivoxVoiceChatPlayerRemoved.Broadcast(channelName, playerName);
}

void UVivoxCore::OnVoiceChatPlayerAdded(const FString& channelName, const FString& playerName)
{
	OnVivoxVoiceChatPlayerAdded.Broadcast(channelName, playerName);
}

void UVivoxCore::OnVoiceChatPlayerTalkingUpdated(const FString& channelName, const FString& playerName, bool bIsTalking)
{
	OnVivoxVoiceChatPlayerTalkingUpdated.Broadcast(channelName, playerName, bIsTalking);
}

void UVivoxCore::OnVoiceChatPlayerMuteUpdated(const FString& channelName, const FString& playerName, bool bIsMuted)
{
	OnVivoxVoiceChatPlayerMuteUpdated.Broadcast(channelName, playerName, bIsMuted);
}

void UVivoxCore::OnVoiceChatPlayerVolumeUpdated(const FString& channelName, const FString& playerName, float volume)
{
	OnVivoxVoiceChatPlayerVolumeUpdated.Broadcast(channelName, playerName, volume);
}
#endif