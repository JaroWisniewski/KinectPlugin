// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#include "KinectGestureRot.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"

// Amgle at which Reps will be increased

#define SETUP_TIME 20
#define NO_SETUP -1
#define POSITION_RESET 7

UKinectGestureRot::UKinectGestureRot() :

	LastAngle(0),
	isJump(false),
	Punch(false)
{}


UKinectGestureRot::~UKinectGestureRot()
{

}

bool UKinectGestureRot::updateGestureData(float angle, float elbowAngle, float time, float distance, float shoulderAngle)
{
	//TODO Correct calculations
	if (elbowAngle > 100) { isJump = true; }
	else { isJump = false; }

	if (elbowAngle < 15 && shoulderAngle > 50)
	{
		Punch = true;
	}
	else
	{
		Punch = false;
	}
	LastAngle = 180 - angle;
	if (distance >= 0)
	{
		turnRight = false;
	}
	else
	{
		turnRight = true;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Angle Collected %f"), elbowAngle));

	return true;

}


void UKinectGestureRot::clearGestureData()
{	

	LastAngle = 0;
	isJump = false;
	Punch = false;

}

