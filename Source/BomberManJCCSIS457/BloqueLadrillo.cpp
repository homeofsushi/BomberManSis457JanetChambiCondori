// Fill out your copyright notice in the Description page of Project Settings.


#include "BloqueLadrillo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABloqueLadrillo::ABloqueLadrillo()
{
	// Asignamos el tipo de bloque a "Ladrillo"
	TipoDeBloque = EBlockType::Ladrillo;
    // Cargar malla predeterminada de cubo (Shape_Cube) en el constructor
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (Mesh.Succeeded())
    {
        MeshComp->SetStaticMesh(Mesh.Object);  // Asignamos la malla de madera
    }

    // Cargar material para la madera (M_Wood_Oak)
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New"));
    if (Material.Succeeded())
    {
        MeshComp->SetMaterial(0, Material.Object);  // Asignamos el material
    }
}

void ABloqueLadrillo::ConfigurarBloque()
{
	
}

