// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#include "KinectController.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"
#include "GameFramework/Actor.h"
#include "Input/Events.h"
#include "Framework/Application/SlateApplication.h"
#include "KinectInput.h"
#include "KinectBody.h"
#include <vector>

#include <sstream> //for std::stringstream 
#include <string>  //for std::string
#include <stdlib.h>

using namespace std;

int ControllerCount = 0;

const FKey FKeysKinect::KinectRightHandX("Right Hand Move X");
const FKey FKeysKinect::KinectLeftHandY("Left Hand Move Y");
const FKey FKeysKinect::KinectRightLatRaiseK("Right Hand Lateral Raise K");
const FKey FKeysKinect::KinectRightLatRaise("Right Hand Lateral Raise");
const FKey FKeysKinect::KinectLeftLatRaiseK("Left Hand Lateral Raise K");
const FKey FKeysKinect::KinectRightPunch("Right Punch");


bool EmitKeyUpEventForKey(FKey Key, int32 User, bool Repeat)
{
	FKeyEvent KeyEvent(Key, FSlateApplication::Get().GetModifierKeys(), User, Repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey Key, int32 User, bool Repeat)
{
	FKeyEvent KeyEvent(Key, FSlateApplication::Get().GetModifierKeys(), User, Repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey Key, float Value, int32 User, bool Repeat)
{
	FAnalogInputEvent AnalogInputEvent(Key, FSlateApplication::Get().GetModifierKeys(), User, Repeat, 0, 0, Value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}


UKinectController::UKinectController(const FObjectInitializer &ObjectInitializer) : UActorComponent(ObjectInitializer), Private(new FKinectControllerPrivate())
{
	
	waitableHandle = 0;
	m_pCoordinateMapper = nullptr;
	m_pColorCoordinates = nullptr;
	m_pColorCoordinatesAux = nullptr;
	m_pDepthCoordinates = nullptr;
	m_pDepthFrameReader = nullptr;
	m_pColorFrameReader = nullptr;
	m_pBodyFrameReader = nullptr;
	m_pBodyIndexFrameReader = nullptr;
	m_pInfraredFrameReader = nullptr;
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;	
	
	bodies.Empty();
	bodies.SetNum(KinectBodyCount);
	bodies.Shrink();

	for (int i = 0; i < KinectBodyCount; i++)
	{
		bodies[i] = NewObject<UKinectBody>();
		bodies[i]->AddToRoot();
		bodies[i]->BodyIndex = i;
	}

	HRESULT hr;

	BOOLEAN isOpen;

	if (Private->Sensor)

	{

		hr = Private->Sensor->get_IsOpen(&isOpen);

		if (SUCCEEDED(hr))

		{

			Private->Sensor->SubscribeIsAvailableChanged(&waitableHandle);

			hr = Private->Sensor->get_CoordinateMapper(&m_pCoordinateMapper);

			IDepthFrameSource* pDepthFrameSource = nullptr;

			hr = Private->Sensor->get_DepthFrameSource(&pDepthFrameSource);

			if (SUCCEEDED(hr))

			{

				hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);

			}

			SafeRelease(pDepthFrameSource);

			pDepthFrameSource = nullptr;

			if (ExtractColorData)

			{

				// Initialize the Kinect and get the color reader

				IColorFrameSource* pColorFrameSource = NULL;

				hr = Private->Sensor->get_ColorFrameSource(&pColorFrameSource);

				if (SUCCEEDED(hr))

				{

					hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);

				}

				SafeRelease(pColorFrameSource);

				pColorFrameSource = nullptr;

			}

			if (ExtractIRData)

			{

				// Initialize the Kinect and get the infrared reader

				IInfraredFrameSource* pInfraredFrameSource = nullptr;

				hr = Private->Sensor->get_InfraredFrameSource(&pInfraredFrameSource);

				if (SUCCEEDED(hr))

				{

					hr = pInfraredFrameSource->OpenReader(&m_pInfraredFrameReader);

				}

				SafeRelease(pInfraredFrameSource);

				pInfraredFrameSource = nullptr;

				// Additional functionality - Infrared Image processing and displaying

			}

			if (ExtractSkeletonData)

			{
				// Initialize the Kinect and get coordinate mapper and the body reader

				IBodyFrameSource* pBodyFrameSource = nullptr;

				hr = Private->Sensor->get_BodyFrameSource(&pBodyFrameSource);

				if (SUCCEEDED(hr))

				{

					hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);

				}

				SafeRelease(pBodyFrameSource);

				pBodyFrameSource = nullptr;

				IBodyIndexFrameSource* pBodyIndexFrameSource = nullptr;

				hr = Private->Sensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);

				if (SUCCEEDED(hr))

				{

					hr = pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);

				}

				SafeRelease(pBodyIndexFrameSource);

				pBodyIndexFrameSource = nullptr;

			}

			UE_LOG(LogTemp, Error, TEXT("Device Initialization Succesfull!"));

		}

	}
	
}

UKinectController::~UKinectController()
{
	waitableHandle = 0;
	m_pCoordinateMapper = nullptr;
	m_pColorCoordinates = nullptr;
	m_pColorCoordinatesAux = nullptr;
	m_pDepthCoordinates = nullptr;
	m_pDepthFrameReader = nullptr;
	m_pColorFrameReader = nullptr;
	m_pBodyFrameReader = nullptr;
	m_pBodyIndexFrameReader = nullptr;
	m_pInfraredFrameReader = nullptr;
	

	bodies.Empty();

	delete Private;
	Private = nullptr;
	this->RemoveFromRoot();
	
}

bool UKinectController::IsConnected()
{

	BOOLEAN available;

	Private->Sensor->get_IsAvailable(&available);

	if (available) 
	{
		isAvailable = true;
	}
	else
	{
		isAvailable = false;
	}

	return isAvailable;
}

void UKinectController::OnRegister()
{
	Super::OnRegister();

	if (Private->Sensor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Device Initialization Unsuccesfull"));

	}

	APawn* Character = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Character != nullptr)
	{
		UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UInputComponent>();
		PlayerInputComponent->BindAxis("Move Right Hand Lateral", this, &UKinectController::moveRight);
		PlayerInputComponent->BindAxis("Move Left Hand Lateral", this, &UKinectController::moveLeft);
		PlayerInputComponent->BindAxis("Shoulder Raise Key", this, &UKinectController::Jump);
		PlayerInputComponent->BindAxis("Right Punch Key", this, &UKinectController::Shoot);

	}

	ControllerCount++;

}

void UKinectController::OnUnregister()
{
	ControllerCount--;
	for (int i = 0; i < KinectBodyCount; i++)
	{
		bodies[i]->RemoveFromRoot();
	}
	bodies.Empty();
	Super::OnUnregister();
	UE_LOG(LogTemp, Log, TEXT("Unregistered Kinect Component."));
}

void UKinectController::TickComponent(float DeltaTime, enum ELevelTick TickType,
									FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!isPaused)
	{

		InterfaceEventTick(DeltaTime);
	
	}

}



//Main Event driven tick

void UKinectController::InterfaceEventTick(float DeltaTime)
{

	collectData(DeltaTime);

}

bool UKinectController::collectData(float DeltaTime)
{

	if (m_pBodyFrameReader == nullptr)
	{

		UE_LOG(LogTemp, Error, TEXT("Body Frame not found!"));

		return false;

	}


	IBodyFrame* pBodyFrame = NULL;

	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (FAILED(hr))
	{

		return false;

	}

	INT64 nTime = 0;

	hr = pBodyFrame->get_RelativeTime(&nTime);

	IBody* ppBodies[BODY_COUNT] = { 0 };

	if (FAILED(hr))
	{

		UE_LOG(LogTemp, Error, TEXT("Frame time not collected!"));

		return false;

	}

	hr = pBodyFrame->GetAndRefreshBodyData(countof(ppBodies), ppBodies);


	if (FAILED(hr))

	{

		UE_LOG(LogTemp, Error, TEXT("<Collect Data>   Body data not extracted!"));

		return false;

	}

	ProcessBody(nTime, BODY_COUNT, ppBodies, DeltaTime);

	for (int i = 0; i < countof(ppBodies); ++i)

	{

		SafeRelease(ppBodies[i]);

	}


	SafeRelease(pBodyFrame);

	return true;
}

bool UKinectController::ProcessBody(INT64 nTime, int noBodies, IBody** pBodies, float DeltaTime)
{

	HRESULT hr;

	tracking = false;

	int bodyNo;

	for (int i = 0; i < noBodies; ++i)

	{

		IBody* pBody = pBodies[i];

		if (pBody)

		{
			BOOLEAN bTracked = false;

			hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)

			{

				bodies[i]->updateBodyData(pBody, GetWorld()->GetTimeSeconds());

				tracking = true;

				bodyNo = i;

				if (bodies[i]->WasTracked == true)
				{
					if (bodies[i]->TrackRotation == false)
					{
						moveRight(bodies[i]->RHandLatRaise->LastAngle);

						moveLeft(bodies[i]->LHandLatRaise->LastAngle);
					}
					else
					{
						if (bodies[i]->RArmRotation->turnRight == true)
						{
							moveRight(bodies[i]->RArmRotation->LastAngle);
						}
						else
						{
							moveLeft(bodies[i]->RArmRotation->LastAngle);
						}
					}
				}
				if (bodies[i]->TrackRotation == false)
				{
					if (bodies[i]->RHandFrontRaise->isJump)

					{

						Jump(1);

					}
				}
				else
				{
					if (bodies[i]->RArmRotation->isJump)

					{

						Jump(1);

					}
					if (bodies[i]->RArmRotation->Punch)

					{

						Shoot(1);

					}
				}

			}

			else

			{

				bodies[i]->clearBodyData();

			}

		}

		else

		{

			UE_LOG(LogTemp, Error, TEXT("No body found!"));

			return false;

		}

	}

	if (tracking)

	{

		UKinectBody* body = bodies[bodyNo];

		FKinectJoint leftHand = body->BodyJoints[7];

	}

	return true;

}

FVector UKinectController::ExtractRightHandPosition()
{

	if (bodies[0] != nullptr)

	{

		return bodies[0]->BodyJoints[7].Location;

	}

	else

	{

		UE_LOG(LogTemp, Error, TEXT("No body found!"));

		return FVector(0, 0, 0);

	}
}

void UKinectController::moveRight(float x)
{

	EmitAnalogInputEventForKey(FKeysKinect::KinectRightLatRaise, x, 0, 0);
}

void UKinectController::moveLeft(float x)
{

	EmitAnalogInputEventForKey(FKeysKinect::KinectLeftLatRaiseK, x, 0, 0);

}

void UKinectController::Jump(float x)
{

	EmitAnalogInputEventForKey(FKeysKinect::KinectRightLatRaiseK, x, 0, 0);

}

void UKinectController::Shoot(float x)
{

	EmitAnalogInputEventForKey(FKeysKinect::KinectRightPunch, x, 0, 0);

}

bool UKinectController::Pause()
{

	if (!isPaused)
	{
		isPaused = true;
	}
	return isPaused;
}

UKinectBody* UKinectController::GetActiveBody()
{

	for (int i = 0; i < BODY_COUNT; ++i)

	{

		if (bodies[i]->IsTracked == true)
		{
			return bodies[i];
		}

	}

	return bodies[0];

}
