// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloqueBase.h"
#include "BloqueAcero.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANJCCSIS457_API ABloqueAcero : public ABloqueBase
{
	GENERATED_BODY()
public:
	ABloqueAcero();

	virtual void ConfigurarBloque() override;
	
};
