// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorDiscordRichPresence.h"
#include "discord-files/discord.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Runtime/Core/Public/Misc/App.h"
#if WITH_EDITOR
#include "UnrealEd.h"
//#include "Toolkits/AssetEditorToolkit.h"
//#include "AssetEditorSubsystem.h"
#include "Interfaces/IPluginManager.h"
#include "Framework/Application/SlateApplication.h"
#endif
//#include "Editor/StaticMeshEditor/Public/StaticMeshEditorModule.h"
//#include "Editor/EditorFramework/Public/Tools/Modes.h"
#include "Editor/EditorFramework/Public/EditorModes.h"
#include "EditorModeManager.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FEditorDiscordRichPresenceModule"

discord::Core* fcore{};

void FEditorDiscordRichPresenceModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// Get the base directory of this plugin
#if WITH_EDITOR
	//new
	UAssetEditorSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (AssetSubsystem)
	{
		AssetSubsystem->OnAssetEditorRequestedOpen().AddRaw(this, &FEditorDiscordRichPresenceModule::NotifyAssetOpened);
	}

	//ULevelEditorSubsystem* LevelSubsystem = GEditor->GetLevelEd

/*
	IStaticMeshEditorModule* StaticMeshEditorModule = &FModuleManager::LoadModuleChecked<IStaticMeshEditorModule>("StaticMeshEditor");
	{
		FAssetEditorManager::Get().OnAssetOpenedInEditor().AddRaw(this, &FEditorDiscordRichPresenceModule::NotifyAssetOpened);
	}*/

	//old
	FString BaseDir = IPluginManager::Get().FindPlugin("EditorDiscordRichPresence")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("ThirdParty/Discord/x64/Release/discord_game_sdk.dll"));
	DLLHandle = FPlatformProcess::GetDllHandle(*LibraryPath);
#endif // PLATFORM_WINDOWS

#endif

	/*
	Grab that Client ID from earlier
	Discord.CreateFlags.Default will require Discord to be running for the game to work
	If Discord is not running, it will:
	1. Close your game
	2. Open Discord
	3. Attempt to re-open your game
	Step 3 will fail when running directly from the Unreal Engine editor
	Therefore, always keep Discord running during tests, or use Discord.CreateFlags.NoRequireDiscord*/

		auto result = discord::Core::Create(1023921525285457950, DiscordCreateFlags_NoRequireDiscord, &fcore);
		//discord::Activity activity{};
		activity.SetType(discord::ActivityType::Streaming);


		//Get project name from project settings, if blank get it from FApp (module name?)
		//FString ProjectName;

		//get project name from project config
		GConfig->GetString(
			TEXT("/Script/EngineSettings.GeneralProjectSettings"),
			TEXT("ProjectName"),
			ProjectName,
			GGameIni
		);

		//check if its blank and use FApp to get it
		if (ProjectName.IsEmpty())
		{
			ProjectName = FApp::GetProjectName();

			if (ProjectName.IsEmpty())
			{
				ProjectName = "Untitled Project";
			}
		}

		//get engine version
		//FString VersionNum = "Engine Version: ";
		VersionNum.AppendInt(ENGINE_MAJOR_VERSION);
		VersionNum.Append(".").AppendInt(ENGINE_MINOR_VERSION);
		VersionNum.Append(".").AppendInt(ENGINE_PATCH_VERSION);


		//combine with engine ver
		ProjectName.Append(" | ");
		ProjectName.Append(VersionNum);

		//ini status
		EditorStatus = "Level Editor";

		activity.SetDetails(TCHAR_TO_ANSI(*ProjectName));
		activity.SetState(TCHAR_TO_ANSI(*EditorStatus));

		discord::Timestamp time = FDateTime::UtcNow().ToUnixTimestamp();
		activity.GetTimestamps().SetStart(time);

		activity.GetAssets().SetLargeImage("ue5");

		if (fcore)
		{
			fcore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Discord Error: fcore is null"));
		}


		//Tick for Discord update
		TickDelegate = FTickerDelegate::CreateRaw(this, &FEditorDiscordRichPresenceModule::Tick);
		//TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
		FTSTicker::GetCoreTicker().AddTicker(TickDelegate);

		//FSlateApplication* SApp = &FSlateApplication::Get();
		//FText WTitle = SApp->GetActiveTopLevelRegularWindow()->GetTitle();

	
		//if (SApp)
		//{
			//FString WTitle = SApp->GetTitle().ToString();

			//UE_LOG(LogTemp, Log, TEXT("ProjectName: %s"), *WTitle);
		//}
		//else
		//{
			//UE_LOG(LogTemp, Log, TEXT("ProjectName: NULL"));
		//}


		//del tests
		FEditorDelegates::BeginPIE.AddRaw(this, &FEditorDiscordRichPresenceModule::PlayInEditorBegin);
		FEditorDelegates::EndPIE.AddRaw(this, &FEditorDiscordRichPresenceModule::PlayInEditorEnd);
		//FEditorDelegates::selec.AddRaw(this, &FEditorDiscordRichPresenceModule::EditorModeIDExit);

		//FName ModName = TEXT("LevelEditor");
		FLevelEditorModule& levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		levelEditor.OnActorSelectionChanged().AddRaw(this, &FEditorDiscordRichPresenceModule::OnSelectionChanged);


		//FEditorDelegates::EditorModeIDEnter.AddRaw(this, &FEditorDiscordRichPresenceModule::EditorModeIDExit);
		//AssetSubsystem->OnEditorModeRegistered().AddRaw(this,&FEditorDiscordRichPresenceModule::EditorModeIDExit);
/*
		FEditorModeInfo ModeInfo;
		AssetSubsystem->FindEditorModeInfo(FBuiltinEditorModes::EM_Level, ModeInfo);
		ModeInfo.*/
}

void FEditorDiscordRichPresenceModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
#if WITH_EDITOR
	//FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	FPlatformProcess::FreeDllHandle(DLLHandle);
#endif
}

bool FEditorDiscordRichPresenceModule::Tick(float DeltaTime)
{
	//FEditorDiscordRichPresenceModule::Update(DeltaTime);
	//UpdateFocusedWindowStatus();
	UpdateDiscordStatus();





	FSlateApplication* SApp = &FSlateApplication::Get();
	if (SApp)
	{
		//FText WTitle = SApp->GetActiveTopLevelRegularWindow()->;
		//UE_LOG(LogTemp, Log, TEXT("ProjectName: %s"), *WTitle.ToString());
	}

	if (::fcore)
	{
		::fcore->RunCallbacks();
	}


	return true;
}


void FEditorDiscordRichPresenceModule::UpdateFocusedWindowStatus()
{
	//FAssetEditorManager assetEditorManager = FAssetEditorManager::Get();
	//TArray<UObject*> editedAssets = assetEditorManager.GetAllEditedAssets();
	//TArray<IAssetEditorInstance*> openedEditors
	//IAssetEditorInstance* focusedEditor = nullptr;


	UAssetEditorSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (AssetSubsystem)
	{
		TArray<UObject*> editedAssets = AssetSubsystem->GetAllEditedAssets();
		TArray<IAssetEditorInstance*> openedEditors;

		for (UObject* editedAsset : editedAssets)
		{
			if (editedAsset)
			{
				openedEditors.Add(AssetSubsystem->FindEditorForAsset(editedAsset, false));
			}

		}

		double maxLastActivationTime = 0.0;

		for (IAssetEditorInstance* openedEditor : openedEditors)
		{
			if (openedEditor && openedEditor->GetLastActivationTime() > maxLastActivationTime)
			{
				maxLastActivationTime = openedEditor->GetLastActivationTime();
				FocusedEditor = openedEditor;

				//UE_LOG(LogTemp, Log, TEXT("Discord: %f"), maxLastActivationTime);
			}
		}



		if (!FocusedEditor && EditorStatus != "Level Editor")
		{
			EditorStatus = "Level Editor";
			activity.SetState(TCHAR_TO_ANSI(*EditorStatus));
			activity.GetAssets().SetLargeImage("editor");

			//UE_LOG(LogTemp, Log, TEXT("Discord: %s"), *EditorStatus);
		}
		else if(FocusedEditor && FName::NameToDisplayString(FocusedEditor->GetEditorName().ToString(), false) != EditorStatus /*&& !GLevelEditorModeTools().IsOnlyVisibleActiveMode(FBuiltinEditorModes::EM_Default)*/)
		{

			EditorStatus = FName::NameToDisplayString(FocusedEditor->GetEditorName().ToString(), false);
			activity.SetState(TCHAR_TO_ANSI(*EditorStatus));


			//UE_LOG(LogTemp, Log, TEXT("Discord: %s"), *EditorStatus);
		}
		else/* if(FocusedEditor && !GLevelEditorModeTools().IsModeActive(FBuiltinEditorModes::EM_Level))*/
		{
			FEditorModeID EID = FBuiltinEditorModes::EM_Level;



			if (GLevelEditorModeTools().IsOnlyVisibleActiveMode(FBuiltinEditorModes::EM_Default) && EditorStatus != "Level Editor")
			{
				EditorStatus = "Level Editor";
				activity.SetState(TCHAR_TO_ANSI(*EditorStatus));/**/
			}
		}

		//clear
		FocusedEditor = nullptr;
		//UE_LOG(LogTemp, Log, TEXT("Discord: %s"), *EditorStatus);
		//update
		if (fcore)
		{
			fcore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
	}
}


void FEditorDiscordRichPresenceModule::UpdateDiscordStatus()
{
	//PIE
	if (bIsInPIE)
	{
		EditorStatus = "Playing In Editor";
		activity.SetState(TCHAR_TO_ANSI(*EditorStatus));

		if (fcore)
		{
			fcore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}

		return;
	}

	//if not in pie check for open assets that have a greater last active time

	//Get subsystem
	UAssetEditorSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (AssetSubsystem)
	{
		//get all opened assets
		TArray<UObject*> editedAssets = AssetSubsystem->GetAllEditedAssets();
		TArray<IAssetEditorInstance*> openedEditors;

		for (UObject* editedAsset : editedAssets)
		{
			if (editedAsset)
			{
				openedEditors.Add(AssetSubsystem->FindEditorForAsset(editedAsset, false));
			}
		}

		double OldLastActiveTime = LastActiveTime;

		for (IAssetEditorInstance* openedEditor : openedEditors)
		{
			if (openedEditor && openedEditor->GetLastActivationTime() > LastActiveTime)
			{
				LastActiveTime = openedEditor->GetLastActivationTime();
				FocusedEditor = openedEditor;
			}
		}

		if (bIsLevelEditor)
		{
			if (LastActiveTime > OldLastActiveTime && FocusedEditor)
			{
				bIsLevelEditor = false;
				EditorStatus = FName::NameToDisplayString(FocusedEditor->GetEditorName().ToString(), false);

				activity.GetAssets().SetLargeImage(TCHAR_TO_ANSI(*GetImageKey(FocusedEditor->GetEditorName())));
				//UE_LOG(LogTemp, Warning, TEXT("Discord: %s"), *FocusedEditor->GetEditorName().ToString());
			}
			else
			{
				//level editor
				EditorStatus = "Level Editor";
				activity.GetAssets().SetLargeImage("editor");
			}
		}
		else
		{
			if(FocusedEditor)
			{
				EditorStatus = FName::NameToDisplayString(FocusedEditor->GetEditorName().ToString(), false);

				activity.GetAssets().SetLargeImage(TCHAR_TO_ANSI(*GetImageKey(FocusedEditor->GetEditorName())));
				//UE_LOG(LogTemp, Warning, TEXT("Discord: %s"), *FocusedEditor->GetEditorName().ToString());
			}
		}

		activity.SetState(TCHAR_TO_ANSI(*EditorStatus));

		if (fcore)
		{
			fcore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
	}

	FocusedEditor = nullptr;
}

void FEditorDiscordRichPresenceModule::NotifyAssetOpened(UObject* Asset/*, IAssetEditorInstance* InInstance*/)
{
	//UE_LOG(LogTemp, Warning, TEXT("Discord Asset opened"));
}

void FEditorDiscordRichPresenceModule::PlayInEditorBegin(bool bIsSimulating)
{
	PrePIEStatus = EditorStatus;
	//bIsLevelEditor = false;
	bIsInPIE = true;
	//UE_LOG(LogTemp, Warning, TEXT("Discord BeginPIE"));
}

void FEditorDiscordRichPresenceModule::PlayInEditorEnd(bool bIsSimulating)
{
	//bIsLevelEditor = true;
	bIsInPIE = false;
	//UE_LOG(LogTemp, Warning, TEXT("Discord EndPIE"));
}

void FEditorDiscordRichPresenceModule::OnSelectionChanged(const TArray<UObject*>& Selection, bool MyBool)
{
	//LastActiveTime += 1;
	bIsLevelEditor = true;
	//UE_LOG(LogTemp, Warning, TEXT("Discord Selection changed"));
}

FString FEditorDiscordRichPresenceModule::GetImageKey(FName EditorName)
{
	FString KeyResult = "default";

	//blueprint
	if (EditorName == FName("BlueprintEditor"))
	{
		KeyResult = "bp";
	}

	//
	if (EditorName == FName("StaticMeshEditor"))
	{
		KeyResult = "mesh";
	}

	//
	if (EditorName == FName("TextureEditor"))
	{
		KeyResult = "texture";
	}

	//
	if (EditorName == FName("WidgetBlueprintEditor"))
	{
		KeyResult = "ui";
	}

	//
	if (EditorName == FName("MaterialEditor") || EditorName == FName("MaterialInstanceEditor"))
	{
		KeyResult = "mat";
	}

	//
	if (EditorName == FName("SkeletalMeshEditor"))
	{
		KeyResult = "mesh";
	}

	//
	if (EditorName == FName("EnumEditor"))
	{
		KeyResult = "bp";
	}

	//
	if (EditorName == FName("GenericAssetEditor"))
	{
		KeyResult = "default";
	}

	//
	if (EditorName == FName("SpriteEditor"))
	{
		KeyResult = "mat";
	}

	//
	if (EditorName == FName("SoundCueEditor") || EditorName == FName("SoundSubmixEditor") || EditorName == FName("SoundClassEditor"))
	{
		KeyResult = "sound";
	}

	UE_LOG(LogTemp, Warning, TEXT("DiscordRP: Set to %s"), *KeyResult);

	return KeyResult;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorDiscordRichPresenceModule, EditorDiscordRichPresence)