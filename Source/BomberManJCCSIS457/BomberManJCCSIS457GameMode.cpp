
#include "BomberManJCCSIS457GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "LaberintoBuilderAleatorio.h" // Asegúrate de incluir este encabezado
#include "Templates/SharedPointer.h"
#include "Moneda.h" 
#include "BomberManJCCSIS457Character.h"
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
    if (PuertaBP.Succeeded()) Builder->SetClasePuerta(PuertaBP.Class);

    ClaseMoneda = AMoneda::StaticClass();

    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Succeeded())
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
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

// En tu GameMode.cpp
FVector ABomberManJCCSIS457GameMode::ObtenerPosicionLibreAleatoria()
{
    if (!LaberintoBuilder.IsValid()) return FVector::ZeroVector;

    int intentos = 0;
    const int intentosMax = 1000;
    while (intentos < intentosMax)
    {
        int32 PosX = FMath::RandRange(0, LaberintoBuilder->AnchoLaberinto - 1);
        int32 PosY = FMath::RandRange(0, LaberintoBuilder->AltoLaberinto - 1);
        int32 Index = PosY * LaberintoBuilder->AnchoLaberinto + PosX;
        if (LaberintoBuilder->Laberinto.IsValidIndex(Index) && LaberintoBuilder->Laberinto[Index] == 0)
        {
            return FVector(LaberintoBuilder->LimiteXMin + PosX * LaberintoBuilder->TamanoBloque,
                LaberintoBuilder->LimiteYMin + PosY * LaberintoBuilder->TamanoBloque,
                100.0f);
        }
        intentos++;
    }
    return FVector::ZeroVector; // Si no encuentra, regresa (0,0,0)
}


