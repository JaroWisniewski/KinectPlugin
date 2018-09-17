// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#include "KinectGesture.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"

#define SETUP_TIME 20 // 20 seconds for max angle detection
#define NO_SETUP -1
#define POSITION_RESET 7 // Min angle at which position is restarted

UKinectGesture::UKinectGesture() :

	IsTracked(false),
	GestureState(EGestureState::State_Unknown),
	LastAngle(0),
	minAngle(0), // check values
	maxAngle(120),
	ElbowForm(true),
	DirectionForm(true),
	positionReset(false),
	timeStart(0),
	timeFinish(0),
	TopAngle(1),
	isJump(false),
	FormChecker(true),
	limitSet(true),
	TimeCollected(false),
	Reps(0),
	handicap(0)
{
	MovementData.Empty();
	TimeData.Empty();
	setupTime = -1;
}


UKinectGesture::~UKinectGesture()
{
	MovementData.Empty();
	TimeData.Empty();
}

bool UKinectGesture::updateGestureData(float angle, float elbowAngle, float time, float distance)
{
	if (FormChecker)
	{
		if (elbowAngle > 15)
		{

			ElbowForm = false;

		}
		if (FMath::Abs(distance) > 0.1)
		{
			DirectionForm = false;
		}
		else
		{
			DirectionForm = true;
		}
	}

	IsTracked = true;

	//Hand Raising
	if (angle > LastAngle)
	{

		switch (GestureState)

		{

		case EGestureState::State_Unknown:

			GestureState = EGestureState::State_Detected;

			break;

		case EGestureState::State_Detected:

			if (!ElbowForm) { ElbowForm = true; }

			positionReset = true;

			TimeCollected = true;

			timeStart = time;

			if (minAngle > angle && angle > 0)

			{

				minAngle = angle;

			}

			GestureState = EGestureState::State_Raise;

			break;

		case EGestureState::State_Raise:

			if (maxAngle <= angle)
			
			{
			
				if(positionReset)
					
				{

					isJump = true;

					Reps++;

					handicap++;

					positionReset = false;

					ElbowForm = true;
					
				}

				else

				{

					isJump = true;

					ElbowForm = true; // Movement abovue the limit won't be penelised for the form

				}

			}

			else if (TimeCollected && angle - minAngle > POSITION_RESET)

			{

				timeStart = time;

				TimeCollected = false;

				isJump = false;

			}

			else

			{
				isJump = false;
			}

			if (TopAngle < angle)

			{

				TopAngle = angle;

			}

			break;

		case EGestureState::State_Middle:

			GestureState = EGestureState::State_Raise;

			if (maxAngle <= angle)

			{

				if (positionReset)

				{

					isJump = true;

					Reps++;

					handicap++;

					positionReset = false;

					ElbowForm = true;

				}

				else

				{

					isJump = true;

					ElbowForm = true; // Movement abovue the limit won't be penelised for the form

				}

			}

			else

			{

				isJump = false;
			}

			if (TopAngle <= angle)

			{

				TopAngle = angle;

			}

			break;

		case EGestureState::State_Lower:

			if (angle - minAngle < POSITION_RESET && positionReset == false)

			{

				isJump = false;

				GestureState = EGestureState::State_Finished;

				if (minAngle > angle && angle > 0)

				{

					minAngle = angle;

				}

			}

			else

			{

				GestureState = EGestureState::State_Raise;

				if (minAngle > angle && angle > 0)

				{

					minAngle = angle;

				}

				isJump = false;

			}

			break;

		case EGestureState::State_Finished:

			isJump = false;

			if (!positionReset) 
			
			{

				positionReset = true;
			
			}

			GestureState = EGestureState::State_Raise;

			if (TimeCollected && angle - minAngle > POSITION_RESET)

			{

				timeStart = time;

				TimeCollected = false;

			}

			break;

		}

	}

	else if (angle < LastAngle)

	{

		isJump = false;

		switch (GestureState)

		{

		case EGestureState::State_Unknown:

			GestureState = EGestureState::State_Detected;

			break;

		case EGestureState::State_Detected:

			if (!ElbowForm) { ElbowForm = true; }

			positionReset = true;

			TimeCollected = true;

			timeStart = time;

			if (minAngle > angle && angle > 0)

			{

				minAngle = angle;

			}

			break;

		case EGestureState::State_Raise:

			GestureState = EGestureState::State_Middle;

			if (!angleCollected && angle > ((2*maxAngle)/10) && angle > POSITION_RESET)// TODO check counter effects

			{

				MovementData.Add(LastAngle);

				angleCollected = true;

			}

			break;

		case EGestureState::State_Middle:

			GestureState = EGestureState::State_Lower;

			break;

		case EGestureState::State_Lower:

			if (angle - minAngle < POSITION_RESET)// && positionReset == false)

			{

				GestureState = EGestureState::State_Finished;

				if (minAngle > angle && angle > 0)

				{

					minAngle = angle;

				}

				if (!TimeCollected)

				{

					timeFinish = time - timeStart;

					TimeData.Add(timeFinish);

					TimeCollected = true;

					angleCollected = false;

				}

			}

			else

			{

				if (minAngle > angle && angle > 0)

				{

					minAngle = angle;

				}

			}

			break;

		case EGestureState::State_Finished:

			if (minAngle > angle && angle > 0)

			{

				minAngle = angle;

			}

			break;

		}

	}

	else

	{
		isJump = false;

		switch (GestureState)

		{

		case EGestureState::State_Unknown:

			break;

		case EGestureState::State_Detected:

			if (!ElbowForm) { ElbowForm = true; }

			TimeCollected = true;

			timeStart = time;

			break;

		case EGestureState::State_Raise:

			GestureState = EGestureState::State_Middle;

			break;

		case EGestureState::State_Middle:

			break;

		case EGestureState::State_Lower:

			if (angle - minAngle < POSITION_RESET && positionReset == false)

			{

				GestureState = EGestureState::State_Finished;

			}

			else

			{

				if (minAngle > angle && angle > 0)

				{

					minAngle = angle;

				}

			}

			break;

		case EGestureState::State_Finished:

			break;

		}

	}

	if (handicap == 5)
	
	{

		maxAngle += 5;

		handicap = 0;

	}

	LastAngle = angle;

	return true;

}


void UKinectGesture::clearGestureData()
{	
	IsTracked = false;
	TimeCollected = false;
	GestureState = EGestureState::State_Unknown;
	LastAngle = 0;
	Reps = 0;
	handicap = 0;
	TopAngle = 0;
	minAngle = 0;
	maxAngle = 120;
	setupTime = -1;
	timeStart = 0;
	timeFinish = 0;
	isJump = false;
	ElbowForm = true;
	DirectionForm = true;
	positionReset = false;
	limitSet = false;
	MovementData.Empty();
	TimeData.Empty();
}

void UKinectGesture::loadPlayerData(float minAngl, float maxAngl)
{

	this->minAngle = minAngl;

	this->maxAngle = maxAngl;

	setupTime = NO_SETUP;

	limitSet = true;

	positionReset = true;

	angleCollected = false;

	handicap = 0;

}

void UKinectGesture::setupMovementLimits(float angl, float elbwAngl, float time, float dist)
{

	if (setupTime == NO_SETUP)
	{
	
		setupTime = time;

		ClearMotionLimitData();
	
	}

	if(time - setupTime < SETUP_TIME)

	{

		if (minAngle > angl && angl > 0)

		{

			minAngle = angl;

		}

		if (maxAngle < angl && FMath::Abs(dist) < 0.15 && elbwAngl < 15)

		{

			maxAngle = angl;

		}

	}

	else

	{

		limitSet = true;

		setupTime = NO_SETUP;

		MovementData.Empty();

	}

}

bool UKinectGesture::IsMotionLimitSet()
{

	return limitSet;

}


void UKinectGesture::StartSetup()
{
	if (limitSet == false)
	{

	}
	else
	{
		limitSet = false;
	}
}

void UKinectGesture::ClearMotionLimitData()
{

	minAngle = 120;

	maxAngle = 0;

	TimeCollected = false;

	handicap = 0;

}

void UKinectGesture::SwitchFormChecker()
{

	if (FormChecker)
	{
	
		FormChecker = false;
	
	}
	else
	{

		FormChecker = true;
	
	}
}

void UKinectGesture::DecreaseAngleLimit()
{
	if (maxAngle > minAngle + POSITION_RESET) { maxAngle -= 10; }
}
