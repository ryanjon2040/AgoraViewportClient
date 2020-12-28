// Created by Satheesh (Twitter: @ryanjon2040 - Discord: ryanjon2040#5319). Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DeveloperSettings.h"
#include "HardwareInfo.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include "Launch/Resources/Version.h"
#include "AgoraViewportClientSettings.generated.h"

USTRUCT(BlueprintType)
struct FAgoraViewportText
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text")
	bool bEnabled;

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
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

	UPROPERTY(EditAnywhere, Category="Agora Viewport Text", meta = (EditCondition = "bEnabled"))
	int32 FontSize;

	FAgoraViewportText(): HorizontalAlignment(HAlign_Center), VerticalAlignment(VAlign_Center)
	{
		bEnabled = true;
		Padding = FIntPoint(10);
		Color = FLinearColor(0.8, 0.8f, 0.8f, 0.2f);
		ShadowColor = FLinearColor(0.f, 0.f, 0.f, 0.0);
		ShadowOffset = FVector2D::ZeroVector;
		FontSize = 14;
	}
};

UCLASS(config = AgoraViewportClient, defaultconfig)
class AGORAVIEWPORTCLIENT_API UAgoraViewportClientSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
private:

	/** Enable or disable features.*/
	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client")
	uint8 bEnable : 1;
	
	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	uint8 bAddBuiltWithEngineVersionToCreatedBy : 1;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	FAgoraViewportText TitleText;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	FAgoraViewportText CreatedBy;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client", meta = (EditCondition = "bEnable"))
	FAgoraViewportText SystemDetails;

public:

	UAgoraViewportClientSettings()
	{
		bEnable = true;
		bAddBuiltWithEngineVersionToCreatedBy = true;
		
		TitleText.Text = NSLOCTEXT("Agora", "AgoraViewportClientTitle", "WORK IN PROGRESS - DOES NOT REPRESENT FINAL LOOK OF THE GAME");
		TitleText.HorizontalAlignment = HAlign_Center;
		TitleText.VerticalAlignment = VAlign_Top;
		TitleText.Padding = FIntPoint(10, 100);
		TitleText.FontSize = 24;
		
		CreatedBy.Text = NSLOCTEXT("Agora", "AgoraViewportClientCreatedBy", "Created by <YOUR_NAME_HERE>");
		CreatedBy.VerticalAlignment = VAlign_Bottom;
		CreatedBy.HorizontalAlignment = HAlign_Left;
		CreatedBy.Padding = FIntPoint(10);
		CreatedBy.FontSize = 16;

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
		SystemDetails.Text = FText::Format(NSLOCTEXT("Agora", "AgoraViewportClientSystemDetails", "{CpuBrand} ({CpuCores} Cores)\n{GpuBrand} ({RHI})\nDriver: {GpuDriver}"), Args);
		SystemDetails.VerticalAlignment = VAlign_Bottom;
		SystemDetails.HorizontalAlignment = HAlign_Right;
		SystemDetails.Padding = FIntPoint(10);
		SystemDetails.FontSize = 18;
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
	FORCEINLINE FAgoraViewportText GetSystemDetails() const		{ return SystemDetails; }

	FORCEINLINE bool AddBuiltWithUE4VersionToCreatedBy() const	{ return bAddBuiltWithEngineVersionToCreatedBy; }
	static FORCEINLINE FText GetUnrealEngineVersionText()
	{
		static FFormatNamedArguments Args;
		Args.Add(TEXT("UnrealEngineVersion"), FText::FromString(FString(ENGINE_VERSION_STRING)));
		static const FText BuiltText = FText::Format(NSLOCTEXT("Agora", "AgoraViewportClientEngineVersion", "Built using Unreal Engine {UnrealEngineVersion}"), Args);
		return BuiltText;
	}
};
