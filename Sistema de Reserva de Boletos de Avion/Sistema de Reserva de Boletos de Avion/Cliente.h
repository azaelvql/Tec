#pragma once
#include "Usuario.h"

using namespace::std;

class Cliente : public Usuario {
public:
    Cliente() { setPermisos(2); }
    Cliente(int id, const string& nombre, const string& correo, const string& password)
        : Usuario(id, nombre, correo, password, 2) {
    }
};
