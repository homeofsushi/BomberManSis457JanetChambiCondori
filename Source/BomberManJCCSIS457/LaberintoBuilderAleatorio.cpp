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
    // Iniciar las variables
    TamanoBloque = 100.0f;  // Tamaño de cada bloque
    AnchoLaberinto = (LimiteXMax - LimiteXMin) / TamanoBloque;
    AltoLaberinto = (LimiteYMax - LimiteYMin) / TamanoBloque;

    int32 TotalBloques = AnchoLaberinto * AltoLaberinto;
    int32 BloquesOcupados = FMath::RoundToInt(TotalBloques * 0.4f); // 40% del espacio

    while (BloquesOcupados > 0)
    {
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
            // Verificar que la posición esté vacía y dentro de los límites
            Index = PosY * AnchoLaberinto + PosX;
            if (Index >= 0 && Index < Laberinto.Num() && Laberinto[Index] == 0)
            {
                // Elegir un tipo de bloque aleatorio
                int32 TipoBloque;
                if (BloquesUsados.Num() < 3)
                {
                    TipoBloque = TiposBloques.Pop(); // Sacar un tipo de bloque distinto
                    BloquesUsados.Add(TipoBloque);
                }
                else
                {
                    // Elegir cualquier tipo de bloque después de los primeros 3
                    TipoBloque = 4;
                }

                Laberinto[Index] = TipoBloque;
                BloquesOcupados--;

                // Instanciar el bloque en el mundo
                FVector PosicionBloque = FVector(LimiteXMin + PosX * TamanoBloque, LimiteYMin + PosY * TamanoBloque, 0.0f);
                FActorSpawnParameters SpawnParams;

                // Determinar la clase del bloque usando las variables miembro
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
                    ClaseBloque = ABloqueAcero::StaticClass(); // Si tienes un BP, pásalo igual que los otros
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

            // Mover a una posición adyacente
            int32 DeltaX = FMath::RandRange(-1, 1);
            int32 DeltaY = FMath::RandRange(-1, 1);

            // Validar que la nueva posición esté dentro de los límites del laberinto
            PosX = FMath::Clamp(PosX + DeltaX, 0, AnchoLaberinto - 1);
            PosY = FMath::Clamp(PosY + DeltaY, 0, AltoLaberinto - 1);
        }

        // Separar el grupo con al menos 1 bloque vacío
        for (int32 Separacion = 0; Separacion < 1; Separacion++)
        {
            PosX = FMath::RandRange(0, AnchoLaberinto - 1);
            PosY = FMath::RandRange(0, AltoLaberinto - 1);
        }
    }
}

void LaberintoBuilderAleatorio::AgregarPuertas()
{
    // Aquí puedes enlazar las puertas entre sí si quieres (ej. usando un array dentro de APuertaTeletransportadora)
}

void LaberintoBuilderAleatorio::AgregarMoneda()
{
    for (int32 i = 0; i < 5; i++)
    {
        FVector PosicionMoneda;
        bool PosicionValida = false;

        // Buscar una posición vacía
        while (!PosicionValida)
        {
            int32 PosX = FMath::RandRange(0, AnchoLaberinto - 1);
            int32 PosY = FMath::RandRange(0, AltoLaberinto - 1);
            int32 Index = PosY * AnchoLaberinto + PosX;

            if (Laberinto[Index] == 0) // Verificar que esté vacío
            {
                PosicionMoneda = FVector(LimiteXMin + PosX * TamanoBloque, LimiteYMin + PosY * TamanoBloque, 100.0f);
                PosicionValida = true;
            }
        }

        // Instanciar la moneda usando la clase recibida
        FActorSpawnParameters SpawnParams;
        if (ClaseMoneda)
        {
            World->SpawnActor<AMoneda>(ClaseMoneda, PosicionMoneda, FRotator::ZeroRotator, SpawnParams);
        }
    }
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
