// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "MetasoundFrontendRegistries.h"

class FMetaSoundToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	MetaSoundToolLibraryHandle;
};
