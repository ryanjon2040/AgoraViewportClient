// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Math/IntPoint.h"
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

using FOnGitInfoUpdated = TDelegate<void(const bool bSuccess, const FString& BranchString, const FString& CommitString)>;

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

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable"))
	bool bShowGitInformation;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	bool bShowBranchName;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	bool bShowCommitHash;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	bool bShowBuildTime;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation", UIMin = 7, ClampMin = 7, UIMax = 40, ClampMax = 40))
	uint8 CommitHashLength;

	UPROPERTY(VisibleAnywhere, Category = "Agora Viewport Client - Git")
	FText GitText;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	TEnumAsByte<EHorizontalAlignment> GitHorizontalAlignment;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	TEnumAsByte<EVerticalAlignment> GitVerticalAlignment;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	FIntPoint GitPadding;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	FLinearColor GitColor;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	FLinearColor GitShadowColor;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	FVector2D GitShadowOffset;

	UPROPERTY(Config, EditAnywhere, Category = "Agora Viewport Client - Git", meta = (EditCondition = "bEnable && bShowGitInformation"))
	FSlateFontInfo GitFontInfo;

	TOptional<FString> BranchName;
	TOptional<FString> CommitHash;

	bool bIsRefreshingGitDetails;

public:

	UAgoraViewportClientSettings();

	static FORCEINLINE const UAgoraViewportClientSettings* Get()
	{
		const UAgoraViewportClientSettings* ProxyDevSettings = CastChecked<UAgoraViewportClientSettings>(UAgoraViewportClientSettings::StaticClass()->GetDefaultObject());
		return ProxyDevSettings;
	}

	static FORCEINLINE UAgoraViewportClientSettings* GetMutable()
	{
		UAgoraViewportClientSettings* ProxyDevSettings = CastChecked<UAgoraViewportClientSettings>(UAgoraViewportClientSettings::StaticClass()->GetDefaultObject());
		return ProxyDevSettings;
	}

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
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
	
	FORCEINLINE FAgoraViewportText GetGitText() const
	{
		FAgoraViewportText GitDetails;
		GitDetails.bEnabled = bShowGitInformation;
		GitDetails.Color = GitColor;
		GitDetails.Padding = GitPadding;
		GitDetails.Text = GitText;
		GitDetails.SetFontInfo(GitFontInfo);
		GitDetails.HorizontalAlignment = GitHorizontalAlignment;
		GitDetails.VerticalAlignment = GitVerticalAlignment;
		GitDetails.ShadowColor = GitShadowColor;
		GitDetails.ShadowOffset = GitShadowOffset;
		return GitDetails;
	}

	UFUNCTION(BlueprintPure, Category = AgoraViewportClient)
	static bool GetProjectGitBranchName(FText& OutBranchName);

	UFUNCTION(BlueprintPure, Category = AgoraViewportClient)
	static bool GetProjectGitCommitHash(FText& OutCommitHash);

	void RefreshGitDetails();

private:

	static bool Internal_ExecGit(const TCHAR* Params, FString& OutStdOut, FString& OutStdErr);
	static void Internal_SetGit(FOnGitInfoUpdated OnGitInfoUpdated);
};
