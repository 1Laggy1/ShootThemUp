// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "Modules/ModuleManager.h"
#include "Containers/Ticker.h"
#include "discord-files/discord.h"
#if WITH_EDITORONLY_DATA
#include "Editor/EditorFramework/Public/EditorModes.h"
#include "Interfaces/IPluginManager.h"
#endif

class FEditorDiscordRichPresenceModule : public IModuleInterface
{
public:

	FTickerDelegate TickDelegate;
	//FDelegateHandle TickDelegateHandle;
	FTSTicker::FDelegateHandle TickDelegateHandle;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool Tick(float DeltaTime);
	void UpdateFocusedWindowStatus();
	void UpdateDiscordStatus();


private:
	/** Handle to the test dll we will load */
	void* DLLHandle;

	//strings
	FString EditorStatus;
	FString ProjectName;
	FString VersionNum;

	IAssetEditorInstance* FocusedEditor = nullptr;
	double LastActiveTime = 0.0f;
	bool bIsInPIE;
	bool bIsLevelEditor;
	FString PrePIEStatus;

	void NotifyAssetOpened(UObject* Asset/*, IAssetEditorInstance* InInstance*/);
	//FString GetFriendlyEditorName(FName name);

	//UFUNCTION()
	void PlayInEditorBegin(bool bIsSimulating);
	void PlayInEditorEnd(bool bIsSimulating);
	void OnSelectionChanged(const TArray<UObject*>& Selection, bool MyBool);

	FString GetImageKey(FName EditorName);

	discord::Activity activity{};
};
