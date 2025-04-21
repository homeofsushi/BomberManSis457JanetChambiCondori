// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloqueBase.generated.h"

// Enum para los diferentes tipos de bloques
UENUM(BlueprintType)
enum class EBlockType : uint8
{
	Pared UMETA(DisplayName = "Pared"),
	Madera UMETA(DisplayName = "Madera"),
	Acero UMETA(DisplayName = "Acero"),
	Ladrillo UMETA(DisplayName = "Ladrillo")
};

UCLASS()
class BOMBERMANJCCSIS457_API ABloqueBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloqueBase();
	//TIPO DE BLOQUE PARA MI ENUM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bloque")
	EBlockType TipoDeBloque;

	// Componente de malla
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	// Función para configurar el bloque, la implementaremos en las clases derivadas
	virtual void ConfigurarBloque();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
