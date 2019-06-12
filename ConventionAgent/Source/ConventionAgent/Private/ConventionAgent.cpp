// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ConventionAgent.h"
#include "ConventionAgentStyle.h"
#include "ConventionAgentCommands.h"
#include <LevelEditor.h>
#include <Framework/MultiBox/MultiBoxBuilder.h>
#include <Engine/AssetManager.h>
#include <MessageLog.h>

#include <Widgets/Docking/SDockTab.h>
#include <Widgets/Layout/SBox.h>
#include <Widgets/Text/STextBlock.h>
#include <Widgets/Input/SButton.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Layout/SSpacer.h>
#include <Widgets/Input/SEditableTextBox.h>

#include "AssetInfoHandler.h"
#include "AssetNamingChecker.h"

#define LOCTEXT_NAMESPACE "FConventionAgentModule"

DEFINE_LOG_CATEGORY_STATIC(LogConventionAgent, Log, All);

void FConventionAgentModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FConventionAgentStyle::Initialize();
	FConventionAgentStyle::ReloadTextures();

	FConventionAgentCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FConventionAgentCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FConventionAgentModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FConventionAgentModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FConventionAgentModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ConventionAgentTabName, FOnSpawnTab::CreateRaw(this, &FConventionAgentModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FConventionAgentTabTitle", "Convention Agent"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// Create logging tab
	caLog = new FMessageLog(FName(*logname.ToString()));
	caLog->NewPage(logname);

	fileManager = new FFileManagerGeneric();
	assetInfoHandler = new AssetInfoHandler(configFile);
	if (assetInfoHandler->checkOnStartup) {
		CheckAssets();
	}
}

void FConventionAgentModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	assetInfoHandler->SaveInfoToFile(configFile);

	FConventionAgentStyle::Shutdown();

	FConventionAgentCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ConventionAgentTabName);
}

TSharedRef<SDockTab> FConventionAgentModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedRef<SDockTab> dockContainer = SNew(SDockTab).TabRole(ETabRole::MajorTab)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot().MaxHeight(50)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SNew(SButton)
					.Text(FText::FromString("Start Checking"))
					.OnClicked_Raw(this, &FConventionAgentModule::CheckAssets, FString(""))
				]+SHorizontalBox::Slot()
				[
					SNew(SButton)
					.Text(FText::FromString("Save Config"))
					.OnClicked_Raw(this, &FConventionAgentModule::SaveConfig)
				]
			]
			+SVerticalBox::Slot().FillHeight(2)
			[
				SNew(SScrollBox)
				+SScrollBox::Slot()
				[
					GeneratePluginWindow()
				]
			]
		];
	return dockContainer;
}

void FConventionAgentModule::LostFocus(const FText & _InText, ETextCommit::Type _Type, bool _IsPre, FAssetInfo * _info)
{
	if (_IsPre) {
		_info->prefix = _InText.ToString();
	}
	else {
		_info->suffix = _InText.ToString();
	}
}

TSharedRef<SVerticalBox> FConventionAgentModule::GeneratePluginWindow()
{
	TSharedRef<SVerticalBox> infoContainer = SNew(SVerticalBox);
	for (FAssetInfo* info : assetInfoHandler->assetsInfo)
	{
		//TODO: Hide subtype if empty and resize type textbox
		EVisibility showSub = /*info->subType.IsEmpty() ? EVisibility::Collapsed :*/ EVisibility::Visible;

		infoContainer->AddSlot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Type:"))
			]
		+ SHorizontalBox::Slot()
			[
				SNew(SEditableTextBox)
				.IsEnabled(false)
			.Text(FText::FromString(info->type))
			]
		+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Visibility(showSub)
			.Text(FText::FromString("Subtype:"))
			]
		+ SHorizontalBox::Slot()
			[
				SNew(SEditableTextBox)
				.IsEnabled(false)
			.Visibility(showSub)
			.Text(FText::FromString(info->subType))
			]
		+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Prefix:"))
			]
		+ SHorizontalBox::Slot()
			[
				SNew(SEditableTextBox)
				.Text(FText::FromString(info->prefix))
			.OnTextCommitted_Raw(this, &FConventionAgentModule::LostFocus, true, info)
			]
		+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Suffix:"))
			]
		+ SHorizontalBox::Slot()
			[
				SNew(SEditableTextBox)
				.Text(FText::FromString(info->suffix))
			.OnTextCommitted_Raw(this, &FConventionAgentModule::LostFocus, false, info)
			]
			];
	}
	return infoContainer;
}

FReply FConventionAgentModule::CheckAssets(FString _FromDir)
{
	// Uses path if set, root dir otherwise
	FString checkPath = _FromDir.IsEmpty() ? FPaths::ProjectContentDir() : _FromDir;

	FString fileExtension = "uasset";
	FString allFileExtension = "*." + fileExtension;
	FString contentName = "/Content/";
	FString gameName = "/Game/";
	FString assetName, finalAssetPath;
	int32 contentIndex, dotIndex = 0;
	UAssetManager &assetManager = UAssetManager::Get();
	FAssetData fileData;

	// Store names of all found assets
	TArray<FString> files;
	fileManager->FindFilesRecursive(files, *checkPath, *allFileExtension, true, false);

	//caLog->~FMessageLog();
	caLog->NewPage(logname);
	for (FString file : files)
	{
		// Convert file path to something UAssetManager understands
		contentIndex = file.Find(contentName);
		file.RemoveAt(0, contentIndex + contentName.Len());
		file.FindLastChar('/', contentIndex);
		file.FindLastChar('.', dotIndex);
		assetName = file.Mid(contentIndex + 1, dotIndex - contentIndex - 1);
		finalAssetPath = gameName + file.Replace(*fileExtension, *assetName);

		// Attempts to check formating
		FSoftObjectPath objpath(finalAssetPath);
		if (assetManager.GetAssetDataForPath(objpath, fileData)) {
			FString assetType;
			if (!AssetNamingChecker::IsAssetNamedCorrect(&fileData, assetInfoHandler, assetType))
			{
				caLog->Open(EMessageSeverity::Warning, true);
				caLog->Warning(FText::FromString("Asset '" + gameName + file + "' is wrong format! Type is: " + assetType));
			}
		}
		else {
			UE_LOG(LogConventionAgent, Warning, TEXT("Failed to get asset data!"));
		}
	}
	int32 loggedMessages = caLog->NumMessages(EMessageSeverity::Warning);
	caLog->Info(FText::FromString(FString::FromInt(loggedMessages) + " incorrect named files found."));

	// Flush the buffer, forcing all buffered messages to the screen. 
	caLog->~FMessageLog();
	return FReply::Handled();
}

FReply FConventionAgentModule::SaveConfig() 
{
	if (!assetInfoHandler->SaveInfoToFile(configFile)) {
		UE_LOG(LogConventionAgent, Warning, TEXT("Failed to save config file! Path: %s"), *configFile);
	}
	return FReply::Handled();
}

void FConventionAgentModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(ConventionAgentTabName);
}

void FConventionAgentModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FConventionAgentCommands::Get().OpenPluginWindow);
}

void FConventionAgentModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FConventionAgentCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FConventionAgentModule, ConventionAgent)