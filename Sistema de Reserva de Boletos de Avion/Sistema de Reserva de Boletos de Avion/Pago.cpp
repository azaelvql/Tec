// Pago.cpp
#include "Pago.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>       // necesario para std::numeric_limits
#include <cctype>       // isdigit

static bool EnviarTarjetaAlServer(const std::string& servidor, const std::string& tarjeta, int& bytesEnviados, int& bytesRecibidos) {
    const int DEFAULT_BUFLEN = 512;
    const char* DEFAULT_PORT = "27015";

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    const char* sendbuf = tarjeta.c_str();
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(servidor.c_str(), DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
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

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }

    printf("Bytes Sent: %ld\n", iResult);
    bytesEnviados = iResult;

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }

    // Receive until the peer closes the connection
    int totalRecv = 0;
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            totalRecv += iResult;
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    bytesRecibidos = totalRecv;

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return true;
}

// Cambia el status de la reserva (por idReserva) a "Pagado" en Reservas.txt
static bool MarcarReservaComoPagada(int idReserva, int idUsuario) {
    std::ifstream in("Reservas.txt");
    if (!in.is_open()) return false;

    std::vector<std::string> lineas;
    std::string line;
    bool cambiado = false;

    while (std::getline(in, line)) {
        if (line.empty()) {
            lineas.push_back(line);
            continue;
        }
        std::stringstream ss(line);
        int idReg = 0, idUsr = 0, idVuelo = 0, asiento = 0;
        std::string status;
        if (!(ss >> idReg >> idUsr >> idVuelo >> asiento >> status)) {
            // línea mal formada: dejarla como está
            lineas.push_back(line);
            continue;
        }
        if (idReg == idReserva && idUsr == idUsuario) {
            // actualizar estado solo si estaba NoPagado (opcional)
            status = "Pagado";
            std::stringstream nuevo;
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

    std::ofstream out("Reservas.txt", std::ios::trunc);
    if (!out.is_open()) return false;

    for (const auto& l : lineas) {
        out << l << "\n";
    }
    out.close();
    return true;
}

void Pago::MostrarMenuPago(int idReserva, std::shared_ptr<Usuario> usuario) {
    if (!usuario) {
        std::cout << "Usuario no válido.\n";
        return;
    }

    // validar que la reserva pertenece al usuario y que está "NoPagado"
    {
        std::ifstream in("Reservas.txt");
        if (!in.is_open()) {
            std::cout << "No existe Reservas.txt\n";
            return;
        }
        bool encontrada = false;
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            int idReg, idUsr, idVuelo, asiento;
            std::string status;
            if (!(ss >> idReg >> idUsr >> idVuelo >> asiento >> status)) continue;
            if (idReg == idReserva && idUsr == usuario->getId()) {
                encontrada = true;
                if (status == "Pagado") {
                    std::cout << "La reserva ya está pagada.\n";
                    return;
                }
                break;
            }
        }
        if (!encontrada) {
            std::cout << "No se encontró la reserva o no pertenece al usuario.\n";
            return;
        }
    }

    // Pedir numero de tarjeta (16 dígitos)
    std::string tarjeta;
    while (true) {
        std::cout << "Ingrese numero de tarjeta (16 digitos): ";
        std::cin >> tarjeta;
        if (tarjeta.size() != 16) {
            std::cout << "La tarjeta debe tener exactamente 16 digitos.\n";
            continue;
        }
        bool soloNums = true;
        for (char c : tarjeta) if (!std::isdigit((unsigned char)c)) { soloNums = false; break; }
        if (!soloNums) {
            std::cout << "Ingrese solo numeros.\n";
            continue;
        }
        break;
    }

    // Enviar al server local (localhost)
    int bytesEnviados = 0;
    int bytesRecibidos = 0;
    std::cout << "Conectando y enviando tarjeta al servidor local...\n";

    bool ok = EnviarTarjetaAlServer("localhost", tarjeta, bytesEnviados, bytesRecibidos);
    if (!ok) {
        std::cout << "Error al enviar los datos al servidor.\n";
        return;
    }

    std::cout << "Bytes enviados: " << bytesEnviados << "\n";
    std::cout << "Bytes recibidos: " << bytesRecibidos << "\n";

    // Si el envío fue exitoso, marcamos la reserva como pagada
    int idUsuario = usuario->getId();
    bool marcado = MarcarReservaComoPagada(idReserva, idUsuario);
    if (marcado) {
        std::cout << "Tu reserva fue pagada\n";
    }
    else {
        std::cout << "No se encontró la reserva para marcar como pagada o no pertenece al usuario.\n";
    }

    // Pausa para que el usuario vea el resultado
    std::cout << "Presiona Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
