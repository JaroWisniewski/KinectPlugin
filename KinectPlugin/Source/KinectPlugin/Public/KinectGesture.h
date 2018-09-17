// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "KinectJoint.h"
#include "KinectGesture.generated.h"


UCLASS(BlueprintType, Category = "Kinect|Gesture")
class UKinectGesture : public UObject
{
	GENERATED_BODY()

public:
	
	UKinectGesture();
	~UKinectGesture();


	// If this body is being tracked
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	bool		IsTracked; 

	/* Movement Form Checking*/
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	bool			FormChecker;

	// Gesture State 
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	EGestureState			GestureState;

	// Last Angle of gesture collected
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	float					LastAngle;

	// Highest value of angle
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	float					TopAngle;
	
	// Number of Reps
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	float					Reps;

	// Array of collected movement angles at peak only above the max limit)
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	TArray<float>			MovementData;

	// Array of collected movement times (only above the max limit)
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	TArray<float>			TimeData;

	// Angle at which we execute motion / max turning value
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	float					maxAngle;

	// Angle at which we reset position
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	float					minAngle;

	// Incorrect elbow form notification
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	bool					ElbowForm;

	// Incorrect elbow form notification
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
	bool					DirectionForm;

	/* Load movement Limits*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture data"), Category = "Kinect|Gesture")
	void loadPlayerData(float minAngl, float maxAngl);

	/* Decrease movement Limits*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture data"), Category = "Kinect|Gesture")
	void DecreaseAngleLimit();

	/* Kinects collects movement data for 10 seconds in order to setup new movement limits
	- locks Kinect control for that time */
	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture data"), Category = "Kinect|Gesture")
	void setupMovementLimits(float angl, float elbwAngl, float time, float dist);

	/* Are Movement Motion Limist Set*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture data"), Category = "Kinect|Gesture")
	bool IsMotionLimitSet();

	/* Switches Form Checker On/Off*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture data"), Category = "Kinect|Gesture")
	void SwitchFormChecker();

	/* Changes Startup flag - limitSet*/
	void StartSetup();

	void clearGestureData();

	void ClearMotionLimitData();

	bool updateGestureData(float, float, float, float);

	bool isJump;

private:

	bool					TimeCollected; // Time collection flag

	bool					angleCollected; // Angle collection flag

	bool					positionReset; //Used for rep counting

	float					setupTime; // Setup time start

	float					timeStart; // Time of gesture start

	float					timeFinish; // Time of gesture - moved to TimeData

	bool					limitSet; // Is the Movement Limit Set?

	int						handicap; // increases max angle
};

