// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BloqueBase.h" // Mueve este include antes del .generated.h
#include "BomberManJCCSIS457GameMode.generated.h" // El archivo .generated.h debe ser el último include


UCLASS(minimalapi)
class ABomberManJCCSIS457GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABomberManJCCSIS457GameMode();
	// Variables para los límites del laberinto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteXMin = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteYMin = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteXMax = 2850.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteYMax = 3350.0f;

	// Declaración de la función CrearLaberinto
	UFUNCTION(BlueprintCallable, Category = "Laberinto")
	void CrearLaberinto();

	// Propiedades del laberinto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	int32 AnchoLaberinto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	int32 AltoLaberinto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float TamanoBloque;
	UPROPERTY()
	TArray<int32> Laberinto;


protected:
// Called when the game starts or when spawned
virtual void BeginPlay() override;

public:
// Called every frame
virtual void Tick(float DeltaTime) override;

};
