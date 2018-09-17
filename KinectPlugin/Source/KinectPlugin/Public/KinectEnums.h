// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "AllowWindowsPlatformTypes.h"
#include "../ThirdParty/include/Kinect.h"
#include "HideWindowsPlatformTypes.h"
#include "KinectEnums.generated.h"

#define countof(arr) sizeof(arr) / sizeof(arr[0])

constexpr unsigned int KinectBodyCount = BODY_COUNT;
constexpr unsigned int KinectJointCount = JointType_Count;

UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EKinectHndState : uint8
{
	Unknown = 0,
	NotTracked = 1,
	Open = 2,
	Closed = 3,	
	Lasso = 4
};


UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EDetectionResult : uint8
{
	Unknown = 0,
	No = 1,
	Maybe = 2,
	Yes = 3
};

UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EKinectJointT : uint8
{
	SpineBase = 0,
	SpineMid = 1,
	Neck = 2,
	Head = 3,
	ShoulderLeft = 4,
	ElbowLeft = 5,
	WristLeft = 6,
	HandLeft = 7,
	ShoulderRight = 8,
	ElbowRight = 9,
	WristRight = 10,
	HandRight = 11,
	HipLeft = 12,
	KneeLeft = 13,
	AnkleLeft = 14,
	FootLeft = 15,
	HipRight = 16,
	KneeRight = 17,
	AnkleRight = 18,
	FootRight = 19,
	SpineShoulder = 20,
	HandTipLeft = 21,
	ThumbLeft = 22,
	HandTipRight = 23,
	ThumbRight = 24,
	Total = 25
};

UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EKinectTrckingState : uint8
{
	NotTracked = 0,
	Inferred = 1,
	Tracked = 2
};

UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EHandTrckingState : uint8
{
	Hand_Unknown = 0,
	Hand_NotTracked = 1,
	Hand_Open = 2,
	Hand_Closed = 3,
	Hand_Lasso = 4
};

UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EKinectBoneType : uint8
{
	Spine = 0, // Spine - Shoulder Center
	RTorso = 1, // Right Shoulder - Right Hip - Virtual Bone used for angle measurement
	LTorso = 2, // Left Shoulder - Left Hip - Virtual Bone used for angle measurement
	RUpperArm = 3,
	RForearm = 4,
	RHand = 5,
	LUpperArm = 6,
	LForearm = 7,
	LHand = 8,
	RUpperLeg = 9,
	RLowerLeg = 10,
	RFoot = 11,
	LUpperLeg = 12,
	LLowerLeg = 13,
	LFoot = 14,
	Total = 15
};


UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EDetectionState : uint8
{
	State_Unknown = 0,
	State_No = 1,
	State_Maybe = 2,
	State_Yes = 3
};

UENUM(BlueprintType, Category = "Kinect|Enums")
enum class EGestureState : uint8
{
	State_Unknown = 0,
	State_Detected = 1,
	State_Raise = 2,
	State_Middle = 3,
	State_Lower = 4,
	State_Finished = 5
};