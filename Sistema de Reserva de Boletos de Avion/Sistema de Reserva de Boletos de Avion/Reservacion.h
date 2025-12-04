#pragma once
#include "AdministradorDeSesion.h"

using namespace::std;

class Reservacion
{
public:
	//atributos

public:
	//métodos
	bool Reservar(int idUsuario)
	{
		AdministradorDeSesion adminSesion;
		
		if (adminSesion.isLoggedIn(idUsuario))
		{
			//realizar la reservacion del 
		}
		else
		{
			//el usuario no tiene sesion activa
		}
	}
};

