#pragma once
#include "BomberManJCCSIS457GameMode.h"

class ILaberintoBuilder {
public:
    virtual ~ILaberintoBuilder() = default;
    virtual void CrearLaberinto() = 0;
    virtual void AgregarBloques() = 0;
    virtual void AgregarPuertas() = 0;
    virtual void AgregarMoneda() = 0;
};
