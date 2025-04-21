// Fill out your copyright notice in the Description page of Project Settings.

#include "Moneda.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "BomberManJCCSIS457GameMode.h"
#include "BomberManJCCSIS457Character.h"

// Sets default values
AMoneda::AMoneda()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Establecer el componente de malla
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	// Cargar la malla de la moneda
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/ThirdPerson/Maps/_GENERATED/ENVY/Sphere_EAD5A969.Sphere_EAD5A969"));
	if (Mesh.Succeeded())
	{
		MeshComp->SetStaticMesh(Mesh.Object);  // Asigna la malla de la moneda
	}

	// Cargar el material de la moneda
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (Material.Succeeded())
	{
		MeshComp->SetMaterial(0, Material.Object);  // Asigna el material de la moneda
	}
	
	VelocidadRotacion = 30.0f;  
	AnguloMaximoRotacion = 45.0f;  // Ángulo máximo de rotación
	EjeRotacion = FVector(1, 0, 0);  

}

// Called when the game starts or when spawned
void AMoneda::BeginPlay()
{
	Super::BeginPlay();

	//posición inicial de la moneda
	PosicionInicial = GetActorLocation();
	PosicionObjetivo = PosicionInicial;

	// Configurar el temporizador para alternar entre posiciones
	GetWorldTimerManager().SetTimer(MovimientoTimerHandle, this, &AMoneda::AlternarPosicion, 10.0f, true);
}
void AMoneda::AlternarPosicion()
{
	// Si la posición objetivo es la inicial, calcula una nueva posición
	if (PosicionObjetivo == PosicionInicial)
	{
		PosicionObjetivo = ObtenerPosicionValida();
	}
	else
	{
		// Si no, vuelve a la posición inicial
		PosicionObjetivo = PosicionInicial;
	}

	// Mover la moneda a la posición objetivo
	SetActorLocation(PosicionObjetivo);
}


// Called every frame
void AMoneda::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator Rotacion = FRotator(0, VelocidadRotacion * DeltaTime, 0);
	MeshComp->AddRelativeRotation(Rotacion);

	// Verificar si se alcanzó el ángulo máximo
	if (FMath::Abs(MeshComp->GetComponentRotation().Yaw) >= AnguloMaximoRotacion)
	{
		CambiarDireccionRotacion();   
	}

}
FVector AMoneda::ObtenerPosicionValida()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ABomberManJCCSIS457GameMode* GameMode = Cast<ABomberManJCCSIS457GameMode>(UGameplayStatics::GetGameMode(World));
		if (GameMode)
		{
			// Generar coordenadas aleatorias dentro de los límites del laberinto
			int32 PosX, PosY;
			do
			{
				PosX = FMath::RandRange(0, GameMode->AnchoLaberinto - 1);
				PosY = FMath::RandRange(0, GameMode->AltoLaberinto - 1);
			} while (GameMode->Laberinto[PosY * GameMode->AnchoLaberinto + PosX] != 0); // Asegurarse de que esté vacío

			// Convertir las coordenadas del laberinto a coordenadas del mundo
			float X = GameMode->LimiteXMin + PosX * GameMode->TamanoBloque;
			float Y = GameMode->LimiteYMin + PosY * GameMode->TamanoBloque;
			float Z = GameMode->TamanoBloque / 2.0f; // Ajustar la altura para que coincida con los bloques

			return FVector(X, Y, Z);
		}
	}

	// Si no se puede obtener el mundo o el GameMode, retornar una posición por defecto
	return FVector(0.0f, 0.0f, 100.0f);
}



void AMoneda::CambiarDireccionRotacion()
{
	VelocidadRotacion = -VelocidadRotacion;
}

