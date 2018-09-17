// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#include "KinectPlugin.h"
#include "Core.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "ModuleManager.h"
#include "KinectInput.h"
#include "IPluginManager.h"




#define LOCTEXT_NAMESPACE "FKinectPluginModule"

void FKinectPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("KinectPlugin")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/KinectPluginLibrary/Win64/Kinect20.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/KinectPluginLibrary/Mac/Release/libExampleLibrary.dylib"));
#endif // PLATFORM_WINDOWS

	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (LibraryHandle)
	{
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
	EKeys::AddKey(FKeyDetails(FKeysKinect::KinectRightHandX, LOCTEXT("RightHandMoveX", "Right Hand Move X"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(FKeysKinect::KinectLeftHandY, LOCTEXT("LeftHandMoveY", "Left Hand Move Y"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(FKeysKinect::KinectRightLatRaiseK, LOCTEXT("RightHandLateralRaiseK", "Right Hand Lateral Raise K"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(FKeysKinect::KinectRightLatRaise, LOCTEXT("RightHandLateralRaise", "Right Hand Lateral Raise"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(FKeysKinect::KinectLeftLatRaiseK, LOCTEXT("LeftHandLateralRaiseK", "Left Hand Lateral Raise K"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(FKeysKinect::KinectRightPunch, LOCTEXT("RightPunch", "Right Punch"), FKeyDetails::GamepadKey));
}

void FKinectPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(LibraryHandle);
	LibraryHandle = nullptr;

	IKinectSensor* sensor = nullptr;

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKinectPluginModule, KinectPlugin)