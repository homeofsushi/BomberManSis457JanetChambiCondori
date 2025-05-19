#include "Moneda.h"
#include "Components/StaticMeshComponent.h"
#include "BomberManJCCSIS457GameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AMoneda::AMoneda()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/ThirdPerson/Maps/_GENERATED/ENVY/Sphere_EAD5A969.Sphere_EAD5A969"));
	if (Mesh.Succeeded())
	{
		MeshComp->SetStaticMesh(Mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (Material.Succeeded())
	{
		MeshComp->SetMaterial(0, Material.Object);
	}

	VelocidadRotacion = 30.0f;
	AnguloMaximoRotacion = 45.0f;
	EjeRotacion = FVector(1, 0, 0);
}

void AMoneda::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MovimientoTimerHandle, this, &AMoneda::TeletransporteAleatorio, 3.0f, true);
}

void AMoneda::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Rotacion = FRotator(0, VelocidadRotacion * DeltaTime, 0);
	MeshComp->AddRelativeRotation(Rotacion);

	if (FMath::Abs(MeshComp->GetComponentRotation().Yaw) >= AnguloMaximoRotacion)
	{
		CambiarDireccionRotacion();
	}
}

void AMoneda::CambiarDireccionRotacion()
{
	VelocidadRotacion = -VelocidadRotacion;
}

void AMoneda::Teletransportar(FVector NuevaPosicion)
{
	SetActorLocation(NuevaPosicion);
}

void AMoneda::TeletransporteAleatorio()
{
	auto* GameMode = Cast<ABomberManJCCSIS457GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		FVector NuevaPos = GameMode->ObtenerPosicionLibreAleatoria();
		SetActorLocation(NuevaPos);
	}
}


