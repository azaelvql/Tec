#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX            // evita que windows.h defina macros min/max que rompen std::numeric_limits

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <memory>
#include "Usuario.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Pago {
public:
    void MostrarMenuPago(int idReserva, std::shared_ptr<Usuario> usuario);
};
