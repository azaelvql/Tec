#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace::std;

class Usuario {
private:
    int id = 0;
    string nombre;
    string correo;
    string password;
    int permisos = 0; // 1 admin, 2 cliente

    static shared_ptr<Usuario> currentUser;

public:
    // constructores
    Usuario() = default;
    Usuario(int id, const string& nombre, const string& correo, const string& password, int permisos = 0);

    // métodos existentes (firma)
    bool IniciarSesion();
    bool ValidarInicioSesion(const string& enteredEmail, const string& enteredPassword) const;

    bool RegistrarUsuario(string nombre, string correo, string password);
    vector<shared_ptr<Usuario>> ObtenerUsuarios(const string& filename);

    // getters / setters
    int getId() const;
    void setId(int v);

    string getNombre() const;
    void setNombre(const string& v);

    string getCorreo() const;
    void setCorreo(const string& v);

    string getPassword() const;
    void setPassword(const string& v);

    int getPermisos() const;
    void setPermisos(int v);

    // current user helpers
    static shared_ptr<Usuario> GetCurrentUser();
    static void SetCurrentUser(shared_ptr<Usuario> u);
};
