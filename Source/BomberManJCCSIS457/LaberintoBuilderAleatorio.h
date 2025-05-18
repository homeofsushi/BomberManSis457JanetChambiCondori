#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BloqueBase.h"
#include "Moneda.h"
#include "PuertaTeletransportadora.h"

// Si tu clase no hereda de UObject, deja solo class
class LaberintoBuilderAleatorio
{
public:
    LaberintoBuilderAleatorio(UWorld* InWorld, FVector InOrigen, int InFilas, int InColumnas, float InTamanoBloque);

    // Setters para las clases Blueprint
    void SetClaseBloqueMadera(TSubclassOf<ABloqueBase> InClase) { ClaseBloqueMadera = InClase; }
    void SetClaseBloqueHierro(TSubclassOf<ABloqueBase> InClase) { ClaseBloqueHierro = InClase; }
    void SetClaseMoneda(TSubclassOf<AMoneda> InClase) { ClaseMoneda = InClase; }
    void SetClasePuerta(TSubclassOf<APuertaTeletransportadora> InClase) { ClasePuerta = InClase; }

    void CrearLaberinto();
    void AgregarBloques();
    void AgregarPuertas();
    void AgregarMoneda();

    const TArray<int32>& ObtenerLaberinto() const;
    const TArray<FVector>& ObtenerPosicionesMonedas() const;
    const TArray<FVector>& ObtenerPosicionesPuertas() const;

public:
    UWorld* World;
    FVector Origen;
    int Filas;
    int Columnas;
    float TamanoBloque;

    float LimiteXMin = 0.0f;
    float LimiteXMax = 1000.0f;
    float LimiteYMin = 0.0f;
    float LimiteYMax = 1000.0f;
    int AnchoLaberinto = 0;
    int AltoLaberinto = 0;

    // Referencias a las clases Blueprint
    TSubclassOf<ABloqueBase> ClaseBloqueMadera;
    TSubclassOf<ABloqueBase> ClaseBloqueHierro;
    TSubclassOf<AMoneda> ClaseMoneda;
    TSubclassOf<APuertaTeletransportadora> ClasePuerta;

    TArray<int32> Laberinto;
    TArray<FVector> PosicionesMonedas;
    TArray<FVector> PosicionesPuertas;
};
