#pragma once
#include <map>
#include <memory>
#include "Usuario.h"

using namespace::std;

class AdministradorDeSesion
{
private:
    map<int, shared_ptr<Usuario>> sesionesActivas;

public:
    bool login(const string& correo, const string& password, const vector<shared_ptr<Usuario>>& usuarios) {
        for (const auto& usuario : usuarios) {
            if (usuario->ValidarInicioSesion(correo, password)) {
                sesionesActivas[usuario->getId()] = usuario;
                return true;
            }
        }
        return false;
    }

    void logout(int userId) {
        sesionesActivas.erase(userId);
    }
    bool isLoggedIn(int userId) const {
        return sesionesActivas.find(userId) != sesionesActivas.end();
    }

    shared_ptr<Usuario> getSesionUsuario(int idUsuario) const {
        auto it = sesionesActivas.find(idUsuario);
        if (it != sesionesActivas.end()) {
            return it->second;
        }
        return nullptr;
    }
};
