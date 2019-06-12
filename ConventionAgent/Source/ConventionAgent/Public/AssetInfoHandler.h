#pragma once
#include <CoreMinimal.h>

struct FAssetInfo
{
	FString type;
	FString subType;
	FString prefix;
	FString suffix;

	FAssetInfo(FString _type, FString _subType, FString _Prefix, FString _Suffix) {
		type = _type;
		subType = _subType;
		prefix = _Prefix;
		suffix = _Suffix;
	}
};

class AssetInfoHandler
{
public:
	AssetInfoHandler();
	AssetInfoHandler(FString _Path);
	~AssetInfoHandler();

	bool LoadInfoFromFile(FString _Path);
	bool SaveInfoToFile(FString _Path);

	FAssetInfo* FindAssetInfo(FString _AssetType, FString _AssetSubType = "");

	TArray<FAssetInfo*> assetsInfo;
	bool checkOnStartup = false;

private:

	void GenerateInfo();
	ECompressionFlags compresType = ECompressionFlags::COMPRESS_ZLIB;

	const FString jsonArrayName = "AssetInfo", jsonTypeName = "Type", jsonSubName = "Subtype", jsonPreName = "Prefix", jsonSufName = "Suffix", jsonCheckName = "CheckOnStartup";
};

