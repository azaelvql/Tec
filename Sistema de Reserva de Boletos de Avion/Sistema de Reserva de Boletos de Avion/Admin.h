#pragma once
#include "Usuario.h"

class Admin :
    public Usuario {
public:
    Admin() { permisos = 1; } // permiso 1 = admin
    Admin(int id, string nombre, string correo, string password) : Usuario(id, nombre, correo, password, 1) {}
    // Admin puede registrar (usa Usuario::RegistrarUsuario que escribe en Clientes.txt)
};
