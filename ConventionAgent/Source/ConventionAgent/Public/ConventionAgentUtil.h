#pragma once
#include <CoreMinimal.h>

class ConventionAgentUtil
{
public:
	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& _name, TEnum _value);
	static FORCEINLINE FString GetClassAsString(UClass* _class);

	static TArray<FString> classStrings;
};

#define EnumToString(EnumClassName, ValueOfEnum) ConventionAgentUtil::GetEnumValueAsString<EnumClassName>(FString(TEXT(#EnumClassName)), (ValueOfEnum))