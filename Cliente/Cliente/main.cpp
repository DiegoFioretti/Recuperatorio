#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <conio.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")	// Libreria de Winsock
#define SERVER "127.0.0.1"			// Direccion IP por defecto del servidor
#define BUFLON 512					// Longitud del buffer de mensaje
#define PUERTO 8888					// Puerto por defecto por el cual se reciben mensajes
#define NOMLON 12					// Longitud del nombre del jugador

std::string nombreJugador;
bool firstInput = true;

int main(void)
{
	struct sockaddr_in socketServ;
	int s, slen = sizeof(socketServ);
	char buffer[BUFLON];
	char mensaje[BUFLON];
	WSADATA wsaData;

	// Iniciar Winsock
	printf("\nIniciando Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Inicio fallido. Codigo de error : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Winsock inicializado.\n");

	// Crear Socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("Creacion de Socket fallida. Codigo de error : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Configuracion de estructura de socket a conectar
	memset((char*)&socketServ, 0, sizeof(socketServ));
	socketServ.sin_family = AF_INET;
	socketServ.sin_port = htons(PUERTO);
	socketServ.sin_addr.S_un.S_addr = inet_addr(SERVER);

	// Loop de comunicacion
	int point = 0;
	memset(mensaje, '\0', BUFLON);
	std::cout << "Bienvenido al TaTeTi World!" << std::endl;
	do
	{
		std::cout << "Por favor introduzca su nombre sin exceder el limite de 12 caracteres o que este vacio: ";
		std::cin >> nombreJugador;
		std::cout << std::endl;
	} while (nombreJugador.length() > NOMLON || nombreJugador.length() <= 0);

	FD_SET fds;
	struct timeval timeVal;
	int auxSelect; 
	char input;
	int aux;
	while (1) {
		if (firstInput)
		{
			mensaje[0] = '#';
			mensaje[1] = 'l';
			mensaje[2] = 'o';
			mensaje[3] = 'g';
			mensaje[4] = 'i';
			mensaje[5] = 'n';
			mensaje[6] = ' ';
			aux = 7;
			for (size_t i = 0; i < nombreJugador.length(); i++)
			{
				mensaje[aux] = nombreJugador[i];
				aux++;
			}
			if (sendto(s, mensaje, strlen(mensaje), 0, (struct sockaddr*)&socketServ, slen) == SOCKET_ERROR)
			{
				printf("Error de sendto(), codigo de error : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			memset(mensaje, '\0', BUFLON);
			firstInput = false;
		}
		while (_kbhit()) {
			input = _getch();
			if (input == 13) {
				if (sendto(s, mensaje, strlen(mensaje), 0, (struct sockaddr*)&socketServ, slen) == SOCKET_ERROR)
				{
					printf("Error de sendto(), codigo de error : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				memset(mensaje, '\0', BUFLON);
				point = 0;
				std::cout << std::endl;
			}
			else if (input == 8) {
				point--;
				if (point < 0)
					point = 0;
				mensaje[point] = '\0';

				std::cout << '\r';
				std::string mess2 = "Ingrese mensaje : ";
				for (int i = 0; i < mess2.length(); i++)
				{
					std::cout << ' ';
				}
				for (int i = 0; i < BUFLON; i++)
				{
					if (mensaje[i] != '\0')
						std::cout << ' ';
					else
						i = BUFLON;
				}
				std::cout << ' ';
				std::cout << '\r' << "Ingrese mensaje : " << mensaje;
			}
			else {
				if (point > BUFLON - 2)
					point = BUFLON - 2;
				mensaje[point] = input;
				point++;
			}
		}

		// Recibe una respuesta del servidor y la escribe en consola
		// Se limpia el buffer llenandolo con valores null ya que podría tener datos de la conexion anterior

		FD_ZERO(&fds);
		FD_SET(s, &fds);

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 30000;

		auxSelect = select(s, &fds, NULL, NULL, &timeVal);
		memset(buffer, '\0', BUFLON);

		// Se intenta recibir una respuesta, esta es una llamada bloqueante
		if (auxSelect > 0) {
			memset(buffer, '\0', BUFLON);
			if (recvfrom(s, buffer, BUFLON, 0, (struct sockaddr*)&socketServ, &slen) == SOCKET_ERROR) {
				printf("Error de recvfrom(), codigo de error : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			puts(buffer);
		}
		else if (auxSelect == 0) {
			std::cout << "\rIngrese mensaje : " << mensaje;
		}
		else if (auxSelect < 0) {
			printf("Error");
			exit(EXIT_FAILURE);
		}
	}

	closesocket(s);
	WSACleanup();

	return 0;
}