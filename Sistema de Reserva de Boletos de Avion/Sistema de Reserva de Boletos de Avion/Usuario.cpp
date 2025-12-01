#include "Usuario.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "AdministradorDeSesion.h"

shared_ptr<Usuario> Usuario::currentUser = nullptr;

bool Usuario::IniciarSesion()
{
    // Antes de pedir datos, revisar existencia de archivos y crearlos si no existen.
    {
        ifstream fAdmin("Admin.txt");
        if (!fAdmin.is_open()) {
            // crear Admin.txt con admin por defecto
            ofstream out("Admin.txt");
            if (out.is_open()) {
                // id nombre correo password  (separados por espacios)
                out << 1 << " " << "admin" << " " << "admin@admin" << " " << "12345" << endl;
                out.close();
            }
        }
        else fAdmin.close();

        ifstream fClientes("Clientes.txt");
        if (!fClientes.is_open()) {
            // crear Clientes.txt vacío
            ofstream out("Clientes.txt");
            out.close();
        }
        else fClientes.close();
    }

    // Preguntar si inicia como cliente o administrador
    int tipo = 0;
    cout << "Iniciar como: 1-Cliente  2-Administrador : ";
    cin >> tipo;
    while (tipo != 1 && tipo != 2) {
        cout << "Seleccione 1 o 2: ";
        cin >> tipo;
    }

    // Pedir credenciales
    cout << "Usuario (correo): ";
    cin >> correo;

    cout << "Password: ";
    cin >> password;

    // Determinar archivo a usar para login
    string archivo = (tipo == 1) ? "Clientes.txt" : "Admin.txt";

    // Crear vector de usuarios desde archivo seleccionado
    vector<shared_ptr<Usuario>> usuarios = ObtenerUsuarios(archivo);

    // Buscar credenciales en vector
    int idUsuario = 0;
    shared_ptr<Usuario> usuarioEncontrado = nullptr;
    for (const shared_ptr<Usuario>& u : usuarios) {
        if (u->ValidarInicioSesion(correo, password)) {
            idUsuario = u->getId();
            usuarioEncontrado = u;
            break;
        }
    }

    // AdministradorDeSesion maneja el registro de sesión en memoria
    AdministradorDeSesion adminSesion;
    if (usuarioEncontrado && adminSesion.login(correo, password, usuarios)) {
        // guardar referencia al usuario actual para que Menu la consulte
        currentUser = usuarioEncontrado;

        // Mostrar mensaje con correo (requisito)
        std::cout << "\nSesion iniciada correctamente para: " << correo << "\n";
        std::cout << "Bienvenido (" << correo << ")\n";
        getchar();
        return true;
    }
    else {
        std::cout << "\nCredenciales inválidas.\n";
        getchar();
        return false;
    }
}

int Usuario::getId() const
{
    return id;
}

string Usuario::getCorreo() const
{
    return correo;
}

bool Usuario::RegistrarUsuario(string nombre, string correo, string password)
{
    // Registrar en Clientes.txt (no en Admin.txt)
    vector<shared_ptr<Usuario>> usuarios = ObtenerUsuarios("Clientes.txt");

    int ultimoId = 0;
    for (const shared_ptr<Usuario>& usuario : usuarios)
    {
        if (usuario->getId() > ultimoId) ultimoId = usuario->getId();
    }
    int nuevoId = ultimoId + 1;

    //Guardar usuario en archivo de texto (Clientes.txt)
    ofstream outputFile("Clientes.txt", ios::app);

    if (outputFile.is_open())
    {
        // guardamos: id nombre correo password (separados por espacios)
        outputFile << nuevoId << " " << nombre << " " << correo << " " << password << endl;
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
    //leer el archivo filename y sacar los datos linea por linea
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Archivo no disponible: " << filename << std::endl;
        return usuariosReg; // Salida vacía
    }
    else
    {
        string line;
        vector<string> words;
        string word;

        while (getline(inputFile, line)) { // leer linea por linea
            if (line.empty()) continue;
            //extraer los tokens de cada linea del archivo (separador espacio)
            stringstream ss(line);
            words.clear();
            while (ss >> word) {
                words.push_back(word);
            }
            // seguridad: si no tiene al menos 4 tokens, saltar
            if (words.size() < 4) continue;

            // determinar permisos: Admin.txt -> permisos = 1, Clientes.txt -> permisos = 2
            int permisos = 0;
            if (filename == "Admin.txt") permisos = 1;
            else if (filename == "Clientes.txt") permisos = 2;

            usuariosReg.push_back(make_shared<Usuario>(stoi(words.at(0)),
                words.at(1), words.at(2), words.at(3), permisos));
        }
        inputFile.close();
        return usuariosReg;
    }
}
