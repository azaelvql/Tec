#include "Menu.h" 
#include "Vuelo.h"
#include "Usuario.h"
#include "Pago.h"  
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h> 

using namespace::std;



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
        v.setAsientos(vector<int>(10, 0));

        int tmpId; string tmpOrigen, tmpDestino;
        string sfecha, sdia, shora, lfecha, ldia, lhora;
        int tipo; double precio; string numeroAvion;

        ss >> tmpId >> tmpOrigen >> tmpDestino
            >> sfecha >> sdia >> shora
            >> lfecha >> ldia >> lhora
            >> tipo >> precio
            >> numeroAvion;

        v.setId(tmpId);
        v.setOrigen(tmpOrigen);
        v.setDestino(tmpDestino);
        v.setSalidaFecha(sfecha);
        v.setSalidaDia(sdia);
        v.setSalidaHora(shora);
        v.setLlegadaFecha(lfecha);
        v.setLlegadaDia(ldia);
        v.setLlegadaHora(lhora);
        v.setTipo(tipo);
        v.setPrecio(precio);
        v.setNumeroAvion(numeroAvion);

        for (int i = 0; i < 10; ++i) {
            int seat = 0;
            ss >> seat;
            v.setAsiento(i, seat);
        }
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
        out << v.getId() << " " << v.getOrigen() << " " << v.getDestino() << " "
            << v.getSalidaFecha() << " " << v.getSalidaDia() << " " << v.getSalidaHora() << " "
            << v.getLlegadaFecha() << " " << v.getLlegadaDia() << " " << v.getLlegadaHora() << " "
            << v.getTipo() << " " << v.getPrecio() << " "
            << v.getNumeroAvion();

        auto seats = v.getAsientos();
        for (int s : seats) out << " " << s;
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
    for (const auto& v : vuelos) if (v.getId() > last) last = v.getId();
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
    v.setId(ObtenerSiguienteIdVuelo(vuelos));

    cout << "Origen: "; string org; cin >> org; v.setOrigen(org);
    cout << "Destino: "; string dest; cin >> dest; v.setDestino(dest);

    int d, m, a;
    cout << "Fecha de salida - Dia: "; cin >> d;
    cout << "Fecha de salida - Mes: "; cin >> m;
    cout << "Fecha de salida - Año: "; cin >> a;

    char buf[20]; sprintf_s(buf, "%02d/%02d/%04d", d, m, a);
    v.setSalidaFecha(string(buf));

    cout << "Dia de la semana (Salida): "; string sd; cin >> sd; v.setSalidaDia(sd);

    int hh, mm;
    cout << "Hora salida (hora): "; cin >> hh;
    cout << "Minutos salida: "; cin >> mm;
    sprintf_s(buf, "%02d:%02d", hh, mm);
    v.setSalidaHora(string(buf));

    cout << "Fecha de llegada - Dia: "; cin >> d;
    cout << "Fecha de llegada - Mes: "; cin >> m;
    cout << "Fecha de llegada - Año: "; cin >> a;
    sprintf_s(buf, "%02d/%02d/%04d", d, m, a);
    v.setLlegadaFecha(string(buf));

    cout << "Dia de la semana (Llegada): "; string ld; cin >> ld; v.setLlegadaDia(ld);

    cout << "Hora llegada (hora): "; cin >> hh;
    cout << "Minutos llegada: "; cin >> mm;
    sprintf_s(buf, "%02d:%02d", hh, mm);
    v.setLlegadaHora(string(buf));

    cout << "Precio por asiento: "; double precio; cin >> precio; v.setPrecio(precio);
    cout << "Tipo (Internacional=2, Nacional=1): "; int tipo; cin >> tipo; v.setTipo(tipo);

    cout << "Numero de avion (6 digitos): "; string numAv; cin >> numAv; v.setNumeroAvion(numAv);

    v.setAsientos(vector<int>(10, 0));

    vuelos.push_back(v);
    if (GuardarVuelos(vuelos)) cout << "Vuelo registrado con ID: " << v.getId() << "\n";
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
        auto seats = v.getAsientos();
        for (int s : seats) if (s == 0) ++disponibles;

        cout << "ID: " << v.getId() << "  " << v.getOrigen() << " - " << v.getDestino() << "\n";
        cout << "Numero de avion: " << v.getNumeroAvion() << "\n";
        cout << "Salida: " << v.getSalidaDia() << " " << v.getSalidaFecha() << " " << v.getSalidaHora() << "\n";
        cout << "Llegada: " << v.getLlegadaDia() << " " << v.getLlegadaFecha() << " " << v.getLlegadaHora() << "\n";
        cout << "Asientos disponibles: " << disponibles << "  Precio: $MXN" << v.getPrecio() << "\n\n";
    }

    int idSeleccionado;
    cout << "Ingrese ID del vuelo a reservar: "; cin >> idSeleccionado;

    auto it = find_if(vuelos.begin(), vuelos.end(), [&](const Vuelo& x) { return x.getId() == idSeleccionado; });
    if (it == vuelos.end()) { cout << "ID inválido.\n"; getchar(); return; }

    Vuelo& vuelo = *it;

    cout << "Seleccione asiento (1-10):\n";
    MostrarAsientosConColor(vuelo.getAsientos());

    int asiento;
    while (true) {
        cout << "\nNumero de asiento: "; cin >> asiento;
        if (asiento < 1 || asiento > 10) { cout << "Asiento inválido.\n"; continue; }
        if (vuelo.getAsiento(asiento - 1) == 1) { cout << "Asiento ocupado.\n"; continue; }
        vuelo.setAsiento(asiento - 1, 1);
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
        out << idRegistro << " " << idUsuario << " " << vuelo.getId() << " " << asiento << " " << status << "\n";
        out.close();

        cout << "Reserva creada. IDReserva: " << idRegistro << "\n";

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
            auto it = find_if(vuelos.begin(), vuelos.end(), [&](const Vuelo& v) { return v.getId() == idVuelo; });
            if (it != vuelos.end()) {
                const Vuelo& v = *it;
                cout << "ID Reserva: " << idReserva << "\n";
                cout << "Vuelo: " << v.getOrigen() << " -> " << v.getDestino() << "\n";
                cout << "Numero de avion: " << v.getNumeroAvion() << "\n";
                cout << "Fecha/Hora salida: " << v.getSalidaFecha() << " " << v.getSalidaHora() << "\n";
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
