#include "BloquePared.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABloquePared::ABloquePared()
{

    // Asignamos el tipo de bloque a "Pared"
    TipoDeBloque = EBlockType::Pared;

    // Cargar malla predeterminada de cubo (Shape_Cube) en el constructor
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (Mesh.Succeeded())
    {
        MeshComp->SetStaticMesh(Mesh.Object);  // Asignamos la malla de madera
    }

    // Cargar material para la madera (M_Wood_Oak)
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/StarterContent/Materials/M_Brick_Clay_Beveled.M_Brick_Clay_Beveled"));
    if (Material.Succeeded())
    {
        MeshComp->SetMaterial(0, Material.Object);  // Asignamos el material
    }
}

void ABloquePared::ConfigurarBloque()
{
   
}

