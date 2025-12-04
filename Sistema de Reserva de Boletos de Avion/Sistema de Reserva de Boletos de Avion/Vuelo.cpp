#include "Vuelo.h"
#include <algorithm>

using namespace::std;

Vuelo::Vuelo() : id(0), tipo(0), precio(0.0), numeroAvion("000000"), asientos(10, 0) {}

int Vuelo::getId() const { return id; }
void Vuelo::setId(int v) { id = v; }

string Vuelo::getOrigen() const { return origen; }
void Vuelo::setOrigen(const ::string& v) { origen = v; }

string Vuelo::getDestino() const { return destino; }
void Vuelo::setDestino(const ::string& v) { destino = v; }

string Vuelo::getSalidaFecha() const { return salida_fecha; }
void Vuelo::setSalidaFecha(const ::string& v) { salida_fecha = v; }

string Vuelo::getSalidaDia() const { return salida_dia; }
void Vuelo::setSalidaDia(const ::string& v) { salida_dia = v; }

string Vuelo::getSalidaHora() const { return salida_hora; }
void Vuelo::setSalidaHora(const ::string& v) { salida_hora = v; }

string Vuelo::getLlegadaFecha() const { return llegada_fecha; }
void Vuelo::setLlegadaFecha(const ::string& v) { llegada_fecha = v; }

string Vuelo::getLlegadaDia() const { return llegada_dia; }
void Vuelo::setLlegadaDia(const ::string& v) { llegada_dia = v; }

string Vuelo::getLlegadaHora() const { return llegada_hora; }
void Vuelo::setLlegadaHora(const ::string& v) { llegada_hora = v; }

int Vuelo::getTipo() const { return tipo; }
void Vuelo::setTipo(int v) { tipo = v; }

double Vuelo::getPrecio() const { return precio; }
void Vuelo::setPrecio(double v) { precio = v; }

string Vuelo::getNumeroAvion() const { return numeroAvion; }
void Vuelo::setNumeroAvion(const ::string& v) { numeroAvion = v; }

vector<int> Vuelo::getAsientos() const { return asientos; }
void Vuelo::setAsientos(const ::vector<int>& v) { asientos = v; }
int Vuelo::getAsiento(int index) const {
    if (index < 0 || index >= (int)asientos.size()) return -1;
    return asientos[index];
}
void Vuelo::setAsiento(int index, int val) {
    if (index < 0 || index >= (int)asientos.size()) return;
    asientos[index] = val;
}
