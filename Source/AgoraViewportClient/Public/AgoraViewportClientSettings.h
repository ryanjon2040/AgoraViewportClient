// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

#pragma once

#include "Engine/DeveloperSettings.h"
#include "HardwareInfo.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include "Launch/Resources/Version.h"
#include "Math/IntPoint.h"
#include "Runtime/SlateCore/Public/Styling/CoreStyle.h"
#include "AgoraViewportClientSettings.generated.h"

USTRUCT(BlueprintType)
struct FAgoraViewportText
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text")
	bool bEnabled;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled", MultiLine = "true"))
	FText Text;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	FIntPoint Padding;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	FLinearColor Color;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	FLinearColor ShadowColor;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	FVector2D ShadowOffset;

private:
	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	FSlateFontInfo FontInfo;

public:

	FORCEINLINE void SetFontInfo(const FSlateFontInfo& NewFontInfo)
	{
		FontInfo = NewFontInfo;
	}

	FORCEINLINE FSlateFontInfo GetFontInfo() const
	{
		if (!IsValid(FontInfo.FontObject))
		{
			return FCoreStyle::GetDefaultFontStyle("BoldCondensed", FontInfo.Size);
		}

		return FontInfo;
	}

	FAgoraViewportText() : HorizontalAlignment(HAlign_Center), VerticalAlignment(VAlign_Center)
	{
		bEnabled = true;
		Padding = FIntPoint(10, 10);
		Color = FLinearColor(0.8, 0.8f, 0.8f, 0.2f);
		ShadowColor = FLinearColor(0.f, 0.f, 0.f, 0.0);
		ShadowOffset = FVector2D::ZeroVector;
	}
};

UCLASS(config = Game, defaultconfig)
class AGORAVIEWPORTCLIENT_API UAgoraViewportClientSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	/** Enable or disable features.*/
	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client")
	uint8 bEnable : 1;
	
	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	uint8 bAddBuiltWithEngineVersionToCreatedBy : 1;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bEnable"))
	uint8 bAddSystemInfo : 1;

	UPROPERTY(VisibleAnywhere, Category="Agora Viewport Client")
	FText SystemInfo;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	TEnumAsByte<EHorizontalAlignment> SystemInfoHorizontalAlignment;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	TEnumAsByte<EVerticalAlignment> SystemInfoVerticalAlignment;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	FIntPoint SystemInfoPadding;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	FLinearColor SystemInfoColor;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	FLinearColor SystemInfoShadowColor;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	FVector2D SystemInfoShadowOffset;

	UPROPERTY(Config, EditAnywhere, Category="Agora Viewport Client", meta = (EditCondition = "bAddSystemInfo && bEnable"))
	FSlateFontInfo SystemFontInfo;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	FAgoraViewportText TitleText;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	FAgoraViewportText CreatedBy;

public:

	UAgoraViewportClientSettings()
	{
		bEnable = true;
		bAddBuiltWithEngineVersionToCreatedBy = true;
		
		TitleText.Text = NSLOCTEXT("Agora", "AgoraViewportClientTitle", "WORK IN PROGRESS - DOES NOT REPRESENT FINAL LOOK OF THE GAME");
		TitleText.HorizontalAlignment = HAlign_Center;
		TitleText.VerticalAlignment = VAlign_Top;
		TitleText.Padding = FIntPoint(10, 100);
		TitleText.SetFontInfo(FCoreStyle::GetDefaultFontStyle("BoldCondensed", 24)); // Style from SlateEditorStyle.cpp @line: 4355 (as of 4.22)
		
		CreatedBy.Text = NSLOCTEXT("Agora", "AgoraViewportClientCreatedBy", "Created by <YOUR_NAME_HERE>");
		CreatedBy.VerticalAlignment = VAlign_Bottom;
		CreatedBy.HorizontalAlignment = HAlign_Left;
		CreatedBy.Padding = FIntPoint(10, 10);
		CreatedBy.SetFontInfo(FCoreStyle::GetDefaultFontStyle("BoldCondensed", 16));

		FFormatNamedArguments Args;
		static const FGPUDriverInfo GPUDriverInfo = FPlatformMisc::GetGPUDriverInfo(GRHIAdapterName);
#if PLATFORM_WINDOWS
		Args.Add(TEXT("CpuBrand"), FText::FromString(FString(FWindowsPlatformMisc::GetCPUBrand())));
		Args.Add(TEXT("CpuCores"), FText::AsNumber(FWindowsPlatformMisc::NumberOfCores()));
		Args.Add(TEXT("GpuBrand"), FText::FromString(FWindowsPlatformMisc::GetPrimaryGPUBrand()));
#elif PLATFORM_MAC
		Args.Add(TEXT("CpuBrand"), FText::FromString(FString(FMacPlatformMisc::GetCPUBrand())));
		Args.Add(TEXT("CpuCores"), FText::AsNumber(FMacPlatformMisc::NumberOfCores()));
		Args.Add(TEXT("GpuBrand"), FText::FromString(FMacPlatformMisc::GetPrimaryGPUBrand()));
#elif PLATFORM_LINUX
		Args.Add(TEXT("CpuBrand"), FText::FromString(FString(FLinuxPlatformMisc::GetCPUBrand())));
		Args.Add(TEXT("CpuCores"), FText::AsNumber(FLinuxPlatformMisc::NumberOfCores()));
		Args.Add(TEXT("GpuBrand"), FText::FromString(FLinuxPlatformMisc::GetPrimaryGPUBrand()));
#else
		Args.Add(TEXT("CpuBrand"), FText::FromString(FString(FPlatformMisc::GetCPUBrand())));
		Args.Add(TEXT("CpuCores"), FText::AsNumber(FPlatformMisc::NumberOfCores()));
		Args.Add(TEXT("GpuBrand"), FText::FromString(FPlatformMisc::GetPrimaryGPUBrand()));
#endif
		
		Args.Add(TEXT("GpuDriver"), FText::FromString(GPUDriverInfo.UserDriverVersion));
		Args.Add(TEXT("RHI"), FText::FromString(FHardwareInfo::GetHardwareInfo(NAME_RHI)));

		bAddSystemInfo = true;
		SystemInfo = FText::Format(NSLOCTEXT("Agora", "AgoraViewportClientSystemDetails", "{CpuBrand} ({CpuCores} Cores)\n{GpuBrand} ({RHI})\nDriver: {GpuDriver}"), Args);
		SystemInfoVerticalAlignment = VAlign_Bottom;
		SystemInfoHorizontalAlignment = HAlign_Right;
		SystemInfoPadding = FIntPoint(10, 10);
		SystemInfoColor = FLinearColor(0.8, 0.8f, 0.8f, 0.2f);
		SystemFontInfo = FCoreStyle::GetDefaultFontStyle("BoldCondensed", 18);
	}

	static FORCEINLINE const UAgoraViewportClientSettings* Get()
	{
		const UAgoraViewportClientSettings* ProxyDevSettings = CastChecked<UAgoraViewportClientSettings>(UAgoraViewportClientSettings::StaticClass()->GetDefaultObject());
		return ProxyDevSettings;
	}

#if WITH_EDITOR
	virtual FName GetCategoryName() const override final { return TEXT("Agora"); }
	virtual FText GetSectionText() const override final { return FText::FromString("Agora Viewport Client"); }
#endif

	FORCEINLINE bool IsEnabled() const							{ return bEnable; }
	FORCEINLINE FAgoraViewportText GetTitle() const				{ return TitleText; }
	FORCEINLINE FAgoraViewportText GetCreatedBy() const			{ return CreatedBy; }
	FORCEINLINE bool AddBuiltWithUE4VersionToCreatedBy() const	{ return bAddBuiltWithEngineVersionToCreatedBy; }
	static FORCEINLINE FText GetUnrealEngineVersionText()
	{
		static FFormatNamedArguments Args;
		Args.Add(TEXT("UnrealEngineVersion"), FText::FromString(FString(ENGINE_VERSION_STRING)));
		static const FText BuiltText = FText::Format(NSLOCTEXT("Agora", "AgoraViewportClientEngineVersion", "Built using Unreal Engine {UnrealEngineVersion}"), Args);
		return BuiltText;
	}
	
	FORCEINLINE FAgoraViewportText GetSystemDetails() const
	{
		FAgoraViewportText SystemDetails;
		SystemDetails.bEnabled = bAddSystemInfo;
		SystemDetails.Color = SystemInfoColor;
		SystemDetails.Padding = SystemInfoPadding;
		SystemDetails.Text = SystemInfo;
		SystemDetails.SetFontInfo(SystemFontInfo);
		SystemDetails.HorizontalAlignment = SystemInfoHorizontalAlignment;
		SystemDetails.ShadowColor = SystemInfoShadowColor;
		SystemDetails.ShadowOffset = SystemInfoShadowOffset;
		SystemDetails.VerticalAlignment = SystemInfoVerticalAlignment;
		return SystemDetails;
	}
};
