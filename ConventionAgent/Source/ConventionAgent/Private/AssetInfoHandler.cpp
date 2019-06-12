#include "AssetInfoHandler.h"
#include "ConventionAgentUtil.h"
#include <Engine/Blueprint.h>
#include <Materials/Material.h>
#include <FileHelper.h>
#include <Json.h>

AssetInfoHandler::AssetInfoHandler()
{
	GenerateInfo();
}

AssetInfoHandler::AssetInfoHandler(FString _Path)
{
	if (!LoadInfoFromFile(_Path)) {
		GenerateInfo();
		SaveInfoToFile(_Path);
	}
}


AssetInfoHandler::~AssetInfoHandler()
{
}

bool AssetInfoHandler::LoadInfoFromFile(FString _Path)
{
	FString inputString;
	if (!FFileHelper::LoadFileToString(inputString, *_Path)) {
		return false;
	}
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(inputString);

	// Attempt to deserialize the root object
	TSharedPtr<FJsonObject> rootObj;
	if (FJsonSerializer::Deserialize(reader, rootObj)) {

		checkOnStartup = rootObj->GetBoolField(jsonCheckName);
		// Attempt to get the array data
		const TArray<TSharedPtr<FJsonValue>>* infoArray;
		if (rootObj->TryGetArrayField(jsonArrayName, infoArray)) {
			assetsInfo.Empty();

			// Loop over array items and attempt to create FAssetInfo from them
			for (TSharedPtr<FJsonValue> infoVal : *infoArray) {
				const TSharedPtr<FJsonObject>* infoObj;
				if (infoVal->TryGetObject(infoObj)) {
					FString objType = infoObj->ToSharedRef()->GetStringField(jsonTypeName);
					FString objSubType = infoObj->ToSharedRef()->GetStringField(jsonSubName);
					FString objPrefix = infoObj->ToSharedRef()->GetStringField(jsonPreName);
					FString objSuffix = infoObj->ToSharedRef()->GetStringField(jsonSufName);
					if (!objType.IsEmpty()) {
						assetsInfo.Add(new FAssetInfo(objType, objSubType, objPrefix, objSuffix));
					}
				}
			}
		}
	}
	return false;
}

bool AssetInfoHandler::SaveInfoToFile(FString _Path)
{
	TArray<TSharedPtr<FJsonValue>> infoArray;
	for (FAssetInfo* info : assetsInfo)
	{
		TSharedPtr<FJsonObject> infoObj = MakeShareable(new FJsonObject);
		infoObj->SetStringField(jsonTypeName, info->type);
		infoObj->SetStringField(jsonSubName, info->subType);
		infoObj->SetStringField(jsonPreName, info->prefix);
		infoObj->SetStringField(jsonSufName, info->suffix);

		TSharedRef<FJsonValueObject> infoVal = MakeShareable(new FJsonValueObject(infoObj));
		infoArray.Add(infoVal);
	}

	TSharedPtr<FJsonObject> rootObj = MakeShareable(new FJsonObject);
	rootObj->SetBoolField(jsonCheckName, checkOnStartup);
	rootObj->SetArrayField(jsonArrayName, infoArray);

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(rootObj.ToSharedRef(), writer);

	if (!OutputString.IsEmpty()) {
		return FFileHelper::SaveStringToFile(OutputString, *_Path);
	}
	return false;
}

FAssetInfo * AssetInfoHandler::FindAssetInfo(FString _AssetType, FString _AssetSubType)
{
	for (FAssetInfo * info : assetsInfo)
	{
		if (info->type == _AssetType && 
			(_AssetSubType.IsEmpty() || info->subType == _AssetSubType)) {
			return info;
		}
	}
	return nullptr;
}

// Generates the array in case the array failed to generate from the config file
void AssetInfoHandler::GenerateInfo()
{
	// Generate data based of off http://ue4.style

#pragma region Misc
	// Miscellaneous
	assetsInfo.Add(new FAssetInfo("CameraAnim", "", "CA_", ""));
	assetsInfo.Add(new FAssetInfo("CurveLinearColor", "", "CC_", ""));
	assetsInfo.Add(new FAssetInfo("CompositeCurveTable", "", "CT_", ""));
	assetsInfo.Add(new FAssetInfo("CompositeDataTable", "", "CDT_", ""));
	assetsInfo.Add(new FAssetInfo("CurveLinearColorAtlas", "", "CC_", "Atlas"));
	assetsInfo.Add(new FAssetInfo("DataTable", "", "DT_", ""));
	assetsInfo.Add(new FAssetInfo("CurveFloat", "", "CF_", ""));
	assetsInfo.Add(new FAssetInfo("FoliageType_InstancedStaticMesh", "", "FT_", ""));
	assetsInfo.Add(new FAssetInfo("ForceFeedbackEffect", "", "FFE_", ""));
	assetsInfo.Add(new FAssetInfo("HapticFeedbackEffect_Buffer", "", "HFFE_", "Buffer"));
	assetsInfo.Add(new FAssetInfo("HapticFeedbackEffect_Curve", "", "HFFE_", "Curve"));
	assetsInfo.Add(new FAssetInfo("HapticFeedbackEffect_SoundWave", "", "HFFE_", "Soundwave"));
	assetsInfo.Add(new FAssetInfo("LandscapeGrassType", "", "LG_", ""));
	assetsInfo.Add(new FAssetInfo("LightPropagationVolumeBlendable", "", "LPV_", ""));
	assetsInfo.Add(new FAssetInfo("InterpData", "", "Matinee_", ""));
	assetsInfo.Add(new FAssetInfo("ObjectLibrary", "", "OL_", ""));
	assetsInfo.Add(new FAssetInfo("PreviewMeshCollection", "", "PMC_", ""));
	assetsInfo.Add(new FAssetInfo("SlateVectorArtData", "", "SV_", ""));
	assetsInfo.Add(new FAssetInfo("StringTable", "", "ST_", ""));
	assetsInfo.Add(new FAssetInfo("SubUVAnimation", "", "SUA_", ""));
	assetsInfo.Add(new FAssetInfo("TouchInterface", "", "TI_", ""));
	assetsInfo.Add(new FAssetInfo("CurveVector", "", "CV_", "")); 
	assetsInfo.Add(new FAssetInfo("StaticMesh", "", "SM_", "")); 
	assetsInfo.Add(new FAssetInfo("ParticleSystem", "", "PS_", ""));
	assetsInfo.Add(new FAssetInfo("StaticVectorField", "", "VF_", ""));
	assetsInfo.Add(new FAssetInfo("AnimatedVectorField", "", "VFA_", ""));
	assetsInfo.Add(new FAssetInfo("LandscapeLayer", "", "LL_", ""));
#pragma endregion

#pragma region Animations
	// Animations
	assetsInfo.Add(new FAssetInfo("AimOffsetBlendSpace", "", "AO_", ""));
	assetsInfo.Add(new FAssetInfo("AimOffsetBlendSpace1D", "", "AO_", ""));
	assetsInfo.Add(new FAssetInfo("AnimBlueprint", "", "ABP_", ""));
	assetsInfo.Add(new FAssetInfo("AnimComposite", "", "AC_", ""));
	assetsInfo.Add(new FAssetInfo("AnimMontage", "", "AM_", ""));
	assetsInfo.Add(new FAssetInfo("AnimSequence", "", "A_", ""));
	assetsInfo.Add(new FAssetInfo("BlendSpace", "", "BS_", ""));
	assetsInfo.Add(new FAssetInfo("LevelSequence", "", "LS_", ""));
	assetsInfo.Add(new FAssetInfo("MorphTarget", "", "MT_", ""));
	assetsInfo.Add(new FAssetInfo("PoseAsset", "", "PA_", ""));
	assetsInfo.Add(new FAssetInfo("Rig", "", "Rig_", ""));
	assetsInfo.Add(new FAssetInfo("SkeletalMesh", "", "SK_", ""));
	assetsInfo.Add(new FAssetInfo("Skeleton", "", "SKEL_", ""));
#pragma endregion

#pragma region Blueprints
	// Blueprint type and subtypes
	assetsInfo.Add(new FAssetInfo("Blueprint", "", "BP_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", EnumToString(EBlueprintType, EBlueprintType::BPTYPE_MacroLibrary), "BPML_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", EnumToString(EBlueprintType, EBlueprintType::BPTYPE_Interface), "BPI_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", EnumToString(EBlueprintType, EBlueprintType::BPTYPE_FunctionLibrary), "BPFL_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[0], "BPC_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[1], "GM_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[2], "PC_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[3], "AIC_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[4], "TBP_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[5], "BP_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[6], "BP_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[7], "BP_", ""));
	assetsInfo.Add(new FAssetInfo("Animation Blueprint", "", "ABP_", ""));
	assetsInfo.Add(new FAssetInfo("UserDefinedEnum", "", "E", ""));
	assetsInfo.Add(new FAssetInfo("UserDefinedStruct", "", "S", ""));
#pragma endregion

#pragma region AI
	// Artificial Intelligence
	assetsInfo.Add(new FAssetInfo("BehaviourTree", "", "BT_", ""));
	assetsInfo.Add(new FAssetInfo("BlackboardData", "", "BB_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[8], "BTD_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[9], "BTS_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[10], "BTT_", ""));
	assetsInfo.Add(new FAssetInfo("EnvQuery", "", "EQS_", ""));
	assetsInfo.Add(new FAssetInfo("Blueprint", ConventionAgentUtil::classStrings[11], "EQS_", "Context"));
#pragma endregion

#pragma region Materials
	// Material type and subtypes
	assetsInfo.Add(new FAssetInfo("Material", "", "M_", ""));
	assetsInfo.Add(new FAssetInfo("Material", EnumToString(EMaterialDomain, EMaterialDomain::MD_Surface), "M_", "_S"));
	assetsInfo.Add(new FAssetInfo("Material", EnumToString(EMaterialDomain, EMaterialDomain::MD_DeferredDecal), "M_", "_D"));
	assetsInfo.Add(new FAssetInfo("Material", EnumToString(EMaterialDomain, EMaterialDomain::MD_LightFunction), "M_", "_LF"));
	assetsInfo.Add(new FAssetInfo("Material", EnumToString(EMaterialDomain, EMaterialDomain::MD_Volume), "M_", "_V"));
	assetsInfo.Add(new FAssetInfo("Material", EnumToString(EMaterialDomain, EMaterialDomain::MD_PostProcess), "M_", "_P"));
	assetsInfo.Add(new FAssetInfo("Material", EnumToString(EMaterialDomain, EMaterialDomain::MD_UI), "M_", "_UI"));
	assetsInfo.Add(new FAssetInfo("MaterialFunction", "", "MF_", ""));
	assetsInfo.Add(new FAssetInfo("MaterialInstanceConstant", "", "MI_", ""));
	assetsInfo.Add(new FAssetInfo("MaterialParameterCollection", "", "MPC_", ""));
	assetsInfo.Add(new FAssetInfo("SubsurfaceProfile", "", "SP_", ""));
#pragma endregion

#pragma region Textures
	// Textures
	assetsInfo.Add(new FAssetInfo("Texture2D", "", "T_", ""));
	assetsInfo.Add(new FAssetInfo("MediaTexture", "", "MT_", ""));
	assetsInfo.Add(new FAssetInfo("TextureRenderTargetCube", "", "RTC_", ""));
	assetsInfo.Add(new FAssetInfo("CanvasRenderTarget2D", "", "CRT_", ""));
	assetsInfo.Add(new FAssetInfo("TextureRenderTarget2D", "", "TRT_", ""));
	assetsInfo.Add(new FAssetInfo("VolumeTexture", "", "VT_", "")); 
	assetsInfo.Add(new FAssetInfo("TextureLightProfile", "", "TLP_", ""));
	assetsInfo.Add(new FAssetInfo("TextureCube", "", "TC_", ""));
#pragma endregion

#pragma region Media
	// Media
	assetsInfo.Add(new FAssetInfo("FileMediaSource", "", "FMS_", ""));
	assetsInfo.Add(new FAssetInfo("ImgMediaSource", "", "IMS_", ""));
	assetsInfo.Add(new FAssetInfo("MediaPlayer", "", "MP_", ""));
	assetsInfo.Add(new FAssetInfo("MediaPlaylist", "", "MPL_", ""));
	assetsInfo.Add(new FAssetInfo("MediaTexture", "", "MT_", ""));
	assetsInfo.Add(new FAssetInfo("PlatformMediaSource", "", "PMS_", ""));
	assetsInfo.Add(new FAssetInfo("StreamMediaSource", "", "SMC_", ""));
#pragma endregion

#pragma region Paper2D
	// Paper2D
	assetsInfo.Add(new FAssetInfo("PaperFlipbook", "", "PFB_", ""));
	assetsInfo.Add(new FAssetInfo("PaperSprite", "", "SPR_", ""));
	assetsInfo.Add(new FAssetInfo("PaperSpriteAtlasGroup", "", "SPRG_", ""));
	assetsInfo.Add(new FAssetInfo("PaperTileMap", "", "TM_", ""));
	assetsInfo.Add(new FAssetInfo("PaperTileSet", "", "TS_", ""));
#pragma endregion

#pragma region Physics
	// Physics
	assetsInfo.Add(new FAssetInfo("PhysicalMaterial", "", "PM_", ""));
	assetsInfo.Add(new FAssetInfo("PhysicsAsset", "", "PHYS_", ""));
	assetsInfo.Add(new FAssetInfo("DestructibleMesh", "", "DM_", ""));
#pragma endregion

#pragma region Audio
	// Audio
	assetsInfo.Add(new FAssetInfo("DialogueVoice", "", "DV_", ""));
	assetsInfo.Add(new FAssetInfo("DialogueWave", "", "DW_", ""));
	assetsInfo.Add(new FAssetInfo("ReverbEffect", "", "Reverb_", ""));
	assetsInfo.Add(new FAssetInfo("SoundAttenuation", "", "ATT_", ""));
	assetsInfo.Add(new FAssetInfo("SoundConcurrency", "", "", "_SC"));
	assetsInfo.Add(new FAssetInfo("SoundCue", "S_", "", "_Cue"));
	assetsInfo.Add(new FAssetInfo("SoundMix", "", "Mix_", ""));
	assetsInfo.Add(new FAssetInfo("SoundClass", "", "SC_", "")); 
	assetsInfo.Add(new FAssetInfo("SoundWave", "", "S_", "_Wave"));
#pragma endregion

#pragma region UI
	// User Interface
	assetsInfo.Add(new FAssetInfo("Font", "", "Font_", ""));
	assetsInfo.Add(new FAssetInfo("SlateBrushAsset", "", "Brush_", ""));
	assetsInfo.Add(new FAssetInfo("SlateWidgetStyleAsset", "", "Style_", ""));	
	assetsInfo.Add(new FAssetInfo("WidgetBlueprint", "", "WBP_", ""));
#pragma endregion
}