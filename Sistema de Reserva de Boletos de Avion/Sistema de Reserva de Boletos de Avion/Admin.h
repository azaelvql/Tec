#pragma once
#include "Usuario.h"

using namespace::std;

class Admin : public Usuario {
public:
    Admin() { setPermisos(1); }
    Admin(int id, const string& nombre, const string& correo, const string& password)
        : Usuario(id, nombre, correo, password, 1) {
    }
};
