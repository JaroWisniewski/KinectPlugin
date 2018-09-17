// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Kinect Plugin
// Created by Jaroslaw Wisniewski

#pragma once

#include "ModuleManager.h"
#include "KinectController.h"
#include "../ThirdParty/include/Kinect.h"

class FKinectPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	LibraryHandle;
};