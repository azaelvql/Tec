#include <iostream>
#include <Windows.h>
#include "Usuario.h" 
#include "Menu.h"

using namespace std;

class SistemaPrincipal {
public: //atributos 
	int op; 
	Usuario usuario; 
	string nombre, correo, password;
public: //métodos 
	SistemaPrincipal() 
	{
		cout << "1. Iniciar sesion\n"; 
		cin >> op; 
		system("cls"); 
		switch (op) 
		{
		case 1: 
			if (usuario.IniciarSesion()) 
			{ 
				Menu menu; 
				menu.MostrarMenu(); 
			}
			  else cout << "Error al iniciar sesion" << endl; 
			break;

		default: 
			break;
		}
	}
};
int main()
{
	SistemaPrincipal();
}