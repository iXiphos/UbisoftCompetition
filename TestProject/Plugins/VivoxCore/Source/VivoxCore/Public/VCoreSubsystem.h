// Copyright (C) 2017-2021 eelDev

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VCoreTypes.h"
#include "VCoreSubsystem.generated.h"

class IVoiceChat;

UCLASS()
class VIVOXCORE_API UVivoxCore : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "VivoxCore", WorldContext = "worldContextObject"), Category = "VivoxCore")
	static UVivoxCore* Get(UObject* worldContextObject) 
	{
		if (worldContextObject && worldContextObject->GetWorld())
		{
			if (UVivoxCore* m_VivoxSubsystem = UGameInstance::GetSubsystem<UVivoxCore>(worldContextObject->GetWorld()->GetGameInstance()))
			{
				return m_VivoxSubsystem;
			}
		}

		return nullptr;
	}
public:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	/**
	 * Delegate broadcast whenever the available audio devices change. Any cached values from GetAvailableInputDevices or GetAvailableOutputDevices should be discarded and requeried
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatAvailableAudioDevicesChangedDelegate OnVivoxVoiceChatAvailableAudioDevicesChanged;

	/**
	 * Delegate triggered when we are connected to voice chat
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatConnectedDelegate OnVivoxVoiceChatConnected;

	/**
	 * Delegate triggered when we are disconnected from voice chat
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatDisconnectedDelegate OnVivoxVoiceChatDisconnected;

	/**
	 * Delegate triggered when we are reconnected to voice chat in cases where the underlying implementation disconnected
	 * and then reconnected. This is most commonly seen when an application resumes after being suspended. If the disconnect
	 * is unexpected, OnVoiceChatDisconnected will be called instead.
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatReconnectedDelegate OnVivoxVoiceChatReconnected;

	/**
	 * Delegate triggered when we are logged in to voice chat
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatLoggedInDelegate OnVivoxVoiceChatLoggedIn;

	/**
	 * Delegate triggered when we are logged out from voice chat
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatLoggedOutDelegate OnVivoxVoiceChatLoggedOut;

	/**
	 * Delegate triggered when we join a voice channel
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatChannelJoinedDelegate OnVivoxVoiceChatChannelJoined;

	/**
	 * Delegate triggered when we leave a voice channel
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatChannelExitedDelegate OnVivoxVoiceChatChannelExited;

	/**
	 * Delegate triggered when a call is ended, providing the stats for the call.
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatCallStatsUpdatedDelegate OnVivoxVoiceChatCallStatsUpdated;

	/**
	 * Delegate triggered when a player is added to a channel
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatPlayerAddedDelegate OnVivoxVoiceChatPlayerAdded;

	/**
	 * Delegate triggered when a player is removed from a channel
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatPlayerRemovedDelegate OnVivoxVoiceChatPlayerRemoved;

	/**
	 * Delegate triggered when a player's talking state is updated
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatPlayerTalkingUpdatedDelegate OnVivoxVoiceChatPlayerTalkingUpdated;

	/**
	 * Delegate triggered when a player's muted state is updated, usually as a result of calling SetPlayerMuted
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatPlayerMuteUpdatedDelegate OnVivoxVoiceChatPlayerMuteUpdated;

	/**
	 * Delegate triggered when a player's volume is updated, usually as a result of calling SetPlayerVolume
	 */
	UPROPERTY(BlueprintAssignable)
		FOnVivoxVoiceChatPlayerVolumeUpdatedDelegate OnVivoxVoiceChatPlayerVolumeUpdated;
public:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		METHODS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

	/**
	 * Check if all the required Vivox Settings are set in DefaultEngine.ini
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		static bool ValidateVivoxSettings();

	/**
	 * Initialize VoiceChat
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool InitializeVivox();
	 
	/**
	 * Uninitialize VoiceChat
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void UninitializeVivox();

	/**
	 * Connect to a voice server
	 *
	 * @param Delegate delegate called once connect completes
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (AutoCreateRefTerm = "callback"))
		void Connect(const FOnVivoxConnectCallback& callback);

	/**
	 * Disconnect from a voice server
	 *
	 * @param Delegate delegate called once disconnect completes
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (AutoCreateRefTerm = "callback"))
		void Disconnect(const FOnVivoxDisconnectCallback& callback);

	/**
	* Are we connecting to the voice server?
	*
	* @return true if we are connecting to the voice server
	*/
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool IsConnecting() const;

	/**
	 * Are we connected to the voice server?
	 *
	 * @return true if we are connected to the voice server
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool IsConnected() const;

	/**
	 * Login to the connected voice server
	 *
	 * @param userId Platform user Id for the player logging in
	 * @param playerName Name of the account to login to
	 * @param credentials Token or password
	 * @param callback delegate called once login completes
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (AutoCreateRefTerm = "callback"))
		void Login(int32 userId, FString playerName, FString credentials, const FOnVivoxLoginCallback& callback);

	/**
	 * Logout from the connected voice server
	 *
	 * @param Delegate delegate called once logout completes
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (AutoCreateRefTerm = "callback"))
		void Logout(const FOnVivoxLogoutCallback& callback);

	/**
	 * Are we logging in?
	 *
	 * @return true if we are logging in to the voice server
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool IsLoggingIn() const;

	/**
	 * Are we logged in?
	 *
	 * @return true if we are logged in to the voice server
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool IsLoggedIn() const;

	/**
	 * Get the player name used to log in
	 *
	 * @return player name used to log in
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		FString GetLoggedInPlayerName() const;

	/**
	 * Block communication with players
	 *
	 * Adds the PlayerNames to the block list, muting them in all current and subsequently joined channels.
	 * A block for a player persists until either:
	 *		- The local user logs out (e.g. by calling Logout/Disconnect/Uninitialize, or receiving equivalent events)
	 *		- A subsequent call to UnblockPlayers is made with that PlayerName.
	 *
	 * @param PlayerNames List of players to block communication with
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void BlockPlayers(const TArray<FString>& playerNames);

	/**
	 * Unblock communication with players
	 *
	 * @param PlayerNames List of players to unblock communication with
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void UnblockPlayers(const TArray<FString>& playerNames);

	/**
	 * Join a voice channel
	 *
	 * @param ChannelName Channel to join
	 * @param ChannelCredentials password or token
	 * @param ChannelType NonPositional, Positional, or Echo
	 * @param Delegate delegate called once join completes
	 * @param Channel3dProperties Optional parameter to set 3d/Positional channel properties
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (AutoCreateRefTerm = "callback"))
		void JoinChannel(FString channelName, FString channelCredentials, EVivoxVoiceChatChannelType channelType, const FOnVivoxJoinChannelCallback& callback, FVivoxVoiceChatChannel3dProperties channel3dProperties);

	/**
	 * Leave a voice channel
	 *
	 * @param ChannelName Channel to leave
	 * @param Delegate delegate called once leave completes
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (AutoCreateRefTerm = "callback"))
		void LeaveChannel(FString channelName, const FOnVivoxLeaveChannelCallback& callback);

	/**
	 * Set the 3d position of the player
	 *
	 * @param ChannelName
	 * @param SpeakerPosition
	 * @param ListenerPosition
	 * @param ListenerForwardDirection
	 * @param ListenerUpDirection
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void Set3DPosition(const FString& channelName, const FVector& speakerPosition, const FVector& listenerPosition, const FVector& listenerForwardDirection, const FVector& listenerUpDirection);

	/**
	 * Get an array of channels the user is in
	 *
	 * @return Array of connected channel names
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		TArray<FString> GetChannels() const;

	/**
	 * Get an array of players in a channel
	 *
	 * @param ChannelName Channel to get the list of players for
	 * @return Array of names of players in Channel
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		TArray<FString> GetPlayersInChannel(FString channelName) const;

	/**
	 * Get an array of players in a channel
	 *
	 * @param ChannelName Channel to get the type of
	 * @return Channel type
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		EVivoxVoiceChatChannelType GetChannelType(FString channelName) const;

	/**
	 * @param PlayerName Player to get the talking state of
	 * @return true if player is talking
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool IsPlayerTalking(FString playerName);

	/**
	 * Mute or unmute a player
	 *
	 * @param PlayerName Player to mute
	 * @param bMuted true if the player should be muted
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetPlayerMuted(FString playerName, bool bMuted);

	/**
	 * @param PlayerName Player to get the mute state of
	 * @return true if player is muted
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		bool IsPlayerMuted(FString playerName) const;

	/**
	 * Set player volume
	 *
	 * @param PlayerName Player to adjust the volume for
	 * @param Volume Value between 0.0 and 1.0
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetPlayerVolume(FString playerName, float volume);

	/**
	 * @param PlayerName Player to get the volume of
	 * @return volume for player
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		float GetPlayerVolume(FString playerName) const;

	/**
	 * Transmit to all channels that you are currently in
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void TransmitToAllChannels();

	/**
	 * Stop transmitting to all channels that you are currently in. This does not apply to channels you will join.
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void TransmitToNoChannels();

	/**
	 * Transmit to a specific channel
	 *
	 * @param ChannelName Channel to transmit to
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void TransmitToSpecificChannel(FString channelName);

	/**
	 * Get the current transmit mode
	 *
	 * @return Transmit mode. If it is EVoiceChatTransmitMode::Channel, the channel can be retrieved using GetTransmitChannel()
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		EVivoxVoiceChatTransmitMode GetTransmitMode() const;

	/**
	 * Get the specific channel we are transmitting to
	 *
	 * @return Channel we are transmitting to. Will be empty if GetTransmissionMode() returned a value other than EVoiceChatTransmitMode::Channel
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		FString GetTransmitChannel() const;

	/**
	 * Set record volume
	 *
	 * @param Volume Value between 0.0 and 1.0
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetAudioInputVolume(float volume);

	/**
	 * Set playback volume
	 *
	 * @param Volume Value between 0.0 and 1.0
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetAudioOutputVolume(float volume);

	/**
	 * Get volume of audio input device
	 *
	 * @return Audio input device volume. Will be between 0.0 and 1.0
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		float GetAudioInputVolume() const;

	/**
	 * Get volume of audio output device
	 *
	 * @return Audio output device volume. Will be between 0.0 and 1.0
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		float GetAudioOutputVolume() const;

	/**
	 * Mute or unmute the audio input device
	 *
	 * @param bIsMuted set to true to mute the device or false to unmute
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetAudioInputDeviceMuted(bool bIsMuted);

	/**
	 * Mute or unmute the audio output device
	 *
	 * @param bIsMuted set to true to mute the device or false to unmute
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetAudioOutputDeviceMuted(bool bIsMuted);

	/**
	 * Is the input device muted?
	 *
	 * @return true if the input device is muted
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		float GetAudioInputDeviceMuted() const;

	/**
	 * Is the output device muted?
	 *
	 * @return true if the input device is muted
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		float GetAudioOutputDeviceMuted() const;

	/**
	 * Get a list of available audio input devices
	 *
	 * @return Array of audio input devices
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		TArray<FVivoxVoiceChatDeviceInfo> GetAvailableInputDeviceInfos() const;


	/**
	 * Get a list of available audio output devices
	 *
	 * @return Array of audio output devices
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		TArray<FVivoxVoiceChatDeviceInfo> GetAvailableOutputDeviceInfos() const;

	/**
	 * Set the audio input device to use
	 *
	 * @param InputDevice String from GetAvailableInputDevice. If empty, will use the default input device
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetInputDevice(FString inputDevice);

	/**
	 * Set the audio input device to use
	 *
	 * @param InputDeviceId Device Id from GetAvailableInputDeviceInfos. If empty, will use the default input device
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetInputDeviceId(FString inputDeviceId);

	/**
	 * Set the audio output device to use
	 *
	 * @param OutputDevice String from GetAvailableOutputDevice. If empty, will use the default output device
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetOutputDevice(FString outputDevice);

	/**
	 * Set the audio output device to use
	 *
	 * @param OutputDeviceId Device Id from GetAvailableOutputDeviceInfos. If empty, will use the default output device
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		void SetOutputDeviceId(FString outputDeviceId);

	/**
	 * Get the device info of the audio input device that is being used
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		FVivoxVoiceChatDeviceInfo GetInputDeviceInfo() const;

	/**
	 * Get the device info of the audio output device that is being used
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		FVivoxVoiceChatDeviceInfo GetOutputDeviceInfo() const;

	/**
	 * Get the device info of the default audio input device
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		FVivoxVoiceChatDeviceInfo GetDefaultInputDeviceInfo() const;

	/**
	 * Get the device info of the default audio output device
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore")
		FVivoxVoiceChatDeviceInfo GetDefaultOutputDeviceInfo() const;

	UFUNCTION(BlueprintCallable, Category = "VivoxCore|Utilities")
		static FString GetSafePlayerName(FString baseName);

	/**
	 * Generate Login token. Should be used for development purposes only
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (WorldContext = "worldContextObject"))
		static FString InsecureGetLoginToken(UObject* worldContextObject, FString playerName);
	
	/**
	 * Generate Join token. Should be used for development purposes only
	 */
	UFUNCTION(BlueprintCallable, Category = "VivoxCore", meta = (WorldContext = "worldContextObject"))
		static FString InsecureGetJoinToken(UObject* worldContextObject, FString channelName, EVivoxVoiceChatChannelType channelType, FVivoxVoiceChatChannel3dProperties channel3dProperties);

public:
	IVoiceChat* m_VivoxChat;
private:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATE HANDLES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	FDelegateHandle m_OnVivoxVoiceChatAvailableAudioDevicesChanged;
	FDelegateHandle m_OnVivoxVoiceChatConnected;
	FDelegateHandle m_OnVivoxVoiceChatDisconnected;
	FDelegateHandle m_OnVivoxVoiceChatReconnected;
	FDelegateHandle m_OnVoiceChatLoggedIn;
	FDelegateHandle m_OnVoiceChatLoggedOut;

	FDelegateHandle m_OnVoiceChatChannelJoined;
	FDelegateHandle m_OnVoiceChatChannelExited;
	FDelegateHandle m_OnVoiceChatCallStatsUpdated;

	FDelegateHandle m_OnVoiceChatPlayerAdded;
	FDelegateHandle m_OnVoiceCHatPlayerRemoved;

	FDelegateHandle m_OnVivoxVoiceChatPlayerTalkingUpdated;
	FDelegateHandle m_OnVoiceChatPlayerMuteUpdated;
	FDelegateHandle m_OnVivoxVoiceChatPlayerVolumeUpdated;
protected:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK FUNCTIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#if !DISABLE_VIVOXCORE
	void OnVoiceChatAvailableAudioDevicesChanged();
	void OnVoiceChatConnected();
	void OnVoiceChatDisconnected(const FVoiceChatResult& result);
	void OnVoiceChatReconnected();
	void OnVoiceChatLoggedIn(const FString& playerName);
	void OnVoiceChatLoggedOut(const FString& playerName);
	void OnVoiceChatChannelJoined(const FString& playerName);
	void OnVoiceChatChannelExited(const FString& channelName, const FVoiceChatResult& reason);
	void OnVoiceChatCallStatsUpdated(const FVoiceChatCallStats& callstats);
	void OnVoiceChatPlayerRemoved(const FString& channelName, const FString& playerName);
	void OnVoiceChatPlayerAdded(const FString& channelName, const FString& playerName);
	void OnVoiceChatPlayerTalkingUpdated(const FString& channelName, const FString& playerName, bool bIsTalking);
	void OnVoiceChatPlayerMuteUpdated(const FString& channelName, const FString& playerName, bool bIsMuted);
	void OnVoiceChatPlayerVolumeUpdated(const FString& channelName, const FString& playerName, float volume);
#endif
};