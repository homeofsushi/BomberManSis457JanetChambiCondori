// Fill out your copyright notice in the Description page of Project Settings.


#include "BloqueBase.h"

// Sets default values
ABloqueBase::ABloqueBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Crear el componente de malla
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;  // Establecer la malla como ra�z del actor
	// Ajustar la escala para que coincida con la altura del personaje
	MeshComp->SetWorldScale3D(FVector(1.0f, 1.0f, 2.0f)); // Ajusta el valor Z seg�n la altura del personaje
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
void ABloqueBase::BeginPlay()
{
	Super::BeginPlay();
	// Llamar a la funci�n ConfigurarBloque para asignar malla y materiales espec�ficos
	ConfigurarBloque();
	
}
// L�gica para configurar el bloque
void ABloqueBase::ConfigurarBloque()
{
	// Aqu� dejamos la l�gica de configuraci�n vac�a ya que cada clase derivada la va a sobrecargar con sus propios detalles
	// (como la malla y el material espec�ficos)
}
// Called every frame
void ABloqueBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

