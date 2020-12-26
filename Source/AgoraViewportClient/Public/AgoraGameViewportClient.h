// Created by Satheesh (Twitter: @ryanjon2040 - Discord: ryanjon2040#5319). Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "Runtime/SlateCore/Public/Widgets/SCompoundWidget.h"
#include "AgoraGameViewportClient.generated.h"

class SAgoraWatermarkCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAgoraWatermarkCompoundWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		return EVisibility::Visible;
	}

	TSharedPtr<SOverlay> WatermarkOverlay;
};

UCLASS()
class AGORAVIEWPORTCLIENT_API UAgoraGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:

	virtual void Activated(FViewport* InViewport, const FWindowActivateEvent& InActivateEvent) override;
	
	TSharedPtr<SAgoraWatermarkCompoundWidget> AgoraWatermark;
	void ShowWatermarkOnScreen();
	
};
