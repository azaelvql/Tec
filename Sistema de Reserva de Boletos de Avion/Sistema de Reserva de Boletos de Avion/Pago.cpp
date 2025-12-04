#include "Pago.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <cctype>

#pragma comment (lib, "Ws2_32.lib")

using namespace::std;



static bool EnviarTarjetaAlServer(const string& servidor, const string& tarjeta, int& bytesEnviados, int& bytesRecibidos) {
    const int DEFAULT_BUFLEN = 512;
    const char* DEFAULT_PORT = "27015";

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    const char* sendbuf = tarjeta.c_str();
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(servidor.c_str(), DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }
    printf("Bytes Sent: %ld\n", iResult);
    bytesEnviados = iResult;

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }

    int totalRecv = 0;
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            totalRecv += iResult;
        }
        else if (iResult == 0) {
            printf("Connection closed\n");
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
        }
    } while (iResult > 0);

    bytesRecibidos = totalRecv;
    closesocket(ConnectSocket);
    WSACleanup();
    return true;
}

static bool MarcarReservaComoPagada(int idReserva, int idUsuario) {
    ifstream in("Reservas.txt");
    if (!in.is_open()) return false;

    vector<string> lineas;
    string line;
    bool cambiado = false;

    while (getline(in, line)) {
        if (line.empty()) { lineas.push_back(line); continue; }
        stringstream ss(line);
        int idReg = 0, idUsr = 0, idVuelo = 0, asiento = 0;
        string status;
        if (!(ss >> idReg >> idUsr >> idVuelo >> asiento >> status)) {
            lineas.push_back(line);
            continue;
        }
        if (idReg == idReserva && idUsr == idUsuario) {
            status = "Pagado";
            stringstream nuevo;
            nuevo << idReg << " " << idUsr << " " << idVuelo << " " << asiento << " " << status;
            lineas.push_back(nuevo.str());
            cambiado = true;
        }
        else {
            lineas.push_back(line);
        }
    }
    in.close();

    if (!cambiado) return false;
    ofstream out("Reservas.txt", ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& l : lineas) out << l << "\n";
    out.close();
    return true;
}

void Pago::MostrarMenuPago(int idReserva, shared_ptr<Usuario> usuario) {
    if (!usuario) { cout << "Usuario no válido.\n"; return; }

    // comprobar que exista y sea NoPagado
    {
        ifstream in("Reservas.txt");
        if (!in.is_open()) { cout << "No existe Reservas.txt\n"; return; }
        bool encontrada = false;
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            int idReg, idUsr, idVuelo, asiento;
            string status;
            if (!(ss >> idReg >> idUsr >> idVuelo >> asiento >> status)) continue;
            if (idReg == idReserva && idUsr == usuario->getId()) {
                encontrada = true;
                if (status == "Pagado") {
                    cout << "La reserva ya está pagada.\n";
                    return;
                }
                break;
            }
        }
        if (!encontrada) {
            cout << "No se encontró la reserva o no pertenece al usuario.\n";
            return;
        }
    }

    string tarjeta;
    while (true) {
        cout << "Ingrese numero de tarjeta (16 digitos): ";
        cin >> tarjeta;
        if (tarjeta.size() != 16) { cout << "La tarjeta debe tener exactamente 16 digitos.\n"; continue; }
        bool solo = true;
        for (char c : tarjeta) if (!isdigit((unsigned char)c)) { solo = false; break; }
        if (!solo) { cout << "Ingrese solo numeros.\n"; continue; }
        break;
    }

    int be = 0, br = 0;
    cout << "Conectando y enviando tarjeta al servidor local...\n";
    bool ok = EnviarTarjetaAlServer("localhost", tarjeta, be, br);
    if (!ok) { cout << "Error al enviar los datos al servidor.\n"; return; }


    int idUsuario = usuario->getId();
    bool marcado = MarcarReservaComoPagada(idReserva, idUsuario);
    if (marcado) {
        cout << "Tu reserva fue pagada\n";
    }
    else {
        cout << "No se encontró la reserva para marcar como pagada o no pertenece al usuario.\n";
    }

    cout << "Presiona Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
