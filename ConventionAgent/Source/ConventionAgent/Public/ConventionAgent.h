// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleManager.h>
#include <FileManagerGeneric.h>

class FToolBarBuilder;
class FMenuBuilder;
class AssetInfoHandler;
struct FAssetInfo;
class FMessageLog;

class FConventionAgentModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedPtr<class FUICommandList> PluginCommands;

	void LostFocus(const FText& _Text, ETextCommit::Type _Type, bool _IsPre, FAssetInfo* _info);

	TSharedRef<class SVerticalBox> GeneratePluginWindow();
	FReply CheckAssets(FString _FromDir = "");
	FReply SaveConfig();

	FFileManagerGeneric* fileManager;
	AssetInfoHandler* assetInfoHandler;
	FMessageLog* caLog;
	const FText logname = FText::FromString("Convention Agent");
	const FString configFile = FPaths::ProjectPluginsDir() + "ConventionAgent/Config.json";
	const FName ConventionAgentTabName = FName("ConventionAgent");
};
