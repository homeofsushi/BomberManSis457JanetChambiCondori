#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "LaberintoBuilderAleatorio.h"
#include "BloqueBase.h"
#include "Moneda.h"
#include "PuertaTeletransportadora.h"

#include "BomberManJCCSIS457GameMode.generated.h"

class LaberintoBuilderAleatorio;
UCLASS(minimalapi)
class ABomberManJCCSIS457GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABomberManJCCSIS457GameMode();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    float LimiteXMin = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    float LimiteYMin = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    float LimiteXMax = 2850.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    float LimiteYMax = 3350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    int32 AnchoLaberinto = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    int32 AltoLaberinto = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
    float TamanoBloque = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Clases")
    TSubclassOf<ABloqueBase> ClaseBloqueMadera;

    UPROPERTY(EditDefaultsOnly, Category = "Clases")
    TSubclassOf<ABloqueBase> ClaseBloqueHierro;

    UPROPERTY(EditDefaultsOnly, Category = "Clases")
    TSubclassOf<AMoneda> ClaseMoneda;

    UPROPERTY(EditDefaultsOnly, Category = "Clases")
    TSubclassOf<APuertaTeletransportadora> ClasePuerta;

    TSharedPtr<LaberintoBuilderAleatorio> LaberintoBuilder;

    UPROPERTY(BlueprintReadOnly, Category = "Laberinto")
    TArray<int32> Laberinto;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    // Declaración del puntero al builder
    LaberintoBuilderAleatorio* Builder;
};