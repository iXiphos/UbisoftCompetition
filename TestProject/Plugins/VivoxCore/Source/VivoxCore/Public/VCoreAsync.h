// Copyright (C) 2017-2021 eelDev

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "VCoreSubsystem.h"
#include "Containers/Ticker.h"
#include "VCoreAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxConnect, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxDisconnect, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxLogin, const FString&, playerName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxLogout, const FString&, playerName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxJoinChannel, const FString&, channelName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxLeaveChannel, const FString&, channelName, const FVivoxVoiceChatResult&, result);

UCLASS()
class VIVOXCORE_API UVivoxCoreAsyncAction : public UBlueprintAsyncActionBase, public FTickerObjectBase
{
	GENERATED_BODY()
public:
	UVivoxCoreAsyncAction()
		: m_TaskDuration(0.0f)
	{}
public:
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;
public:
	virtual bool Tick(float deltaTime) override;
public:
	UVivoxCore* m_VivoxSubsystem;
private:
	float m_TaskDuration;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxConnect
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class VIVOXCORE_API UVivoxConnect : public UVivoxCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnVivoxConnect OnCallback;
public:
	/**
	 * Connect to a voice server
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "VivoxCore|Async"))
		static UVivoxConnect* Connect(UObject* WorldContextObject);
public:
	void Activate() override;
private:
	template <typename... TArgs>
	void HandleCallback(TArgs&&... args);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxDisconnect
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class VIVOXCORE_API UVivoxDisconnect : public UVivoxCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnVivoxDisconnect OnCallback;
public:
	/**
	 * Disconnect from a voice server
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "VivoxCore|Async"))
		static UVivoxDisconnect* Disconnect(UObject* WorldContextObject);
public:
	void Activate() override;
private:
	template <typename... TArgs>
	void HandleCallback(TArgs&&... args);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxLogin
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class VIVOXCORE_API UVivoxLogin : public UVivoxCoreAsyncAction
{
	GENERATED_BODY()
public:
	int32 m_UserId;
	FString m_PlayerName;
	FString m_Credentials;

	UPROPERTY(BlueprintAssignable)
		FOnVivoxLogin OnCallback;
public:
	/**
	 * Login to the connected voice server
	 *
	 * @param userId Platform user Id for the player logging in
	 * @param playerName Name of the account to login to
	 * @param credentials Token or password
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "VivoxCore|Async"))
		static UVivoxLogin* Login(UObject* WorldContextObject, int32 userId, FString playerName, FString credentials);
public:
	void Activate() override;
private:
	template <typename... TArgs>
	void HandleCallback(TArgs&&... args);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxLogout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class VIVOXCORE_API UVivoxLogout : public UVivoxCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnVivoxLogout OnCallback;
public:
	/**
	 * Logout from the connected voice server
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "VivoxCore|Async"))
		static UVivoxLogout* Logout(UObject* WorldContextObject);
public:
	void Activate() override;
private:
	template <typename... TArgs>
	void HandleCallback(TArgs&&... args);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxJoinChannel
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class VIVOXCORE_API UVivoxJoinChannel : public UVivoxCoreAsyncAction
{
	GENERATED_BODY()
public:
	FString m_ChannelName;
	FString m_ChannelCredentials;
	EVivoxVoiceChatChannelType m_ChannelType;
	FVivoxVoiceChatChannel3dProperties m_Channel3dProperties;

	UPROPERTY(BlueprintAssignable)
		FOnVivoxJoinChannel OnCallback;
public:
	/**
	 * Join a voice channel
	 *
	 * @param channelName Channel to join
	 * @param channelCredentials password or token
	 * @param channelType NonPositional, Positional, or Echo
	 * @param channel3dProperties Optional parameter to set 3d/Positional channel properties
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "VivoxCore|Async"))
		static UVivoxJoinChannel* JoinChannel(UObject* WorldContextObject, FString channelName, FString channelCredentials, EVivoxVoiceChatChannelType channelType, FVivoxVoiceChatChannel3dProperties channel3dProperties);
public:
	void Activate() override;
private:
	template <typename... TArgs>
	void HandleCallback(TArgs&&... args);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UVivoxLeaveChannel
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class VIVOXCORE_API UVivoxLeaveChannel : public UVivoxCoreAsyncAction
{
	GENERATED_BODY()
public:
	FString m_ChannelName;

	UPROPERTY(BlueprintAssignable)
		FOnVivoxLeaveChannel OnCallback;
public:
	/**
	 * Leave a voice channel
	 *
	 * @param channelName Channel to leave
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "VivoxCore|Async"))
		static UVivoxLeaveChannel* LeaveChannel(UObject* WorldContextObject, FString channelName);
public:
	void Activate() override;
private:
	template <typename... TArgs>
	void HandleCallback(TArgs&&... args);
};
