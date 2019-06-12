#include "AssetNamingChecker.h"
#include "AssetInfoHandler.h"
#include "ConventionAgentUtil.h"
#include <Engine/AssetManager.h>
#include <Engine/Blueprint.h>
#include <Materials/Material.h>

AssetNamingChecker::AssetNamingChecker()
{
}


AssetNamingChecker::~AssetNamingChecker()
{
}

bool AssetNamingChecker::IsAssetNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType)
{
	FString className = _AssetData->AssetClass.ToString(); 
	FAssetInfo* info = _assetsInfo->FindAssetInfo(className);
	if (info) {
		if (info->type == "Blueprint") {
			return IsBlueprintNamedCorrect(_AssetData, _assetsInfo, _assetType);
		}
		if (info->type == "Texture2D") {
			return IsTextureNamedCorrect(_AssetData, _assetsInfo, _assetType);
		}
		if (info->type == "Material") {
			return IsMaterialNamedCorrect(_AssetData,_assetsInfo, _assetType);
		}
		_assetType = info->type + "." + info->subType + ".";
		return IsGoodFix(_AssetData->AssetName.ToString(), info);
	}
	return false;
}

bool AssetNamingChecker::IsBlueprintNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType)
{
	UBlueprint* bp = Cast<UBlueprint>(_AssetData->GetAsset());
	if (!bp) {
		return false;
	}
	FAssetInfo* info = nullptr;

	EBlueprintType type = bp->BlueprintType.GetValue();
	if (type != EBlueprintType::BPTYPE_Normal) {
		info = _assetsInfo->FindAssetInfo(_AssetData->AssetClass.ToString(), EnumToString(EBlueprintType, type));
	}
	else {
		// TODO: make this prettier using typeid or something.
		FString parentClassName = ConventionAgentUtil::GetClassAsString(bp->ParentClass.Get());
		info = _assetsInfo->FindAssetInfo(_AssetData->AssetClass.ToString(), parentClassName);
	}

	_assetType = info->type + "." + info->subType + ".";
	return IsGoodFix(_AssetData->AssetName.ToString(), info);
}

bool AssetNamingChecker::IsTextureNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType)
{
	// TODO: cast to UTexture2D and either use enums: TextureGroup or TextureCompressionSettings.
	FAssetInfo* info = _assetsInfo->FindAssetInfo(_AssetData->AssetClass.ToString());

	_assetType = info->type + "." + info->subType + ".";
	return IsGoodFix(_AssetData->AssetName.ToString(), info);
}

bool AssetNamingChecker::IsMaterialNamedCorrect(FAssetData * _AssetData, AssetInfoHandler * _assetsInfo, FString& _assetType)
{
	UMaterial* mat = Cast<UMaterial>(_AssetData->GetAsset());
	if (!mat) {
		return false;
	}

	FAssetInfo* info = nullptr;
	info = _assetsInfo->FindAssetInfo(_AssetData->AssetClass.ToString(), EnumToString(EMaterialDomain, mat->MaterialDomain));

	_assetType = info->type + "." + info->subType + ".";
	return IsGoodFix(_AssetData->AssetName.ToString(), info);
}



bool AssetNamingChecker::IsGoodFix(FString _AssetName, FAssetInfo* _Info)
{
	if (!_Info) 
	{ 
		return false; 
	}
	bool goodPre = _Info->prefix.IsEmpty() || _AssetName.StartsWith(_Info->prefix, ESearchCase::CaseSensitive);
	bool goodSuf = _Info->suffix.IsEmpty() || _AssetName.EndsWith(_Info->suffix, ESearchCase::CaseSensitive);
	return goodPre && goodSuf;
}
