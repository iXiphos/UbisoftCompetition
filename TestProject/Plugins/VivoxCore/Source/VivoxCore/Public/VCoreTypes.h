// Copyright (C) 2017-2021 eelDev

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "VCoreTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

UENUM(BlueprintType)
enum class EVivoxVoiceChatTransmitMode : uint8
{
	/** Transmit to none of the channels you are in */
	None,
	/** Transmit to all of the channels you are in */
	All,
	/** Transmit to a specific channel */
	Channel
};

UENUM(BlueprintType)
enum class EVivoxVoiceChatResult : uint8
{
	// The operation succeeded
	Success = 0,

	// Common state errors
	InvalidState,
	NotInitialized,
	NotConnected,
	NotLoggedIn,
	NotPermitted,
	Throttled,

	// Common argument errors
	InvalidArgument,
	CredentialsInvalid,
	CredentialsExpired,

	// Common connection errors
	ClientTimeout,
	ServerTimeout,
	DnsFailure,
	ConnectionFailure,

	// Error does not map to any common categories
	ImplementationError
};

UENUM(BlueprintType)
enum class EVivoxVoiceChatChannelType : uint8
{
	/** Non positional/2d audio channel */
	NonPositional,
	/** Positional/3d audio channel */
	Positional,
	/** Echo channel. Will only ever have one player and will echo anything you say */
	Echo
};

UENUM(BlueprintType)
enum class EVivoxPTTKey : uint8
{
	PTTNoChannel,
	PTTAreaChannel,
	PTTTeamChannel
};


UENUM(BlueprintType)
enum class EVivoxVoiceChatAttenuationModel : uint8
{
	/** No attenuation is applied. The audio will drop to 0 at MaxDistance */
	None,
	/** The attenuation increases in inverse proportion to the distance. The Rolloff is the inverse of the slope of the attenuation curve. */
	InverseByDistance,
	/** The attenuation increases in linear proportion to the distance. The Rolloff is the negative slope of the attenuation curve. */
	LinearByDistance,
	/** The attenuation increases in inverse proportion to the distance raised to the power of the Rolloff. */
	ExponentialByDistance
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FVivoxVoiceChatDeviceInfo
{
	GENERATED_BODY()
public:
	FVivoxVoiceChatDeviceInfo() = default;
#if !DISABLE_VIVOXCORE
#if ENGINE_MINOR_VERSION > 25
	FVivoxVoiceChatDeviceInfo(const FVoiceChatDeviceInfo& data)
		: DisplayName(data.DisplayName)
		, Id(data.Id)
	{}
#endif
#endif // DISABLE_VIVOXCORE
	FVivoxVoiceChatDeviceInfo(FString displayName) 
		: DisplayName(displayName)
	{}
public:
	/** The display name for the device */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString DisplayName;
	/** The unique id for the device */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString Id;
};

USTRUCT(BlueprintType)
struct FVivoxVoiceChatChannel3dProperties
{
	GENERATED_BODY()
public:
	FVivoxVoiceChatChannel3dProperties()
		: AttenuationModel(EVivoxVoiceChatAttenuationModel::InverseByDistance)
		, MinDistance(270.f)
		, MaxDistance(8100.f)
		, Rolloff(1.0f)
		{}
public:
	/** The model used to determine how loud audio is at different distances */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		EVivoxVoiceChatAttenuationModel AttenuationModel;
	/** The distance at which the sound will start to attenuate */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		float MinDistance;
	/** The distance at which sound will no longer be audible */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		float MaxDistance;
	/** How fast the sound attenuates with distance */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		float Rolloff;
};

USTRUCT(BlueprintType)
struct FVivoxVoiceChatCallStats
{
	GENERATED_BODY()
public:
	FVivoxVoiceChatCallStats() = default;
#if !DISABLE_VIVOXCORE
	FVivoxVoiceChatCallStats(const FVoiceChatCallStats& data)
		: CallLength(LexToString(data.CallLength))
		, LatencyMinMeasuredSeconds(LexToString(data.LatencyMinMeasuredSeconds))
		, LatencyMaxMeasuredSeconds(LexToString(data.LatencyMaxMeasuredSeconds))
		, LatencyAverageMeasuredSeconds(LexToString(data.LatencyAverageMeasuredSeconds))
		, PacketsNumTotal(data.PacketsNumTotal)
		, PacketsNumLost(data.PacketsNumLost)
	{}
#endif //DISABLE_VIVOXCORE
public:
	/** Call length in seconds */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString CallLength;
	/** Minimum measured network latency in seconds. Zero if no measurements made */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString LatencyMinMeasuredSeconds;
	/** Maximum measured network latency in seconds. Zero if no measurements made */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString LatencyMaxMeasuredSeconds;
	/** Average measured network latency in seconds. Zero if no measurements made */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString LatencyAverageMeasuredSeconds;
	/** Total number of packets, both received and lost */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		int PacketsNumTotal;
	/** Total number of lost packets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		int PacketsNumLost;
};

USTRUCT(BlueprintType)
struct FVivoxVoiceChatResult
{
	GENERATED_BODY()
public:
	FVivoxVoiceChatResult() = default;
#if !DISABLE_VIVOXCORE
	FVivoxVoiceChatResult(const FVoiceChatResult& data)
		: ResultCode(static_cast<EVivoxVoiceChatResult>(data.ResultCode))
		, ErrorCode(data.ErrorCode)
		, ErrorNum(data.ErrorNum)
		, ErrorDesc(data.ErrorDesc)
	{}
#endif
public:
	/** Success, or an error category */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		EVivoxVoiceChatResult ResultCode;
	/** If we failed, the code for the error */
	FString ErrorCode;
	/** If we failed, a numeric error from the implementation */
	int ErrorNum = 0;
	/** If we failed, more details about the error condition */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vivox")
		FString ErrorDesc;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		SUBSYSTEM DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatConnectCompleteDelegate, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatDisconnectCompleteDelegate, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatLoginCompleteDelegate, const FString&, playerName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatLogoutCompleteDelegate, const FString&, playerName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatChannelJoinCompleteDelegate, const FString&, channelName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatChannelLeaveCompleteDelegate, const FString&, channelName, const FVivoxVoiceChatResult&, result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVivoxVoiceChatAvailableAudioDevicesChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVivoxVoiceChatReconnectedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVivoxVoiceChatConnectedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatDisconnectedDelegate, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatLoggedInDelegate, const FString&, playerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatLoggedOutDelegate, const FString&, playerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatChannelJoinedDelegate, const FString&, channelName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatChannelExitedDelegate, const FString&, channelName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVivoxVoiceChatCallStatsUpdatedDelegate, const FVivoxVoiceChatCallStats&, callStats);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnVivoxVoiceChatPlayerTalkingUpdatedDelegate, const FString&, channelName, const FString&, playerName, bool, bIsTalking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnVivoxVoiceChatPlayerMuteUpdatedDelegate, const FString&, channelName, const FString&, playerName, bool, bIsMuted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnVivoxVoiceChatPlayerVolumeUpdatedDelegate, const FString&, channelName, const FString&, playerName, float, volume);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatPlayerAddedDelegate, const FString&, channelName, const FString&, playerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVivoxVoiceChatPlayerRemovedDelegate, const FString&, channelName, const FString&, playerName);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACKS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnVivoxConnectCallback, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnVivoxDisconnectCallback, const FVivoxVoiceChatResult&, result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnVivoxLoginCallback, const FString&, playerName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnVivoxLogoutCallback, const FString&, playerName, const FVivoxVoiceChatResult&, result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnVivoxJoinChannelCallback, const FString&, channelName, const FVivoxVoiceChatResult&, result);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnVivoxLeaveChannelCallback, const FString&, channelName, const FVivoxVoiceChatResult&, result);
