// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#include "KinectBody.h"
#include "Engine/Engine.h"
#include "KinectController.h"


UKinectBody::UKinectBody():
		
		BodyIndex(999),
		WasTracked(false),
		IsTracked(false),
		TrackRotation(false),
		TrackingId(999),
		HandLeftState(EKinectHndState::NotTracked),
		HandLeftTrackingState(EKinectTrckingState::NotTracked),
		HandRightState(EKinectHndState::NotTracked),
		HandRightTrackingState(EKinectTrckingState::NotTracked)
{
		BodyJoints.SetNum((int)EKinectJointT::Total);
		int32 n = (int32)EKinectJointT::Total;
		for (int32 i = 0; i < n; ++i)
		{
			BodyJoints[i].Type = (EKinectJointT)i;
		}

		BodyBones.SetNum((int)EKinectBoneType::Total);
		n = (int32)EKinectBoneType::Total;
		for (int32 i = 0; i < n; ++i)
		{
			BodyBones[i].Type = (EKinectBoneType)i;
		}
		RHandLatRaise = NewObject<UKinectGesture>();
		RHandLatRaise->AddToRoot();
		LHandLatRaise = NewObject<UKinectGesture>();
		LHandLatRaise->AddToRoot();
		RHandFrontRaise = NewObject<UKinectGesture>();
		RHandFrontRaise->AddToRoot();
		RArmRotation = NewObject<UKinectGestureRot>();
		RArmRotation->AddToRoot();
		frameTime = -1;
}


UKinectBody::~UKinectBody()
{

	BodyJoints.Empty();
	BodyBones.Empty();
	LHandLatRaise->RemoveFromRoot();
	RHandLatRaise->RemoveFromRoot();
	RHandFrontRaise->RemoveFromRoot();
	RArmRotation->RemoveFromRoot();
}

bool UKinectBody::updateBodyData(IBody* pBody, float delta)
{

	frameTime = delta;

	UINT64 newTrackingId;

	pBody->get_TrackingId(&newTrackingId);

	if (IsTracked)

	{

		if (TrackingId == newTrackingId)

		{

			WasTracked = IsTracked;

		}

		else

		{

			WasTracked = false;

			TrackingId = newTrackingId;

		}
	}

	else // New body found

	{

		IsTracked = true;

		WasTracked = false;

	}

	HRESULT hr;

	EKinectTrckingState bodytracking = EKinectTrckingState::Tracked;

	JointOrientation newOrientations[JointType_Count];

	Joint newJoints[JointType_Count];

	HandState newLeftHandState = HandState_Unknown;

	HandState newRightHandState = HandState_Unknown;

	pBody->get_HandLeftState(&newLeftHandState);

	pBody->get_HandRightState(&newRightHandState);

	HandLeftState = (EKinectHndState)newLeftHandState;

	HandRightState = (EKinectHndState)newRightHandState;

	HandRightTrackingState = EKinectTrckingState::Tracked;

	HandRightTrackingState = EKinectTrckingState::Tracked;

	hr = pBody->GetJointOrientations(KinectJointCount, newOrientations);

	if (FAILED(hr))

	{

		UE_LOG(LogTemp, Error, TEXT("Joint orientation not extracted!"));

	}

	hr = pBody->GetJoints(KinectJointCount, newJoints);

	if (SUCCEEDED(hr))

	{

		for (int j = 0; j < KinectJointCount; ++j)

		{

			BodyJoints[j].TrackingState = (EKinectTrckingState)newJoints[j].TrackingState;

			BodyJoints[j].Location = FVector(newJoints[j].Position.X, newJoints[j].Position.Y, newJoints[j].Position.Z);

//			FQuat newQrtOrientations = FQuat(newOrientations[j].Orientation.x, newOrientations[j].Orientation.y, newOrientations[j].Orientation.z, newOrientations[j].Orientation.w);

		}

		for (int32 i = 0; i < (int32)EKinectBoneType::Total; ++i)

		{	// Update Bones Data
			switch (i)
			{
			case 0:
				//Spine
				BodyBones[i].Location = BodyJoints[20].Location - BodyJoints[1].Location;

				break;

			case 1:
				//Right Side of Torso
				BodyBones[i].Location = BodyJoints[16].Location - BodyJoints[8].Location;

				break;

			case 2:
				//Left Side of Torso
				BodyBones[i].Location = BodyJoints[12].Location - BodyJoints[4].Location;

				break;

			case 3:
				//Right Upper Arm
				BodyBones[i].Location = BodyJoints[9].Location - BodyJoints[8].Location;

				break;

			case 4:
				//Right Forearm
				BodyBones[i].Location = BodyJoints[10].Location - BodyJoints[9].Location;

				break;

			case 5:
				//Right Hand
				BodyBones[i].Location = BodyJoints[23].Location - BodyJoints[10].Location;

				break;
			case 6:
				//Left Upper Arm
				BodyBones[i].Location = BodyJoints[5].Location - BodyJoints[4].Location;

				break;

			case 7:
				//Left Forearm
				BodyBones[i].Location = BodyJoints[6].Location - BodyJoints[5].Location;

				break;

			case 8:
				//Left Hand
				BodyBones[i].Location = BodyJoints[21].Location - BodyJoints[6].Location;

				break;
			case 9:
				//Right Upper Leg
				BodyBones[i].Location = BodyJoints[17].Location - BodyJoints[16].Location;

				break;
			case 10:
				//Right Lower Leg
				BodyBones[i].Location = BodyJoints[18].Location - BodyJoints[17].Location;

				break;

			case 11:
				//Right Foot
				BodyBones[i].Location = BodyJoints[19].Location - BodyJoints[18].Location;

				break;

			case 12:
				//Left Upper Leg
				BodyBones[i].Location = BodyJoints[13].Location - BodyJoints[12].Location;

				break;
			case 13:
				//Left Lower Leg
				BodyBones[i].Location = BodyJoints[14].Location - BodyJoints[13].Location;

				break;

			case 14:
				//Left Foot
				BodyBones[i].Location = BodyJoints[15].Location - BodyJoints[14].Location;

				break;
			}

			BodyBones[i].TrackingState = EKinectTrckingState::Tracked;
		
		}

	}

	if (RHandLatRaise->IsMotionLimitSet() == false || LHandLatRaise->IsMotionLimitSet() == false || RHandFrontRaise->IsMotionLimitSet() == false)
	
	{

		RHandLatRaise->setupMovementLimits(getRightShoulderAngle(), getRightElbowAngle(), frameTime, BodyJoints[8].Location.Z - BodyJoints[9].Location.Z);
		LHandLatRaise->setupMovementLimits(getLeftShoulderAngle(), getLeftElbowAngle(), frameTime, BodyJoints[4].Location.Z - BodyJoints[5].Location.Z);
		RHandFrontRaise->setupMovementLimits(getRightShoulderFlexionAngle(), getRightElbowAngle(), frameTime, BodyJoints[8].Location.X - BodyJoints[9].Location.X);

	}

	else
	
	{
	
		RHandLatRaise->updateGestureData(getRightShoulderAngle(), getRightElbowAngle(), frameTime, BodyJoints[8].Location.Z - BodyJoints[9].Location.Z);
		LHandLatRaise->updateGestureData(getLeftShoulderAngle(), getLeftElbowAngle(), frameTime, BodyJoints[4].Location.Z - BodyJoints[5].Location.Z);
		RHandFrontRaise->updateGestureData(getRightShoulderFlexionAngle(), getRightElbowAngle(), frameTime, BodyJoints[8].Location.X - BodyJoints[9].Location.X);

		if (TrackRotation) { RArmRotation->updateGestureData(getRightArmRotation(), getRightElbowAngle(), frameTime, BodyJoints[9].Location.X - BodyJoints[10].Location.X, getRightShoulderFlexionAngle()); }
	}
	
	return true;

}



void UKinectBody::clearBodyData()
{

	WasTracked = false;
	IsTracked = false;
	TrackingId = 0Ui64;
	frameTime = -1;
	HandLeftState = EKinectHndState::NotTracked;
	HandLeftTrackingState = EKinectTrckingState::NotTracked;

	HandRightState = EKinectHndState::NotTracked;
	HandRightTrackingState = EKinectTrckingState::NotTracked;



	if (BodyJoints.Num() != KinectJointCount) return;

	for (int32 j = 0; j < KinectJointCount; ++j)
	{
		if (!BodyJoints.IsValidIndex(j)) continue;
		BodyJoints[j].TrackingState = EKinectTrckingState::NotTracked;
		BodyJoints[j].Location.Set(0.f, 0.f, 0.f);
	}

	if (BodyBones.Num() != KinectJointCount) return;

	for (int32 j = 0; j < (int32)EKinectBoneType::Total; ++j)
	{
		if (!BodyBones.IsValidIndex(j)) continue;
		BodyBones[j].TrackingState = EKinectTrckingState::NotTracked;
		BodyBones[j].Location.Set(0.f, 0.f, 0.f);
	}

}

float UKinectBody::getRightShoulderAngle()
{
	float angle;
	FVector2D BoneA = FVector2D(BodyBones[1].Location.X, BodyBones[1].Location.Y);
	FVector2D BoneB = FVector2D(BodyBones[3].Location.X, BodyBones[3].Location.Y);
	float Dot = FVector2D::DotProduct(BoneA, BoneB);
	float ALength = FMath::Sqrt(FMath::Pow(BodyBones[1].Location.X, 2) + (FMath::Pow(BodyBones[1].Location.Y, 2) /*+ (FMath::Pow(BodyBones[1].Location.Z, 2))*/));
	float BLength = FMath::Sqrt(FMath::Pow(BodyBones[3].Location.X, 2) + (FMath::Pow(BodyBones[3].Location.Y, 2) /*+ (FMath::Pow(BodyBones[3].Location.Z, 2))*/));

	angle = (FMath::Acos(Dot / (ALength*BLength)) * 180 / PI); // Correct calculations
	return angle;
}

float UKinectBody::getRightShoulderFlexionAngle()
{
	float angle;
	FVector2D BoneA = FVector2D(BodyBones[1].Location.Y, BodyBones[1].Location.Z);
	FVector2D BoneB = FVector2D(BodyBones[3].Location.Y, BodyBones[3].Location.Z);
	float Dot = FVector2D::DotProduct(BoneA, BoneB);
	float ALength = FMath::Sqrt(FMath::Pow(BodyBones[1].Location.Z, 2) + (FMath::Pow(BodyBones[1].Location.Y, 2) /*+ (FMath::Pow(BodyBones[1].Location.Z, 2))*/));
	float BLength = FMath::Sqrt(FMath::Pow(BodyBones[3].Location.Z, 2) + (FMath::Pow(BodyBones[3].Location.Y, 2) /*+ (FMath::Pow(BodyBones[3].Location.Z, 2))*/));

	angle = (FMath::Acos(Dot / (ALength*BLength)) * 180 / PI);  // Correct calculations
	return angle;
}

float UKinectBody::getLeftShoulderAngle()
{
	float angle;
	FVector2D BoneA = FVector2D(BodyBones[2].Location.X, BodyBones[2].Location.Y);
	FVector2D BoneB = FVector2D(BodyBones[6].Location.X, BodyBones[6].Location.Y);
	float Dot = FVector2D::DotProduct(BoneA, BoneB);
	float ALength = FMath::Sqrt(FMath::Pow(BodyBones[2].Location.X, 2) + (FMath::Pow(BodyBones[2].Location.Y, 2) /*+ (FMath::Pow(BodyBones[2].Location.Z, 2))*/));
	float BLength = FMath::Sqrt(FMath::Pow(BodyBones[6].Location.X, 2) + (FMath::Pow(BodyBones[6].Location.Y, 2) /*+ (FMath::Pow(BodyBones[6].Location.Z, 2))*/));

	angle = (FMath::Acos(Dot / (ALength*BLength)) * 180 / PI);  // Correct calculations
	return angle;
}

float UKinectBody::getRightArmRotation()
{
	FVector2D BoneA = FVector2D(BodyBones[4].Location.X, BodyBones[4].Location.Z);
	float BLength = FMath::Sqrt(FMath::Pow(BoneA.X, 2) + (FMath::Pow(BoneA.Y, 2)));
	FVector2D DefaultPosition = FVector2D(0, BodyJoints[9].Location.Z);
	float angle;
	float Dot = FVector2D::DotProduct(DefaultPosition, BoneA);
	float ALength = FMath::Sqrt(FMath::Pow(DefaultPosition.X, 2) + (FMath::Pow(DefaultPosition.Y, 2)));

	angle = (FMath::Acos(Dot / (ALength*BLength)) * 180 / PI);  // Correct calculations
	return angle;
}

float UKinectBody::getRightElbowAngle()
{
	float angle;
	float Dot = FVector::DotProduct(BodyBones[3].Location, BodyBones[4].Location);
	float ALength = FMath::Sqrt(FMath::Pow(BodyBones[3].Location.X, 2) + (FMath::Pow(BodyBones[3].Location.Y, 2) + (FMath::Pow(BodyBones[3].Location.Z, 2))));
	float BLength = FMath::Sqrt(FMath::Pow(BodyBones[4].Location.X, 2) + (FMath::Pow(BodyBones[4].Location.Y, 2) + (FMath::Pow(BodyBones[4].Location.Z, 2))));

	angle = (FMath::Acos(Dot / (ALength*BLength)) * 180 / PI);  // Correct calculations
	return angle;
}

float UKinectBody::getLeftElbowAngle()
{
	float angle;
	float Dot = FVector::DotProduct(BodyBones[6].Location, BodyBones[7].Location);
	float ALength = FMath::Sqrt(FMath::Pow(BodyBones[6].Location.X, 2) + (FMath::Pow(BodyBones[6].Location.Y, 2) + (FMath::Pow(BodyBones[6].Location.Z, 2))));
	float BLength = FMath::Sqrt(FMath::Pow(BodyBones[7].Location.X, 2) + (FMath::Pow(BodyBones[7].Location.Y, 2) + (FMath::Pow(BodyBones[7].Location.Z, 2))));

	angle = (FMath::Acos(Dot / (ALength*BLength)) * 180 / PI);  // Correct calculations
	return angle;
}

void UKinectBody::SetupMovementLimits()
{

	RHandLatRaise->ClearMotionLimitData();
	LHandLatRaise->ClearMotionLimitData();
	RHandFrontRaise->ClearMotionLimitData();

	if (RHandLatRaise->IsMotionLimitSet() == true)
	{

		RHandLatRaise->StartSetup();

	}

	else 
	{

	}

}

void UKinectBody::MovementFocus()
{
	TrackRotation = true;
}


