#include "ConventionAgentUtil.h"
#include <GameFramework/GameModeBase.h>
#include <AIController.h>
#include <EditorTutorial.h>
#include <GameFramework/Character.h>
#include <BehaviorTree/Decorators/BTDecorator_BlueprintBase.h>
#include <BehaviorTree/Services/BTService_BlueprintBase.h>
#include <BehaviorTree/Tasks/BTTask_BlueprintBase.h>
#include <EnvironmentQuery/EnvQueryContext.h>

template<typename TEnum>
inline FString ConventionAgentUtil::GetEnumValueAsString(const FString & _name, TEnum _value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *_name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)_value).ToString();
}

FString ConventionAgentUtil::GetClassAsString(UClass * _class)
{
	{
		if (_class->IsChildOf<UActorComponent>()) {
			return classStrings[0];
		}
		if (_class->IsChildOf<AGameModeBase>()) {
			return classStrings[1];
		}
		if (_class->IsChildOf<APlayerController>()) {
			return classStrings[2];
		}
		if (_class->IsChildOf<AAIController>()) {
			return classStrings[3];
		}
		if (_class->IsChildOf<UEditorTutorial>()) {
			return classStrings[4];
		}
		if (_class->IsChildOf<ACharacter>()) {
			return classStrings[5];
		}
		if (_class->IsChildOf<APawn>()) {
			return classStrings[6];
		}
		if (_class->IsChildOf<AActor>()) {
			return classStrings[7];
		}
		if (_class->IsChildOf<UBTDecorator>()) {
			return classStrings[8];
		}
		if (_class->IsChildOf<UBTService>()) {
			return classStrings[9];
		}
		if (_class->IsChildOf<UBTTaskNode>()) {
			return classStrings[10];
		}
		if (_class->IsChildOf<UEnvQueryContext>()) {
			return classStrings[11];
		}
		return FString("");
	}
}

TArray<FString> ConventionAgentUtil::classStrings {FString("Component"), FString("Gamemode"), FString("PlayerController"),
													FString("AIController"), FString("Tutorial"), FString("Character"),
													FString("Pawn"), FString("Actor"), FString("Decorator"), FString("Service"), FString("Task"), FString("EnvQueryContext") };