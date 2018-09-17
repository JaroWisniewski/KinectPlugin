// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "KinectJoint.h"
#include "KinectGesture.h"
#include "KinectGestureRot.h"
#include "KinectBody.generated.h"


template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL) 
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}


UCLASS(BlueprintType, Category = "Kinect|Body")
class UKinectBody : public UObject
{
	GENERATED_BODY()

public:

	UKinectBody();
	~UKinectBody();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture limit"), Category = "Kinect|Body|Gestures")
		void SetupMovementLimits();
	UFUNCTION(BlueprintCallable, meta = (Keywords = "kinect gesture limit"), Category = "Kinect|Body|Gestures")
		void MovementFocus();

	static const int		KinectGestureCount = 5; // total number of gestures tracked
	// If this body is being tracked
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body")
		bool					IsTracked;
	// If this body was tracke in previous frame - gesture control
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body")
		bool					WasTracked;
	// Is Kinect Tracking Rotation
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body")
		bool					TrackRotation;
	// Body Index (0-5)
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body")
		int32					BodyIndex;
	/** Body's unique Tracking ID. */
	UPROPERTY()
		UINT64					TrackingId;
	/* Array of all body Joints*/
	UPROPERTY()
		TArray<FKinectJoint>	BodyJoints;
	/* Array of all body Bones*/
	UPROPERTY()
		TArray<FKinectBone>		BodyBones;
	/** Left hand pose */
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Hands")
		EKinectHndState		HandLeftState;
	/** Left hand tracking state */
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Hands")
		EKinectTrckingState	HandLeftTrackingState;
	/* Gesture manager*/
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
		UKinectGesture*			RHandLatRaise;
	/* Gesture manager*/
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
		UKinectGesture*			LHandLatRaise;
	/* Gesture manager*/
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
		UKinectGesture*			RHandFrontRaise;
	/* Gesture manager*/
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Gestures")
		UKinectGestureRot*			RArmRotation;
	/** Right hand pose */
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Hands")
		EKinectHndState		HandRightState;
	/** Right hand tracking state */
	UPROPERTY(BlueprintReadOnly, Category = "Kinect|Body|Hands")
		EKinectTrckingState	HandRightTrackingState;
	/** Last Frame Update. */
	UPROPERTY()
		float					frameTime;

	bool updateBodyData(IBody*, float);

	void clearBodyData();

	float getRightShoulderAngle();

	float getRightShoulderFlexionAngle();

	float getRightArmRotation();

	float getLeftShoulderAngle();

	float getRightElbowAngle();

	float getLeftElbowAngle();

};
