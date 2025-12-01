#pragma once
#include "Usuario.h"

class Cliente : 
    public Usuario {
public:
    Cliente() { permisos = 2; } // permiso 2 = cliente
    Cliente(int id, string nombre, string correo, string password) : Usuario(id, nombre, correo, password, 2) {}
    // Cliente solo puede iniciar sesión (no registrar)
};
