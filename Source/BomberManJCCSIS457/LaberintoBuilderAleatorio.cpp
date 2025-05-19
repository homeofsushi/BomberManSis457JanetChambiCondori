#include "LaberintoBuilderAleatorio.h"
#include "BloqueBase.h"
#include "BloquePared.h"
#include "BloqueMadera.h"
#include "BloqueAcero.h"
#include "BloqueLadrillo.h"
#include "Moneda.h"
#include "PuertaTeletransportadora.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

LaberintoBuilderAleatorio::LaberintoBuilderAleatorio(UWorld* InWorld, FVector InOrigen, int InFilas, int InColumnas, float InTamanoBloque)
    : World(InWorld), Origen(InOrigen), Filas(InFilas), TamanoBloque(InTamanoBloque), Columnas(InColumnas)
{
    // No llamar a CargarClases ni usar ConstructorHelpers aquí
}

void LaberintoBuilderAleatorio::CrearLaberinto()
{
    Laberinto.Empty();
    Laberinto.SetNum(AnchoLaberinto * AltoLaberinto);
}

void LaberintoBuilderAleatorio::AgregarBloques()
{
    TamanoBloque = 100.0f;
    AnchoLaberinto = (LimiteXMax - LimiteXMin) / TamanoBloque;
    AltoLaberinto = (LimiteYMax - LimiteYMin) / TamanoBloque;

    // Validar dimensiones
    if (AnchoLaberinto <= 0 || AltoLaberinto <= 0) {
        UE_LOG(LogTemp, Error, TEXT("Dimensiones del laberinto inválidas."));
        return;
    }

    // Inicializar el laberinto
    CrearLaberinto();

    int32 TotalBloques = AnchoLaberinto * AltoLaberinto;
    int32 BloquesOcupados = FMath::RoundToInt(TotalBloques * 0.4f);

    int intentosMaximos = 10000;
    int intentos = 0;

    while (BloquesOcupados > 0 && intentos < intentosMaximos)
    {
        intentos++;
        int intentosInternos = 0;
        const int intentosInternosMax = 1000; // Límite de intentos internos

        while (BloquesOcupados > 0 && intentosInternos < intentosInternosMax)
        {
            intentosInternos++;
            int32 PosX = FMath::RandRange(0, AnchoLaberinto - 1);
            int32 PosY = FMath::RandRange(0, AltoLaberinto - 1);

            int32 Index = PosY * AnchoLaberinto + PosX;
            if (Index < 0 || Index >= Laberinto.Num() || Laberinto[Index] != 0)
            {
                continue; // Si no es válida, intenta con otra posición
            }

            int32 GrupoTamano = FMath::RandRange(3, 6);
            TArray<int32> TiposBloques = { 1, 2, 3, 4 };
            TArray<int32> BloquesUsados;

            for (int32 i = 0; i < GrupoTamano && BloquesOcupados > 0; i++)
            {
                Index = PosY * AnchoLaberinto + PosX;
                if (Index >= 0 && Index < Laberinto.Num() && Laberinto[Index] == 0)
                {
                    int32 TipoBloque;
                    if (BloquesUsados.Num() < 3)
                    {
                        TipoBloque = TiposBloques.Pop();
                        BloquesUsados.Add(TipoBloque);
                    }
                    else
                    {
                        TipoBloque = 4;
                    }

                    Laberinto[Index] = TipoBloque;
                    BloquesOcupados--;

                    FVector PosicionBloque = FVector(LimiteXMin + PosX * TamanoBloque, LimiteYMin + PosY * TamanoBloque, 0.0f);
                    FActorSpawnParameters SpawnParams;

                    TSubclassOf<ABloqueBase> ClaseBloque = nullptr;
                    switch (TipoBloque)
                    {
                    case 1:
                        ClaseBloque = ClaseBloqueMadera;
                        break;
                    case 2:
                        ClaseBloque = ClaseBloqueHierro;
                        break;
                    case 3:
                        ClaseBloque = ABloqueAcero::StaticClass();
                        break;
                    default:
                        ClaseBloque = ABloquePared::StaticClass();
                        break;
                    }
                    if (ClaseBloque)
                    {
                        World->SpawnActor<ABloqueBase>(ClaseBloque, PosicionBloque, FRotator::ZeroRotator, SpawnParams);
                    }
                }

                int32 DeltaX = FMath::RandRange(-1, 1);
                int32 DeltaY = FMath::RandRange(-1, 1);

                PosX = FMath::Clamp(PosX + DeltaX, 0, AnchoLaberinto - 1);
                PosY = FMath::Clamp(PosY + DeltaY, 0, AltoLaberinto - 1);
            }

            for (int32 Separacion = 0; Separacion < 1; Separacion++)
            {
                PosX = FMath::RandRange(0, AnchoLaberinto - 1);
                PosY = FMath::RandRange(0, AltoLaberinto - 1);
            }
        }

        if (intentosInternos >= intentosInternosMax)
        {
            UE_LOG(LogTemp, Warning, TEXT("Se alcanzó el máximo de intentos internos al colocar bloques. Puede que el laberinto esté lleno o no haya más posiciones válidas."));
            break;
        }
    }
    if (intentos >= intentosMaximos)
    {
        UE_LOG(LogTemp, Warning, TEXT("Se alcanzó el máximo de intentos al colocar bloques. Puede que el laberinto esté lleno."));
    }
}

void LaberintoBuilderAleatorio::AgregarMoneda()
{
    if (Laberinto.Num() == 0) {
        UE_LOG(LogTemp, Error, TEXT("El laberinto no está inicializado. Llama a CrearLaberinto() antes de AgregarMoneda()."));
        return;
    }

    PosicionesMonedas.Empty();

    for (int32 i = 0; i < 5; i++)
    {
        FVector PosicionMoneda;
        bool PosicionValida = false;
        int intentos = 0;
        const int intentosMax = 1000;

        while (!PosicionValida && intentos < intentosMax)
        {
            intentos++;
            int32 PosX = FMath::RandRange(0, AnchoLaberinto - 1);
            int32 PosY = FMath::RandRange(0, AltoLaberinto - 1);
            int32 Index = PosY * AnchoLaberinto + PosX;

            if (Index >= 0 && Index < Laberinto.Num() && Laberinto[Index] == 0)
            {
                PosicionMoneda = FVector(LimiteXMin + PosX * TamanoBloque, LimiteYMin + PosY * TamanoBloque, 100.0f);
                PosicionValida = true;
                PosicionesMonedas.Add(PosicionMoneda);
            }
        }

        if (!PosicionValida) {
            UE_LOG(LogTemp, Warning, TEXT("No se pudo encontrar una posición válida para la moneda %d."), i);
            continue;
        }

        FActorSpawnParameters SpawnParams;
        if (ClaseMoneda)
        {
            World->SpawnActor<AMoneda>(ClaseMoneda, PosicionMoneda, FRotator::ZeroRotator, SpawnParams);
        }
    }
}


void LaberintoBuilderAleatorio::AgregarPuertas()
{
    // Aquí puedes enlazar las puertas entre sí si quieres (ej. usando un array dentro de APuertaTeletransportadora)
}


// ... otras implementaciones de métodos de LaberintoBuilderAleatorio ...

const TArray<int32>& LaberintoBuilderAleatorio::ObtenerLaberinto() const
{
    return Laberinto;
}

const TArray<FVector>& LaberintoBuilderAleatorio::ObtenerPosicionesMonedas() const
{
    return PosicionesMonedas;
}

const TArray<FVector>& LaberintoBuilderAleatorio::ObtenerPosicionesPuertas() const
{
    return PosicionesPuertas;
}
