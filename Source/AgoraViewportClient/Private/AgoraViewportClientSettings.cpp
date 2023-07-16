// Created by Satheesh. Copyright 2020.
// https://twitter.com/ryanjon2040. Discord - ryanjon2040#5319
// Please support Agora by becoming a Patron: https://www.patreon.com/ryanjon2040

#include "AgoraViewportClientSettings.h"
#include "HardwareInfo.h"
#include "GenericPlatform/GenericPlatformDriver.h"
#include "Launch/Resources/Version.h"
#include "Runtime/SlateCore/Public/Styling/CoreStyle.h"
#include "RHI/Public/RHI.h"

FSlateFontInfo FAgoraViewportText::GetFontInfo() const
{
	if (!IsValid(FontInfo.FontObject))
	{
		return FCoreStyle::GetDefaultFontStyle("BoldCondensed", FontInfo.Size);
	}

	return FontInfo;
}

UAgoraViewportClientSettings::UAgoraViewportClientSettings()
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

	bShowGitInformation = bShowBranchName = bShowCommitHash = bShowBuildTime = bShowTotalCommitsAsChangelist = true;
	CommitHashLength = 7;
	bIsRefreshingGitDetails = false;
	GitVerticalAlignment = VAlign_Bottom;
	GitHorizontalAlignment = HAlign_Center;
	GitPadding = FIntPoint(10);
	GitColor = FLinearColor(0.8, 0.8f, 0.8f, 0.2f);
	GitFontInfo = FCoreStyle::GetDefaultFontStyle("BoldCondensed", 14);

	RefreshGitDetails();

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

#if WITH_EDITOR
void UAgoraViewportClientSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const auto BranchMember = GET_MEMBER_NAME_CHECKED(UAgoraViewportClientSettings, bShowBranchName);
	const auto CommitMember = GET_MEMBER_NAME_CHECKED(UAgoraViewportClientSettings, bShowCommitHash);
	const auto BuildTimeMember = GET_MEMBER_NAME_CHECKED(UAgoraViewportClientSettings, bShowBuildTime);
	const auto CommitLengthMember = GET_MEMBER_NAME_CHECKED(UAgoraViewportClientSettings, CommitHashLength);
	const auto ShowTotalCommitsAsChangelist = GET_MEMBER_NAME_CHECKED(UAgoraViewportClientSettings, bShowTotalCommitsAsChangelist);
	if (PropertyChangedEvent.GetPropertyName() == BranchMember
		|| PropertyChangedEvent.GetPropertyName() == CommitMember
		|| PropertyChangedEvent.GetPropertyName() == BuildTimeMember
		|| PropertyChangedEvent.GetPropertyName() == CommitLengthMember
		|| PropertyChangedEvent.GetPropertyName() == ShowTotalCommitsAsChangelist)
	{
		RefreshGitDetails();
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FText UAgoraViewportClientSettings::GetUnrealEngineVersionText()
{
	static FFormatNamedArguments Args;
	Args.Add(TEXT("UnrealEngineVersion"), FText::FromString(FString(ENGINE_VERSION_STRING)));
	static const FText BuiltText = FText::Format(NSLOCTEXT("Agora", "AgoraViewportClientEngineVersion", "Built using Unreal Engine {UnrealEngineVersion}"), Args);
	return BuiltText;
}

bool UAgoraViewportClientSettings::GetProjectGitBranchName(FText& OutBranchName)
{
	const auto Settings = Get();
	if (Settings->BranchName.IsSet())
	{
		OutBranchName = FText::FromString(Settings->BranchName.GetValue());
		return true;
	}

	OutBranchName = FText();
	return false;
}

bool UAgoraViewportClientSettings::GetProjectGitCommitHash(FText& OutCommitHash)
{
	const auto Settings = Get();
	if (Settings->CommitHash.IsSet())
	{
		OutCommitHash = FText::FromString(Settings->CommitHash.GetValue());
		return true;
	}

	OutCommitHash = FText();
	return false;
}

bool UAgoraViewportClientSettings::GetProjectGitCommitCount(FText& OutCommitCount)
{
	const auto Settings = Get();
	if (Settings->CommitCount.IsSet())
	{
		OutCommitCount = FText::FromString(Settings->CommitCount.GetValue());
		return true;
	}

	OutCommitCount = FText();
	return false;
}

void UAgoraViewportClientSettings::RefreshGitDetails()
{
	if (bIsRefreshingGitDetails)
	{
		return;
	}

	bIsRefreshingGitDetails = true;
	GitText = INVTEXT("Refreshing. Please wait...");
	BranchName.Reset();
	CommitHash.Reset();
	CommitCount.Reset();
	
	FOnGitInfoUpdated OnGitInfoUpdated;
	OnGitInfoUpdated.BindLambda([&](const bool bSuccess, const FString& BranchString, const FString& CommitString, const FString& CommitCountString)
	{
		GitText = FText();
		if (bSuccess && bShowGitInformation)
		{
			BranchName = BranchString;
			CommitHash = CommitString;
			FString NewGitText;
			if (bShowBranchName && !bShowCommitHash)
			{
				NewGitText = BranchString;
			}
			else if (bShowCommitHash && !bShowBranchName)
			{
				NewGitText = CommitString;
			}
			else
			{
				NewGitText = FString::Printf(TEXT("%s-%s"), *BranchString, *CommitString);
			}

			if (bShowTotalCommitsAsChangelist)
			{
				NewGitText += FString::Printf(TEXT("-CL%s"), *CommitCountString);
			}

			if (bShowBuildTime)
			{
				NewGitText += FString::Printf(TEXT("-%s"), *FDateTime::UtcNow().ToString());
			}

			GitText = FText::FromString(NewGitText);
		}

		bIsRefreshingGitDetails = false;
	});
	
	Internal_SetGit(OnGitInfoUpdated);
}

bool UAgoraViewportClientSettings::Internal_ExecGit(const TCHAR* Params, FString& OutStdOut, FString& OutStdErr)
{
#if WITH_EDITOR
	if (!GEngine->IsEditor())
	{
		return false;
	}

	int32* ReturnCode = nullptr;
	return FPlatformProcess::ExecProcess(TEXT("git"), Params, ReturnCode, &OutStdOut, &OutStdErr, *FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()));
#endif
	return false;
}

void UAgoraViewportClientSettings::Internal_SetGit(FOnGitInfoUpdated OnGitInfoUpdated)
{
#if WITH_EDITOR
	static const auto IniFile = Get()->GetDefaultConfigFilename();
#else
	static const auto IniFile = GGameIni;
#endif

	static const FString SectionName = "/Script/AgoraViewportClient.AgoraViewportClientSettings";
	static const TCHAR* CommitKey = TEXT("Commit");
	static const TCHAR* BranchKey = TEXT("Branch");
	static const TCHAR* CommitCountKey = TEXT("CL");

#if WITH_EDITOR
	static const FString GitDir = FPaths::Combine(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()), TEXT(".git"));
	if (!FPaths::DirectoryExists(GitDir))
	{
		OnGitInfoUpdated.ExecuteIfBound(false, "", "", "");
	}

	const uint8 CommitLength = Get()->CommitHashLength;
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [OnGitInfoUpdated, CommitLength]
	{
		if (GEngine->IsEditor())
		{
			FString StdOut;
			FString StdErr;

			Internal_ExecGit(TEXT("status --short"), StdOut, StdErr);
			StdOut.TrimStartAndEndInline();
			const FString GitOut = StdOut;
			StdOut.Reset();

			const FString GitDescribe = FString::Printf(TEXT("describe --always --abbrev=%i"), CommitLength);
			Internal_ExecGit(*GitDescribe, StdOut, StdErr);
			StdOut.TrimStartAndEndInline();

			const FString CommitString = GitOut.IsEmpty() ? StdOut : FString::Printf(TEXT("%s-dirty"), *StdOut);

			Internal_ExecGit(TEXT("symbolic-ref --short HEAD"), StdOut, StdErr);
			StdOut.TrimStartAndEndInline();
			const FString BranchString = StdOut.IsEmpty() ? "invalid-branch" : StdOut;
			StdOut.Reset();

			FString CommitCountString = "0";
			if (BranchString != "invalid-branch")
			{
				const FString RevList = FString::Printf(TEXT("rev-list --count %s"), *BranchString);
				Internal_ExecGit(*RevList, StdOut, StdErr);				
				StdOut.TrimStartAndEndInline();
				CommitCountString = StdOut;
			}

			GConfig->SetText(*SectionName, CommitKey, FText::FromString(CommitString), IniFile);
			GConfig->SetText(*SectionName, BranchKey, FText::FromString(BranchString), IniFile);
			GConfig->SetText(*SectionName, CommitCountKey, FText::FromString(CommitCountString), IniFile);

			AsyncTask(ENamedThreads::GameThread, [OnGitInfoUpdated, BranchString, CommitString, CommitCountString]
			{
				GetMutable()->SaveConfig(CPF_Config, *IniFile);
				OnGitInfoUpdated.ExecuteIfBound(true, BranchString, CommitString, CommitCountString);
			});
		}
	});
#else
	FText BranchValue;
	const bool bBranchFound = GConfig->GetText(*SectionName, BranchKey, BranchValue, IniFile);

	FText CommitValue;
	const bool bCommitFound = GConfig->GetText(*SectionName, CommitKey, CommitValue, IniFile);

	FText CommitCountValue;
	GConfig->GetText(*SectionName, CommitCountKey, CommitCountValue, IniFile);
	OnGitInfoUpdated.ExecuteIfBound(bBranchFound && bCommitFound, BranchValue.ToString(), CommitValue.ToString(), CommitCountValue.ToString());
#endif
}
