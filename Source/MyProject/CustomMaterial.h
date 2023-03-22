
#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "CustomMaterial.generated.h"

UCLASS(Blueprintable)
class MYPROJECT_API UCustomMaterial : public UMaterial
{
	GENERATED_BODY()
	
public:
	UCustomMaterial();
};
