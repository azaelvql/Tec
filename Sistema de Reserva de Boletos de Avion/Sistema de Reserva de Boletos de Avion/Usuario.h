#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

class Usuario
{
	//atributos
public:
	int id = 0;
	string nombre;
	string correo;
	string password;
	int permisos = 0; // 1 = admin, 2 = cliente, 0 = genérico

	//métodos
public:
	//constructor parametrizado
	Usuario(int id, string nombre, string correo, string password, int permisos = 0)
		:id(id), nombre(nombre), correo(correo), password(password), permisos(permisos) {
	};

	//constructor por defecto
	Usuario() {};

	// Mantener firma original
	bool IniciarSesion();

	bool ValidarInicioSesion(const string& enteredEmail,
		const string& enteredPassword) const {
		return (correo == enteredEmail && password == enteredPassword);
	};

	int getId() const;
	string getCorreo() const;
	int getPermisos() const { return permisos; }

	// Registrar un usuario (escribirá en Clientes.txt)
	bool RegistrarUsuario(string nombre, string correo, string password);

	// Obtener usuarios desde archivo indicado (Admin.txt o Clientes.txt)
	vector<shared_ptr<Usuario>> ObtenerUsuarios(const string& filename);

	// Puntero al usuario que inició sesión para que Menu pueda consultarlo
	static shared_ptr<Usuario> currentUser;
	static shared_ptr<Usuario> GetCurrentUser() { return currentUser; }
};
