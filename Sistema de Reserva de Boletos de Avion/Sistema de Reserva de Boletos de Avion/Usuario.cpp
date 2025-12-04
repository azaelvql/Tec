#include "Usuario.h"
#include "AdministradorDeSesion.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

shared_ptr<Usuario> Usuario::currentUser = nullptr;

Usuario::Usuario(int id, const string& nombre, const string& correo, const string& password, int permisos)
    : id(id), nombre(nombre), correo(correo), password(password), permisos(permisos) {
}

bool Usuario::ValidarInicioSesion(const string& enteredEmail, const string& enteredPassword) const {
    return (correo == enteredEmail && password == enteredPassword);
}

int Usuario::getId() const { return id; }
void Usuario::setId(int v) { id = v; }

string Usuario::getNombre() const { return nombre; }
void Usuario::setNombre(const string& v) { nombre = v; }

string Usuario::getCorreo() const { return correo; }
void Usuario::setCorreo(const string& v) { correo = v; }

string Usuario::getPassword() const { return password; }
void Usuario::setPassword(const string& v) { password = v; }

int Usuario::getPermisos() const { return permisos; }
void Usuario::setPermisos(int v) { permisos = v; }

shared_ptr<Usuario> Usuario::GetCurrentUser() { return currentUser; }
void Usuario::SetCurrentUser(shared_ptr<Usuario> u) { currentUser = u; }

bool Usuario::IniciarSesion()
{
    //Antes: comprobaciones de existencia de archivos y selección de tipo (cliente/admin)
    AdministradorDeSesion adminSesion;
    int idUsuario = 0;

    cout << "Iniciar como: 1-Cliente  2-Administrador : ";
    int tipo;
    cin >> tipo;
    while (tipo != 1 && tipo != 2) {
        cout << "Seleccione 1 o 2: ";
        cin >> tipo;
    }

    cout << "Usuario (correo): ";
    cin >> correo;

    cout << "Password: ";
    cin >> password;

    // crear archivos si no existen (mismo comportamiento previo)
    {
        ifstream fAdmin("Admin.txt");
        if (!fAdmin.is_open()) {
            ofstream out("Admin.txt");
            if (out.is_open()) {
                out << 1 << " " << "admin" << " " << "admin@admin" << " " << "12345" << endl;
                out.close();
            }
        }
        else fAdmin.close();

        ifstream fClientes("Clientes.txt");
        if (!fClientes.is_open()) {
            ofstream out("Clientes.txt");
            out.close();
        }
        else fClientes.close();
    }

    string archivo = (tipo == 1) ? "Clientes.txt" : "Admin.txt";
    vector<shared_ptr<Usuario>> usuarios = ObtenerUsuarios(archivo);

    int foundId = 0;
    shared_ptr<Usuario> usuarioEncontrado = nullptr;
    for (const auto& u : usuarios) {
        if (u->ValidarInicioSesion(correo, password)) {
            usuarioEncontrado = u;
            foundId = u->getId();
            break;
        }
    }

    if (usuarioEncontrado && adminSesion.login(correo, password, usuarios)) {
        cout << "\nSesion iniciada correctamente para: " << correo << "\n";
        Usuario::SetCurrentUser(usuarioEncontrado);
        cout << "Bienvenido (" << correo << ")\n";
        getchar();
        return true;
    }
    else {
        cout << "\nCredenciales inválidas.\n";
        return false;
    }
}

bool Usuario::RegistrarUsuario(string nombreParam, string correoParam, string passwordParam)
{
    vector<shared_ptr<Usuario>> usuarios = ObtenerUsuarios("Clientes.txt");

    int ultimoId = 0;
    for (const auto& usuario : usuarios)
    {
        if (usuario->getId() > ultimoId) ultimoId = usuario->getId();
    }
    int nuevoId = ultimoId + 1;

    ofstream outputFile("Clientes.txt", ios::app);
    if (outputFile.is_open())
    {
        outputFile << nuevoId << " " << nombreParam << " " << correoParam << " " << passwordParam << endl;
        outputFile.close();
        cout << "Usuario registrado exitosamente en Clientes.txt" << endl;
        getchar();
        return true;
    }
    else
        return false;
}

vector<shared_ptr<Usuario>> Usuario::ObtenerUsuarios(const string& filename)
{
    vector<shared_ptr<Usuario>> usuariosReg;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return usuariosReg;
    }
    else
    {
        string line;
        while (getline(inputFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            int idFile; string nombreFile, correoFile, passFile;
            if (!(ss >> idFile >> nombreFile >> correoFile >> passFile)) continue;
            // determinar permisos segun archivo
            int permisos = 0;
            if (filename == "Admin.txt") permisos = 1;
            else if (filename == "Clientes.txt") permisos = 2;
            usuariosReg.push_back(make_shared<Usuario>(idFile, nombreFile, correoFile, passFile, permisos));
        }
        inputFile.close();
        return usuariosReg;
    }
}
