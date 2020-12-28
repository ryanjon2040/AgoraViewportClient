// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

#include "AgoraGameViewportClient.h"
#include "AgoraViewportClientSettings.h"
#include "Runtime/SlateCore/Public/Styling/CoreStyle.h"
#include "Runtime/Slate/Public/Widgets/Text/STextBlock.h"

#define GET_DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

void SAgoraWatermarkCompoundWidget::Construct(const FArguments& InArgs)
{
	const UAgoraViewportClientSettings* AgoraViewportClientSettings = UAgoraViewportClientSettings::Get();
	const FAgoraViewportText TitleText = AgoraViewportClientSettings->GetTitle();
	const FAgoraViewportText CreatedBy = AgoraViewportClientSettings->GetCreatedBy();
	const FAgoraViewportText SystemDetails = AgoraViewportClientSettings->GetSystemDetails();

	FFormatNamedArguments Args;
	Args.Add("CreatedBy", CreatedBy.Text);
	Args.Add("EngineBuild", AgoraViewportClientSettings->GetUnrealEngineVersionText());
	const FText CreatedByText = AgoraViewportClientSettings->AddBuiltWithUE4VersionToCreatedBy() ?
		FText::Format(FText::AsCultureInvariant("{CreatedBy}\n{EngineBuild}"), Args) : CreatedBy.Text;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.Padding(TitleText.Padding.X, TitleText.Padding.Y)
		.VAlign(TitleText.VerticalAlignment)
		.HAlign(TitleText.HorizontalAlignment)
		[
			SNew(STextBlock)
			.Visibility(TitleText.bEnabled ? EVisibility::HitTestInvisible : EVisibility::Collapsed)
			.Font(GET_DEFAULT_FONT("BoldCondensed", TitleText.FontSize)) // Style from SlateEditorStyle.cpp @line: 4355 (as of 4.22)
			.ColorAndOpacity(TitleText.Color)
			.ShadowColorAndOpacity(TitleText.ShadowColor)
			.ShadowOffset(TitleText.ShadowOffset)
			.Text(TAttribute<FText>(TitleText.Text))
		]
		
		+ SOverlay::Slot()
		.Padding(CreatedBy.Padding.X, CreatedBy.Padding.Y)
		.VAlign(CreatedBy.VerticalAlignment)
		.HAlign(CreatedBy.HorizontalAlignment)
		[
			SNew(STextBlock)
			.Visibility(CreatedBy.bEnabled ? EVisibility::HitTestInvisible : EVisibility::Collapsed)
			.Font(GET_DEFAULT_FONT("BoldCondensed", CreatedBy.FontSize))
			.ColorAndOpacity(CreatedBy.Color)
			.ShadowColorAndOpacity(CreatedBy.ShadowColor)
			.ShadowOffset(CreatedBy.ShadowOffset)
			.Text(TAttribute<FText>(CreatedByText))
		]

		+ SOverlay::Slot()
        .Padding(SystemDetails.Padding.X, SystemDetails.Padding.Y)
        .VAlign(SystemDetails.VerticalAlignment)
        .HAlign(SystemDetails.HorizontalAlignment)
        [
            SNew(STextBlock)
            .Visibility(SystemDetails.bEnabled ? EVisibility::HitTestInvisible : EVisibility::Collapsed)
            .Font(GET_DEFAULT_FONT("BoldCondensed", SystemDetails.FontSize))
            .ColorAndOpacity(SystemDetails.Color)
            .ShadowColorAndOpacity(SystemDetails.ShadowColor)
            .ShadowOffset(SystemDetails.ShadowOffset)
            .Text(TAttribute<FText>(SystemDetails.Text))
        ]
	];
}

void UAgoraGameViewportClient::Activated(FViewport* InViewport, const FWindowActivateEvent& InActivateEvent)
{
	Super::Activated(InViewport, InActivateEvent);
	if (UAgoraViewportClientSettings::Get()->IsEnabled())
	{
		ShowWatermarkOnScreen();
	}
}

void UAgoraGameViewportClient::ShowWatermarkOnScreen()
{
	if (AgoraWatermark.IsValid())
	{
		return;
	}

	AgoraWatermark = SNew(SAgoraWatermarkCompoundWidget);
	AgoraWatermark->SetVisibility(EVisibility::HitTestInvisible);
	AddViewportWidgetContent(AgoraWatermark.ToSharedRef(), MAX_int32);
}
