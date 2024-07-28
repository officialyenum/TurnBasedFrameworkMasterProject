// Copyright Chukwuyenum Opone @officialyenum


#include "AbilitySystem/Data/AttributeInfo.h"


FTbfAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FTbfAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(Tag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s],"), *Tag.ToString(), *GetNameSafe(this))
	}
	return FTbfAttributeInfo();
}