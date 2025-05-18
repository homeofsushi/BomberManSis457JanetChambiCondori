#pragma once

#include "ILaberintoBuilder.h"

class LaberintoDirector
{
public:
    void SetBuilder(ILaberintoBuilder* InBuilder)
    {
        Builder = InBuilder;
    }

    void ConstruirLaberintoCompleto()
    {
        if (!Builder) return;

        Builder->CrearLaberinto();
        Builder->AgregarBloques();
        Builder->AgregarPuertas();
        Builder->AgregarMoneda();
    }

private:
    ILaberintoBuilder* Builder;
};
