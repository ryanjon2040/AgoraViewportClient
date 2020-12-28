// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

#include "AgoraViewportClient.h"

#define LOCTEXT_NAMESPACE "FAgoraViewportClient"

DEFINE_LOG_CATEGORY_STATIC(LogAgoraViewportClient, All, All)

void FAgoraViewportClient::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogAgoraViewportClient, Log, TEXT("Agora Viewport Client module started."));
}

void FAgoraViewportClient::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogAgoraViewportClient, Log, TEXT("Agora Viewport Client module shutdown."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAgoraViewportClient, AgoraViewportClient)
