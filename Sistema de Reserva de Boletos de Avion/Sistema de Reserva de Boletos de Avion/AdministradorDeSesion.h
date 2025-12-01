#include <map>
#include <memory>
#include"Usuario.h"

#pragma once

class AdministradorDeSesion
{
private:
	//atributos
	std::map<int, std::shared_ptr<Usuario>> sesionesActivas;

public:
	//métodos
	bool login(const std::string& correo, const std::string& password, const std::vector<std::shared_ptr<Usuario>>& usuarios) {
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

	std::shared_ptr<Usuario> getSesionUsuario(int idUsuario) const {
		auto it = sesionesActivas.find(idUsuario);
		if (it != sesionesActivas.end()) {
			return it->second;
		}
		return nullptr;
	}
};

