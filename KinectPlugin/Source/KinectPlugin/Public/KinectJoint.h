// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "KinectEnums.h"
#include "KinectJoint.generated.h"

#define countof(arr) sizeof(arr) / sizeof(arr[0])

USTRUCT(BlueprintType, Category = "Kinect|Structs")
struct FKinectJoint 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints")
	EKinectJointT			Type;
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints", Meta = (Keywords = "position translation transform camera"))
	FVector					Location;
//	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints", Meta = (Keywords = "transform rotation angles camera"))
//	FRotator				Orientation;
	/** Joint location relative to Kinect sensor in color space with depth. */
//	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints", Meta = (Keywords = "position translation transform color"))
//	FVector					ColorLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints")
		EKinectTrckingState	TrackingState;

	FKinectJoint() :
		Type(EKinectJointT::AnkleLeft),
		TrackingState(EKinectTrckingState::NotTracked),
		Location(FVector(0.f))
		//		Orientation(FRotator(0.f))
	{}

};

USTRUCT(BlueprintType, Category = "Kinect|Structs")
struct FKinectBone
{
	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints")
	EKinectBoneType			Type;
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints", Meta = (Keywords = "position translation transform camera"))
	FVector					Location;
		//	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints", Meta = (Keywords = "transform rotation angles camera"))
		//	FRotator				Orientation;
		/** Joint location relative to Kinect sensor in color space with depth. */
		//	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints", Meta = (Keywords = "position translation transform color"))
		//	FVector					ColorLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Joints")
	EKinectTrckingState	TrackingState;

	FKinectBone() :
			Type(EKinectBoneType::Spine),
			TrackingState(EKinectTrckingState::NotTracked),
			Location(FVector(0.f))
			//		Orientation(FRotator(0.f))
		{}

};
