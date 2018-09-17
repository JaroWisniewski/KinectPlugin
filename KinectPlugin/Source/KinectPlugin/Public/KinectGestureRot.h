// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "KinectJoint.h"
#include "KinectGestureRot.generated.h"


UCLASS(BlueprintType, Category = "Kinect|Gesture")
class UKinectGestureRot : public UObject
{
	GENERATED_BODY()

public:
	
	UKinectGestureRot();
	~UKinectGestureRot();


	// Last Angle of gesture collected
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	float					LastAngle;



	void clearGestureData();

	bool updateGestureData(float, float, float, float, float);

	bool isJump;

	bool Punch;

	bool turnRight = false;

};

