#pragma once
#include <CoreMinimal.h>

struct FAssetData;
class AssetInfoHandler;
struct FAssetInfo;

class AssetNamingChecker
{
public:
	AssetNamingChecker();
	~AssetNamingChecker();

	static bool IsAssetNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType);

private:
	static bool IsBlueprintNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType);
	static bool IsTextureNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType);
	static bool IsMaterialNamedCorrect(FAssetData* _AssetData, AssetInfoHandler* _assetsInfo, FString& _assetType);

	static bool IsGoodFix(FString _AssetName, FAssetInfo* _Info);
};

