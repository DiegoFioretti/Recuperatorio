#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <list>
#include <vector>
#include <random>
#include <ctime>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

enum EstadoJugador {
	noJuega,
	circulo,
	cruz = 3
};