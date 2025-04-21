// Copyright Epic Games, Inc. All Rights Reserved.

#include "BomberManJCCSIS457GameMode.h"
#include "BomberManJCCSIS457Character.h"
#include "UObject/ConstructorHelpers.h"
#include "BloqueBase.h"
#include "BloquePared.h"
#include "BloqueMadera.h"
#include "BloqueAcero.h"
#include "Moneda.h"
#include "BloqueLadrillo.h"
#include "Engine/World.h"
#include "BomberManJCCSIS457Character.h"
#include <Kismet/GameplayStatics.h>

ABomberManJCCSIS457GameMode::ABomberManJCCSIS457GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// Iniciar las variables
	TamanoBloque = 100.0f;  // Tama�o de cada bloque
	AnchoLaberinto = (LimiteXMax - LimiteXMin) / TamanoBloque;
	AltoLaberinto = (LimiteYMax - LimiteYMin) / TamanoBloque;
}
void ABomberManJCCSIS457GameMode::BeginPlay()
{
    Super::BeginPlay(); 
    CrearLaberinto();  
    
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

        // Instanciar la moneda
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<AMoneda>(AMoneda::StaticClass(), PosicionMoneda, FRotator::ZeroRotator, SpawnParams);
    }// Llamar a la funci�n que genera el laberinto
}

void ABomberManJCCSIS457GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);  // Llamar al Tick de la clase base
    // Aqu� puedes agregar la l�gica adicional para el Tick si es necesario
}


void ABomberManJCCSIS457GameMode::CrearLaberinto()
{
    Laberinto.Empty();
    Laberinto.SetNum(AnchoLaberinto * AltoLaberinto);

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

                // Determinar la clase del bloque
                TSubclassOf<ABloqueBase> ClaseBloque;
                switch (TipoBloque)
                {
                case 1:
                    ClaseBloque = ABloqueMadera::StaticClass();
                    break;
                case 2:
                    ClaseBloque = ABloqueLadrillo::StaticClass();
                    break;
                case 3:
                    ClaseBloque = ABloqueAcero::StaticClass();
                    break;
                default:
                    ClaseBloque = ABloquePared::StaticClass();
                    break;
                }

                // Crear el bloque
                GetWorld()->SpawnActor<ABloqueBase>(ClaseBloque, PosicionBloque, FRotator::ZeroRotator, SpawnParams);
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

