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
    // No llamar a CargarClases ni usar ConstructorHelpers aqu�
}

void LaberintoBuilderAleatorio::CrearLaberinto()
{
    Laberinto.Empty();
    Laberinto.SetNum(AnchoLaberinto * AltoLaberinto);
}

void LaberintoBuilderAleatorio::AgregarBloques()
{
    // Iniciar las variables
    TamanoBloque = 100.0f;  // Tama�o de cada bloque
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
            continue; // Si no es v�lida, intenta con otra posici�n
        }

        int32 GrupoTamano = FMath::RandRange(3, 6);
        TArray<int32> TiposBloques = { 1, 2, 3, 4 };
        TArray<int32> BloquesUsados;

        for (int32 i = 0; i < GrupoTamano && BloquesOcupados > 0; i++)
        {
            // Verificar que la posici�n est� vac�a y dentro de los l�mites
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
                    // Elegir cualquier tipo de bloque despu�s de los primeros 3
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
                    ClaseBloque = ABloqueAcero::StaticClass(); // Si tienes un BP, p�salo igual que los otros
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

            // Mover a una posici�n adyacente
            int32 DeltaX = FMath::RandRange(-1, 1);
            int32 DeltaY = FMath::RandRange(-1, 1);

            // Validar que la nueva posici�n est� dentro de los l�mites del laberinto
            PosX = FMath::Clamp(PosX + DeltaX, 0, AnchoLaberinto - 1);
            PosY = FMath::Clamp(PosY + DeltaY, 0, AltoLaberinto - 1);
        }

        // Separar el grupo con al menos 1 bloque vac�o
        for (int32 Separacion = 0; Separacion < 1; Separacion++)
        {
            PosX = FMath::RandRange(0, AnchoLaberinto - 1);
            PosY = FMath::RandRange(0, AltoLaberinto - 1);
        }
    }
}

void LaberintoBuilderAleatorio::AgregarPuertas()
{
    // Aqu� puedes enlazar las puertas entre s� si quieres (ej. usando un array dentro de APuertaTeletransportadora)
}

void LaberintoBuilderAleatorio::AgregarMoneda()
{
    for (int32 i = 0; i < 5; i++)
    {
        FVector PosicionMoneda;
        bool PosicionValida = false;

        // Buscar una posici�n vac�a
        while (!PosicionValida)
        {
            int32 PosX = FMath::RandRange(0, AnchoLaberinto - 1);
            int32 PosY = FMath::RandRange(0, AltoLaberinto - 1);
            int32 Index = PosY * AnchoLaberinto + PosX;

            if (Laberinto[Index] == 0) // Verificar que est� vac�o
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

// ... otras implementaciones de m�todos de LaberintoBuilderAleatorio ...

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
