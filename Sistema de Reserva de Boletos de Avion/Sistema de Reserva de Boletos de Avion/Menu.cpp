#include "Menu.h" 
#include "Usuario.h"
#include "Pago.h"  
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h> 

using namespace std;

// Estructura simple para mantener vuelo en memoria
struct Vuelo {
    int id;
    string origen;
    string destino;
    string salida_fecha;    
    string salida_dia;      
    string salida_hora;     
    string llegada_fecha;
    string llegada_dia;
    string llegada_hora;
    int tipo; // 2=Internacional,1=Nacional
    double precio;
    vector<int> asientos; // tamaño 10, 0 libre,1 ocupado
};

// leer todos los vuelos desde Vuelos.txt
static vector<Vuelo> LeerVuelos() {
    vector<Vuelo> vuelos;
    ifstream in("Vuelos.txt");
    if (!in.is_open()) return vuelos;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Vuelo v;
        v.asientos.assign(10, 0);
        ss >> v.id >> v.origen >> v.destino
            >> v.salida_fecha >> v.salida_dia >> v.salida_hora
            >> v.llegada_fecha >> v.llegada_dia >> v.llegada_hora
            >> v.tipo >> v.precio;
        for (int i = 0; i < 10; ++i) ss >> v.asientos[i];
        vuelos.push_back(v);
    }
    in.close();
    return vuelos;
}

// guardar todos los vuelos (sobrescribe Vuelos.txt)
static bool GuardarVuelos(const vector<Vuelo>& vuelos) {
    ofstream out("Vuelos.txt", ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& v : vuelos) {
        out << v.id << " " << v.origen << " " << v.destino << " "
            << v.salida_fecha << " " << v.salida_dia << " " << v.salida_hora << " "
            << v.llegada_fecha << " " << v.llegada_dia << " " << v.llegada_hora << " "
            << v.tipo << " " << v.precio;
        for (int s : v.asientos) out << " " << s;
        out << "\n";
    }
    out.close();
    return true;
}

// crea archivo Vuelos.txt si no existe
static void AsegurarVuelosExistente() {
    ifstream f("Vuelos.txt");
    if (!f.is_open()) {
        ofstream out("Vuelos.txt");
        out.close();
    }
    else f.close();
}

// crea Reservas.txt si no existe
static void AsegurarReservasExistente() {
    ifstream f("Reservas.txt");
    if (!f.is_open()) {
        ofstream out("Reservas.txt");
        out.close();
    }
    else f.close();
}

// obtiene siguiente id para reservas (basado en Reservas.txt)
static int ObtenerSiguienteIdReserva() {
    ifstream in("Reservas.txt");
    if (!in.is_open()) return 1;
    string line; int last = 0;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int idr; if (ss >> idr) last = idr;
    }
    in.close();
    return last + 1;
}

// obtiene siguiente id vuelo
static int ObtenerSiguienteIdVuelo(const vector<Vuelo>& vuelos) {
    int last = 0;
    for (const auto& v : vuelos) if (v.id > last) last = v.id;
    return last + 1;
}

// mostrar asientos con color
static void MostrarAsientosConColor(const vector<int>& asientos) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < (int)asientos.size(); ++i) {
        if (asientos[i] == 1) {
            SetConsoleTextAttribute(h, 8);
            cout << "[" << (i + 1) << "]";
            SetConsoleTextAttribute(h, 7);
        }
        else {
            cout << "[" << (i + 1) << "]";
        }
        if ((i + 1) % 10 == 0) cout << "\n";
    }
}

// registrar vuelo
static void RegistrarVuelo() {
    AsegurarVuelosExistente();
    vector<Vuelo> vuelos = LeerVuelos();
    Vuelo v;
    v.id = ObtenerSiguienteIdVuelo(vuelos);

    cout << "Origen: "; cin >> v.origen;
    cout << "Destino: "; cin >> v.destino;

    int d, m, a;
    cout << "Fecha de salida - Dia: "; cin >> d;
    cout << "Fecha de salida - Mes: "; cin >> m;
    cout << "Fecha de salida - Año: "; cin >> a;

    char buf[20]; sprintf_s(buf, "%02d/%02d/%04d", d, m, a);
    v.salida_fecha = string(buf);

    cout << "Dia de la semana (Salida): "; cin >> v.salida_dia;

    int hh, mm;
    cout << "Hora salida (hora): "; cin >> hh;
    cout << "Minutos salida: "; cin >> mm;
    sprintf_s(buf, "%02d:%02d", hh, mm);
    v.salida_hora = string(buf);

    cout << "Fecha de llegada - Dia: "; cin >> d;
    cout << "Fecha de llegada - Mes: "; cin >> m;
    cout << "Fecha de llegada - Año: "; cin >> a;
    sprintf_s(buf, "%02d/%02d/%04d", d, m, a);
    v.llegada_fecha = string(buf);

    cout << "Dia de la semana (Llegada): "; cin >> v.llegada_dia;

    cout << "Hora llegada (hora): "; cin >> hh;
    cout << "Minutos llegada: "; cin >> mm;
    sprintf_s(buf, "%02d:%02d", hh, mm);
    v.llegada_hora = string(buf);

    cout << "Precio por asiento: "; cin >> v.precio;
    cout << "Tipo (Internacional=2, Nacional=1): "; cin >> v.tipo;

    v.asientos.assign(10, 0);

    vuelos.push_back(v);
    if (GuardarVuelos(vuelos)) cout << "Vuelo registrado con ID: " << v.id << "\n";
    else cout << "Error al guardar vuelo.\n";

    getchar();
}

// reservar vuelo
static void ReservarVueloParaCliente() {
    AsegurarVuelosExistente();
    AsegurarReservasExistente();
    vector<Vuelo> vuelos = LeerVuelos();

    if (vuelos.empty()) {
        cout << "No hay vuelos disponibles.\n"; getchar(); return;
    }

    cout << "Vuelos disponibles:\n";
    for (const auto& v : vuelos) {
        int disponibles = 0;
        for (int s : v.asientos) if (s == 0) ++disponibles;

        cout << "ID: " << v.id << "  " << v.origen << " - " << v.destino << "\n";
        cout << "Salida: " << v.salida_dia << " " << v.salida_fecha << " " << v.salida_hora << "\n";
        cout << "Llegada: " << v.llegada_dia << " " << v.llegada_fecha << " " << v.llegada_hora << "\n";
        cout << "Asientos disponibles: " << disponibles << "  Precio: $MXN" << v.precio << "\n\n";
    }

    int idSeleccionado;
    cout << "Ingrese ID del vuelo a reservar: "; cin >> idSeleccionado;

    auto it = find_if(vuelos.begin(), vuelos.end(), [&](const Vuelo& x) { return x.id == idSeleccionado; });
    if (it == vuelos.end()) { cout << "ID inválido.\n"; getchar(); return; }

    Vuelo& vuelo = *it;

    cout << "Seleccione asiento (1-10):\n";
    MostrarAsientosConColor(vuelo.asientos);

    int asiento;
    while (true) {
        cout << "\nNumero de asiento: "; cin >> asiento;
        if (asiento < 1 || asiento > 10) { cout << "Asiento inválido.\n"; continue; }
        if (vuelo.asientos[asiento - 1] == 1) { cout << "Asiento ocupado.\n"; continue; }
        vuelo.asientos[asiento - 1] = 1;
        break;
    }

    if (!GuardarVuelos(vuelos)) {
        cout << "Error guardando.\n"; getchar(); return;
    }

    int idRegistro = ObtenerSiguienteIdReserva();
    int idUsuario = Usuario::GetCurrentUser() ? Usuario::GetCurrentUser()->getId() : 0;

    cout << "Desea pagar ahora? 1=Si 2=No: ";
    int paga; cin >> paga;

    string status = "NoPagado";

    ofstream out("Reservas.txt", ios::app);
    if (out.is_open()) {
        out << idRegistro << " " << idUsuario << " " << vuelo.id << " " << asiento << " " << status << "\n";
        out.close();

        cout << "Reserva creada. IDReserva: " << idRegistro << "\n";
        //Pago
        if (paga == 1) {
            Pago p;
            p.MostrarMenuPago(idRegistro, Usuario::GetCurrentUser());
            return;
        }


        else cout << "Regresando al menu del cliente.\n";
    }

    getchar();
}


static void VerReservasCliente() {
    AsegurarReservasExistente();
    AsegurarVuelosExistente();

    auto user = Usuario::GetCurrentUser();
    if (!user) return;

    int userId = user->getId();

    ifstream in("Reservas.txt");
    if (!in.is_open()) {
        cout << "No se pudo abrir Reservas.txt\n";
        getchar();
        return;
    }

    vector<Vuelo> vuelos = LeerVuelos();
    vector<string> reservas;
    string line;

    cout << "\n=== Tus Reservaciones ===\n";

    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int idReserva, idUserFile, idVuelo, asiento;
        string status;

        ss >> idReserva >> idUserFile >> idVuelo >> asiento >> status;

        if (idUserFile == userId) {
            auto it = find_if(vuelos.begin(), vuelos.end(), [&](const Vuelo& v) { return v.id == idVuelo; });
            if (it != vuelos.end()) {
                const Vuelo& v = *it;
                cout << "ID Reserva: " << idReserva << "\n";
                cout << "Vuelo: " << v.origen << " -> " << v.destino << "\n";
                cout << "Fecha/Hora salida: " << v.salida_fecha << " " << v.salida_hora << "\n";
                cout << "Asiento: " << asiento << "\n";
                cout << "Estado: " << status << "\n";
                cout << "---------------------------\n";
            }
        }
    }

    in.close();

    cout << "\n¿Desea pagar una reserva NO PAGADA?\n";
    cout << "Ingrese ID de la reserva o 0 para regresar: ";

    int idSel;
    cin >> idSel;

    if (idSel == 0) return;



    // Verificar si realmente existe y pertenece al usuario y NO está pagada
    {
        ifstream check("Reservas.txt");
        string ln;
        while (getline(check, ln)) {
            stringstream ss(ln);
            int rId, uId, vId, seat;
            string st;
            ss >> rId >> uId >> vId >> seat >> st;

            if (rId == idSel && uId == userId && st == "NoPagado") {
                Pago p;
                p.MostrarMenuPago(idSel, Usuario::GetCurrentUser());
                return;
            }
        }
    }

    // Si no encontró reserva válida para pagar:
    cout << "No es posible pagar esa reserva (no existe, no es tuya o ya está pagada).\n";
    getchar();
}



void Menu::MostrarMenu() const
{
    auto usr = Usuario::GetCurrentUser();

    if (!usr) {
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
                << "2 - Registrar vuelo\n"
                << "3 - Reservar vuelo\n"
                << "4 - Salir\n";
            cin >> opcionMenu;

            switch (opcionMenu)
            {
            case 1: {
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
            case 2:
                RegistrarVuelo();
                break;
            case 3:
                ReservarVueloParaCliente();
                break;
            default:
                break;
            }
        }
        else if (permisos == 2) { // Cliente
            cout << "        Menu - Cliente\n"
                << "1 - Reservar vuelo\n"
                << "2 - Ver mis reservaciones\n"
                << "3 - Salir\n";
            cin >> opcionMenu;

            switch (opcionMenu)
            {
            case 1:
                ReservarVueloParaCliente();
                break;
            case 2:
                VerReservasCliente();
                break;
            default:
                break;
            }
        }
    } while (
        (permisos == 1 && opcionMenu != 4) ||
        (permisos == 2 && opcionMenu != 3)
        );
}
