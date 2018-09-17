// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KinectPlugin.h"
#include "KinectController.generated.h"


class FKinectControllerPrivate
{
public:
	~FKinectControllerPrivate()
	{
		UE_LOG(LogTemp, Error, TEXT("Kinect sensor closed"));
		Sensor->Close();
		Sensor = nullptr;
	}
	FKinectControllerPrivate()
	{
		HRESULT hr;

		hr = GetDefaultKinectSensor(&Sensor);
		if (FAILED(hr)) {
			UE_LOG(LogTemp, Error, TEXT("Device Initialization failed"));
		}
		Sensor->Open();
	}

	IKinectSensor*				Sensor;

};

/**
* Kinect Controller class wrapped into an Actor Component.
*
* The Controller class is your main interface to the Kinect Motion Sensor.
* Create instance of this Controller class to access frames and required data.
*/
UCLASS(ClassGroup=Input, meta=(BlueprintSpawnableComponent))
class UKinectController : public UActorComponent 
{
	GENERATED_UCLASS_BODY()

public:
	UKinectController();
	~UKinectController();
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	void moveRight(float);

	void moveLeft(float);

	void Jump(float);

	void Shoot(float);

	bool collectData(float);

	bool ProcessBody(INT64, int, IBody**, float);
	/*
	* Reports whether this Controller is plugged in.
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "is connected"), Category = "Kinect Controller")
	bool IsConnected();
	/*Extracts position of Right Hand*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "right hand"), Category = "Kinect Controller")
	FVector ExtractRightHandPosition();
	/*Returns First Active Body*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "kinect body"), Category = "Kinect Controller")
	UKinectBody* GetActiveBody();

	/** Should Kinect extract color data? */
	UFUNCTION(BlueprintPure, meta = (Keywords = "is paused"), Category = "Kinect Controller")
	bool Pause();

	/** Should Kinect extract color data? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinect Sensor - Data")
		bool ExtractColorData = true;

	/** Should Kinect extract Infrared Data data? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinect Sensor - Data")
		bool ExtractIRData = true;

	/** Should Kinect extract Skeleton Data data? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinect Sensor - Data")
		bool ExtractSkeletonData = true;

	/** Should Kinect extract Depth Coordinates Data data? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinect Sensor - Data")
		bool ExtractDepthCoordinates = true;

	/** Should Kinect extract Depth Data data? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinect Sensor - Data")
		bool ExtractDepthData = true;

	bool isAvailable = false;

	bool isPaused = false;

private:
	class FKinectControllerPrivate* Private;

	ICoordinateMapper*			m_pCoordinateMapper;
	ColorSpacePoint*			m_pColorCoordinates;
	ColorSpacePoint*			m_pColorCoordinatesAux;
	DepthSpacePoint*			m_pDepthCoordinates;
	IDepthFrameReader*			m_pDepthFrameReader;
	IColorFrameReader*			m_pColorFrameReader;
	IBodyFrameReader*			m_pBodyFrameReader;
	IBodyIndexFrameReader*		m_pBodyIndexFrameReader;
	IInfraredFrameReader*		m_pInfraredFrameReader;
	WAITABLE_HANDLE				waitableHandle;
	TArray<UKinectBody*>		bodies;
	bool						tracking; // currently tracking body

	void InterfaceEventTick(float DeltaTime);
};