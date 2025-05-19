// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Moneda.generated.h"

UCLASS()
class BOMBERMANJCCSIS457_API AMoneda : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoneda();

private:
	// temporizador
	FTimerHandle SaltoTimerHandle;
private:
	// Posinicial
	FVector PosicionInicial;

	// Posici�n actual
	FVector PosicionObjetivo;

	// Temporizador para alternar entre posiciones
	FTimerHandle MovimientoTimerHandle;


protected:
	// Componente de malla
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	// Variables de l�mites del laberinto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteXMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteXMax = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteYMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laberinto")
	float LimiteYMax = 3000.0f;

	// Atributos de rotaci�n
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moneda")
	float VelocidadRotacion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moneda")
	float AnguloMaximoRotacion;

	// Direcci�n de la rotaci�n
	FVector EjeRotacion;

	// M�todos
	
	void CambiarDireccionRotacion();
	FVector ObtenerPosicionValida();

public:
	void Teletransportar(FVector NuevaPosicion);
	void TeletransporteAleatorio();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
