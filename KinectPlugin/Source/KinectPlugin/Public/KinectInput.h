// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "InputCoreTypes.h"

//Input mapping
struct FKeysKinect
{

	//Left Hand movement
	static const FKey KinectRightHandX;
	static const FKey KinectRightHandY;
	static const FKey KinectRightHandZ;

	//Right Hand 
	static const FKey KinectLeftHandX;
	static const FKey KinectLeftHandY;
	static const FKey KinectLeftHandZ;

	//Right Hand Lateral Raise
	static const FKey KinectRightLatRaiseK;	// Key Input
	static const FKey KinectRightLatRaise; // Float Input

	static const FKey KinectLeftLatRaiseK;	// Key Input
	static const FKey KinectLeftLatRaise; // Float Input

	static const FKey KinectRightFrRaiseK;
	static const FKey KinectRightPunch;
};