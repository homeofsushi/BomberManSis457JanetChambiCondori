
#include "BomberManJCCSIS457GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "LaberintoBuilderAleatorio.h" // Asegúrate de incluir este encabezado
#include "Templates/SharedPointer.h"
#include "Engine/World.h"

ABomberManJCCSIS457GameMode::ABomberManJCCSIS457GameMode()
{
    // Cargar las clases Blueprint
    static ConstructorHelpers::FClassFinder<ABloqueBase> BloqueMaderaBP(TEXT("/Game/Bloques/BP_BloqueMadera"));
    static ConstructorHelpers::FClassFinder<ABloqueBase> BloqueHierroBP(TEXT("/Game/Bloques/BP_BloqueHierro"));
    static ConstructorHelpers::FClassFinder<AMoneda> MonedaBP(TEXT("/Game/Items/BP_Moneda"));
    static ConstructorHelpers::FClassFinder<APuertaTeletransportadora> PuertaBP(TEXT("/Game/Items/BP_Puerta"));

    // Crear el builder (ajusta los parámetros según tu lógica)
    Builder = new LaberintoBuilderAleatorio(GetWorld(), FVector::ZeroVector, 10, 10, 100.0f);

    // Pasar las clases al builder
    if (BloqueMaderaBP.Succeeded()) Builder->SetClaseBloqueMadera(BloqueMaderaBP.Class);
    if (BloqueHierroBP.Succeeded()) Builder->SetClaseBloqueHierro(BloqueHierroBP.Class);
    if (MonedaBP.Succeeded()) Builder->SetClaseMoneda(MonedaBP.Class);
    if (PuertaBP.Succeeded()) Builder->SetClasePuerta(PuertaBP.Class);

    // Ahora puedes usar el builder normalmente
}

void ABomberManJCCSIS457GameMode::BeginPlay()
{
    Super::BeginPlay();

    LaberintoBuilder = MakeShared<LaberintoBuilderAleatorio>(
        GetWorld(),
        FVector(LimiteXMin, LimiteYMin, 0),
        AltoLaberinto,
        AnchoLaberinto,
        TamanoBloque
    );
    // Si tu builder necesita las clases, pásalas aquí (ajusta tu builder si es necesario)
    LaberintoBuilder->ClaseBloqueMadera = ClaseBloqueMadera;
    LaberintoBuilder->ClaseBloqueHierro = ClaseBloqueHierro;
    LaberintoBuilder->ClaseMoneda = ClaseMoneda;
    LaberintoBuilder->ClasePuerta = ClasePuerta;

    LaberintoBuilder->LimiteXMin = LimiteXMin;
    LaberintoBuilder->LimiteYMin = LimiteYMin;
    LaberintoBuilder->LimiteXMax = LimiteXMax;
    LaberintoBuilder->LimiteYMax = LimiteYMax;

    LaberintoBuilder->CrearLaberinto();
    LaberintoBuilder->AgregarBloques();
    LaberintoBuilder->AgregarPuertas();
    LaberintoBuilder->AgregarMoneda();

    // Copia el array del laberinto para que otros actores puedan consultarlo
    Laberinto = LaberintoBuilder->ObtenerLaberinto();
}

void ABomberManJCCSIS457GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/*// Copyright Epic Games, Inc. All Rights Reserved.

#include "BomberManJCCSIS457GameMode.h"
#include "BomberManJCCSIS457Character.h"
#include "UObject/ConstructorHelpers.h"
#include "BloqueBase.h"
#include "BloquePared.h"
#include "BloqueMadera.h"
#include "BloqueAcero.h"
#include "Moneda.h"
#include "LaberintoBuilderAleatorio.h"
#include "ILaberintoBuilder.h"


#include "BloqueLadrillo.h"
#include "Engine/World.h"

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
	TamanoBloque = 100.0f;  // Tamaño de cada bloque
	AnchoLaberinto = (LimiteXMax - LimiteXMin) / TamanoBloque;
	AltoLaberinto = (LimiteYMax - LimiteYMin) / TamanoBloque;
}
void ABomberManJCCSIS457GameMode::BeginPlay()
{
    Super::BeginPlay(); 

    // Instanciar el builder (puedes usar un director si lo tienes)
    LaberintoBuilderAleatorio* Builder = new LaberintoBuilderAleatorio(
        GetWorld(),
        FVector(LimiteXMin, LimiteYMin, 0.0f),
        AltoLaberinto,
        AnchoLaberinto,
        TamanoBloque
    );

    Builder->CrearLaberinto();
    Builder->AgregarBloques();
    Builder->AgregarPuertas();
    Builder->AgregarMoneda();
    //CrearLaberinto();  

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

        // Instanciar la moneda
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<AMoneda>(AMoneda::StaticClass(), PosicionMoneda, FRotator::ZeroRotator, SpawnParams);
    }// Llamar a la función que genera el laberinto
    
}

void ABomberManJCCSIS457GameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);  // Llamar al Tick de la clase base
    // Aquí puedes agregar la lógica adicional para el Tick si es necesario
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

*/


