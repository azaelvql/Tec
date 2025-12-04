#ifndef VUELO_H
#define VUELO_H

#include <string>
#include <vector>

using namespace::std;

class Vuelo {
private:
    int id;
    string origen;
    string destino;

    string salida_fecha;
    string salida_dia;
    string salida_hora;

    string llegada_fecha;
    string llegada_dia;
    string llegada_hora;

    int tipo;
    double precio;

    string numeroAvion;

    vector<int> asientos;

public:
    Vuelo();
    // getters/setters
    int getId() const;
    void setId(int v);

    string getOrigen() const;
    void setOrigen(const string& v);

    string getDestino() const;
    void setDestino(const string& v);

    string getSalidaFecha() const;
    void setSalidaFecha(const string& v);

    string getSalidaDia() const;
    void setSalidaDia(const string& v);

    string getSalidaHora() const;
    void setSalidaHora(const string& v);

    string getLlegadaFecha() const;
    void setLlegadaFecha(const string& v);

    string getLlegadaDia() const;
    void setLlegadaDia(const string& v);

    string getLlegadaHora() const;
    void setLlegadaHora(const string& v);

    int getTipo() const;
    void setTipo(int v);

    double getPrecio() const;
    void setPrecio(double v);

    string getNumeroAvion() const;
    void setNumeroAvion(const string& v);

    // asientos
    vector<int> getAsientos() const;
    void setAsientos(const vector<int>& v);
    int getAsiento(int index) const;
    void setAsiento(int index, int val);
};

#endif
