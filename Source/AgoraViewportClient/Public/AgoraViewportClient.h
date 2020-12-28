// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAgoraViewportClient : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
