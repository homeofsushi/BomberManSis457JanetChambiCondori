// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloqueBase.h"
#include "BloquePared.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANJCCSIS457_API ABloquePared : public ABloqueBase
{
	GENERATED_BODY()

public:
	ABloquePared();

	virtual void ConfigurarBloque() override;
	
};
