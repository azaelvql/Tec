#include "Menu.h"
#include "Usuario.h"
#include <iostream>

void Menu::MostrarMenu() const
{
    // Obtener usuario que inició sesión (seteado en Usuario::IniciarSesion)
    auto usr = Usuario::GetCurrentUser();

    if (!usr) {
        // Sin usuario, no mostrar menú
        std::cout << "No hay usuario logueado.\n";
        return;
    }

    int permisos = usr->getPermisos();
    string correo = usr->getCorreo();

    int opcionMenu;
    do {
        system("cls");
        cout << "Bienvenido (" << correo << ")\n\n";

        if (permisos == 1) { // Admin
            cout << "        Menu - Administrador\n"
                << "1 - Registrar usuario\n"
                << "2 - Reservar vuelo\n"
                << "3 - Pagar boleto\n"
                << "4 - Salir\n";
            cin >> opcionMenu;

            switch (opcionMenu)
            {
            case 1: {
                // Registrar usuario (llama a Usuario::RegistrarUsuario)
                string nombre, correoNew, password;
                cout << "Nombre: "; cin >> nombre;
                cout << "Correo: "; cin >> correoNew;
                cout << "Password: "; cin >> password;
                Usuario u;
                if (u.RegistrarUsuario(nombre, correoNew, password)) {
                    cout << "Registrado correctamente.\n";
                }
                else cout << "Error al registrar.\n";
                getchar();
                break;
            }
            case 2: cout << "Reservar vuelo (Admin)" << endl; getchar(); break;
            case 3: cout << "Pagar boleto (Admin)" << endl; getchar(); break;
            default:
                break;
            }
        }
        else if (permisos == 2) { // Cliente
            cout << "        Menu - Cliente\n"
                << "1 - Reservar vuelo\n"
                << "2 - Pagar boleto\n"
                << "3 - Salir\n";
            cin >> opcionMenu;

            switch (opcionMenu)
            {
            case 1: cout << "Reservar vuelo (Cliente)" << endl; getchar(); break;
            case 2: cout << "Pagar boleto (Cliente)" << endl; getchar(); break;
            default:
                break;
            }
        }
        else { // permisos desconocidos
            cout << "        Menu\n"
                << "1 - Reservar vuelo\n"
                << "2 - Pagar boleto\n"
                << "3 - Salir" << endl;
            cin >> opcionMenu;
        }
    } while ((permisos == 1 && opcionMenu != 4) || (permisos != 1 && opcionMenu != 3));
}
