#pragma once
#include "AdministradorDeSesion.h"
class Reservacion
{
public:
	//atributos

public:
	//métodos
	bool ReservarVuelo(int idUsuario)
	{
		AdministradorDeSesion adminSesion;
		
		if (adminSesion.isLoggedIn(idUsuario))
		{
			//realizar la reservacion del vuelo
		}
		else
		{
			//el usuario no tiene sesion activa
		}
	}
};

