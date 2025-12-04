#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <string>
#include <memory>
#include "Usuario.h"

using namespace::std;

class Pago {
public:
    void MostrarMenuPago(int idReserva, shared_ptr<Usuario> usuario);
};
