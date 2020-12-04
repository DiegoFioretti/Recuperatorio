#include "Common.h"
#include "SalaTaTeTi.h"
#include "Jugador.h"

#define BUFLON 512		//Longitud maxima del buffer
#define PUERTO 8888		//Puerto del que se escucha
#define NOMLON 17		//Longitud maxima del nombre de usuario
#define SALASCANT 20	//Cantidad de salas de juego

int _ID = 1;

char bufferRespuesta[BUFLON];
std::vector<struct sockaddr_in> listadoSocketsUsuarios;
std::vector<SalaTaTeTi*> listadoSalasJuego;
std::vector<Jugador*> listadoJugadores;
std::vector<int> salaDeEspera;

void EscribirRespuesta(const std::string& respuesta) {
	int a = 0;
	for (size_t i = 0; i < BUFLON; i++) {
		if (bufferRespuesta[i] == '\0') {
			break;
		}
		else
			a++;
	}
	for (int j = 0; j < respuesta.length(); j++) {
		bufferRespuesta[a] = respuesta[j];
		a++;
	}
}

int GetIDusuario(const sockaddr_in& user) {
	for (size_t i = 0; i < listadoJugadores.size(); i++) {
		if (listadoJugadores[i]->GetIP().S_un.S_addr == user.sin_addr.S_un.S_addr && listadoJugadores[i]->GetPuerto() == user.sin_port) {
			return listadoJugadores[i]->GetID();
		}
	}
	return -1;
}

Jugador* GetJugador(const int& id)
{
	std::vector<Jugador*>::iterator error = (listadoJugadores.end() - 1);
	for (size_t i = 0; i < listadoJugadores.size(); i++) {
		if (listadoJugadores[i]->GetID() == id) {
			return listadoJugadores[i];
		}
	}
	return *error;
}

SalaTaTeTi* GetSaladeJugador(const int& id)
{
	std::vector<SalaTaTeTi*>::iterator error = (listadoSalasJuego.end() - 1);
	for (size_t i = 0; i < listadoSalasJuego.size(); i++) {
		if (listadoSalasJuego[i]->GetIDO() == id || listadoSalasJuego[i]->GetIDX() == id) {
			return listadoSalasJuego[i];
		}
	}
	return *error;
}

void Comandos(const std::string& _buf, const int& user) {
	//#help
	if (_buf[1] == 'h' && _buf[2] == 'e' && _buf[3] == 'l' && _buf[4] == 'p') {
		EscribirRespuesta("Lista de Comandos:\n #play : Te mete en la sala de espera, una vez que hayan suficientes personas empieza una partida\n");
	}
	//#play
	else if (_buf[1] == 'p' && _buf[2] == 'l' && _buf[3] == 'a' && _buf[4] == 'y') {
		salaDeEspera.push_back(user);
		if (salaDeEspera.size() == 2) {
			for (size_t i = 1; i < listadoSalasJuego.size(); i++) {
				if (!listadoSalasJuego[i]->EnProgreso()) {
					GetJugador(salaDeEspera[0])->CambiarSala(listadoSalasJuego[i]->GetNombre());
					GetJugador(salaDeEspera[1])->CambiarSala(listadoSalasJuego[i]->GetNombre());

					int random = rand() % 2 + 1;
					GetJugador(salaDeEspera[0])->SetEstado(random);

					if (listadoSalasJuego[i]->TurnoX()) {
						EscribirRespuesta("Empieza la X\n");
					}
					else {
						EscribirRespuesta("Empieza la O\n");
					}

					if (random == cruz) {
						GetJugador(salaDeEspera[1])->SetEstado(circulo);
						listadoSalasJuego[i]->SetIDX(salaDeEspera[0]);
						listadoSalasJuego[i]->SetIDO(salaDeEspera[1]);
						EscribirRespuesta("Iniciando una partida con " +
							GetJugador(salaDeEspera[0])->GetNombre() + " como la X y " +
							GetJugador(salaDeEspera[1])->GetNombre() + " como la O " + '\n' +
							listadoSalasJuego[i]->GetTabla() + '\n');
					}
					else {
						GetJugador(salaDeEspera[1])->SetEstado(cruz);
						listadoSalasJuego[i]->SetIDX(salaDeEspera[1]);
						listadoSalasJuego[i]->SetIDO(salaDeEspera[0]);
						EscribirRespuesta("Iniciando una partida con " +
							GetJugador(salaDeEspera[1])->GetNombre() + " como la  X y " +
							GetJugador(salaDeEspera[0])->GetNombre() + " como la O " + '\n' +
							listadoSalasJuego[i]->GetTabla() + '\n');
					}
					listadoSalasJuego[i]->SetProgreso(true);
					salaDeEspera.clear();
					break;
				}
			}
		}
		else { EscribirRespuesta(GetJugador(user)->GetNombre() + " ha entrado en la sala de espera en busca de una partida\n"); }
	}
	else {
		printf("Data: Comando Invalido\n");
		EscribirRespuesta("Perdon, pero ese comando no existe. Intenta usar #help para ver los comandos existentes\n");
	}
}

int resultadoInput;
void ComandosJuego(const std::string& _buf, const int& user) {
	//#help
	if (_buf[1] == 'h' && _buf[2] == 'e' && _buf[3] == 'l' && _buf[4] == 'p') {
		EscribirRespuesta("Lista de comandos:\n#quit : Salis de tu sala de juego actual\n#surrender : Rindete en tu partida actual\n#replay : Una ves que termine la partida, te permite aceptar rejugar la partida\n#1-9 : Selecciona el numero de la casilla donde quieres poner tu pieza\n");
	}
	//#quit
	else if (_buf[1] == 'q' && _buf[2] == 'u' && _buf[3] == 'i' && _buf[4] == 't') {

		if (user == GetSaladeJugador(user)->GetIDX())
		{
			EscribirRespuesta("Jugador X " + GetJugador(user)->GetNombre() + " ha dejado la sala de juego\n");
			GetSaladeJugador(user)->SetIDX(-1);
			GetJugador(user)->CambiarSala("general");
		}
		else
		{
			EscribirRespuesta("Jugador O " + GetJugador(user)->GetNombre() + " ha dejado la sala de juego\n");
			GetSaladeJugador(user)->SetIDO(-1);
			GetJugador(user)->CambiarSala("general");
		}

	}
	//#surrender
	else if (_buf[1] == 's' && _buf[2] == 'u' && _buf[3] == 'r' && _buf[4] == 'r' && _buf[5] == 'e' && _buf[6] == 'n' && _buf[7] == 'd' && _buf[8] == 'e' && _buf[9] == 'r') {
		if (!GetSaladeJugador(user)->PartidaTerminada()) {
			GetSaladeJugador(user)->SetSiTermado(true);
			if (user == GetSaladeJugador(user)->GetIDX())
				EscribirRespuesta("Jugador X " + GetJugador(GetSaladeJugador(user)->GetIDX())->GetNombre() + " se ha rendido, el ganador es el jugador O " + GetJugador(GetSaladeJugador(user)->GetIDO())->GetNombre() + ". Usen #replay para empezar un nuevo juego\n");
			else if (user == GetSaladeJugador(user)->GetIDO())
				EscribirRespuesta("Jugador O " + GetJugador(GetSaladeJugador(user)->GetIDO())->GetNombre() + " se ha rendido, el ganador es el jugador X " + GetJugador(GetSaladeJugador(user)->GetIDX())->GetNombre() + ". Usen #replay para emepezar un nuevo juego\n");
		}
		else {
			EscribirRespuesta("No puedes rendirte si la partida ya termino\n");
		}
	}
	//#replay
	else if (_buf[1] == 'r' && _buf[2] == 'e' && _buf[3] == 'p' && _buf[4] == 'l' && _buf[5] == 'a' && _buf[6] == 'y') {
		if (GetSaladeJugador(user)->PartidaTerminada()) {
			if (user == GetSaladeJugador(user)->GetIDX()) {
				EscribirRespuesta("Jugador X " + GetJugador(user)->GetNombre() + " ha aceptado la revancha!\n");
				GetSaladeJugador(user)->SetXRejuega(true);
			}
			else if (user == GetSaladeJugador(user)->GetIDO()) {
				EscribirRespuesta("Jugador O " + GetJugador(user)->GetNombre() + " ha aceptado la revancha!\n");
				GetSaladeJugador(user)->SetORejuega(true);
			}
			if (GetSaladeJugador(user)->Reiniciar()) {
				if (GetSaladeJugador(user)->TurnoX()) { EscribirRespuesta("Una nueva partida ha empezado!\n" + GetSaladeJugador(user)->GetTabla() + "\nJugador X, es tu turno\n"); }
				else { EscribirRespuesta("A new match has started!\n" + GetSaladeJugador(user)->GetTabla() + "\nJugador O, es tu turno\n"); }

			}
		}
		else {
			EscribirRespuesta("No hay necesidad de ir a la revancha, la partida no termino todavia\n");
		}
	}
	//#"Number of placing"
	else if (_buf[1] == '1' || _buf[1] == '2' || _buf[1] == '3' || _buf[1] == '4' || _buf[1] == '5' || _buf[1] == '6' || _buf[1] == '7' || _buf[1] == '8' || _buf[1] == '9') {
		if (!GetSaladeJugador(user)->PartidaTerminada()) {
			resultadoInput = GetSaladeJugador(user)->MovimientoJugador(user, _buf[1]);

			if (resultadoInput > 0)
			{
				EscribirRespuesta(GetSaladeJugador(user)->GetTabla() + "\nJUEGO TERMINADO! El ganador de la partida es " + GetJugador(resultadoInput)->GetNombre() + "\n" + "usen #replay para volver a jugar");
			}
			else if (resultadoInput == NOJUEGA)
			{
				EscribirRespuesta("No es tu turno de jugar " + GetJugador(user)->GetNombre() + '\n');
			}
			else if (resultadoInput == OCUPADO)
			{
				EscribirRespuesta("Ese espacio esta ocupado " + GetJugador(user)->GetNombre() + ", intenta otro\n");
			}
			else if (resultadoInput == JUEGAX && !GetSaladeJugador(user)->PartidaTerminada())
			{
				GetSaladeJugador(user)->SetTurnoX(true);
				EscribirRespuesta('\n' + GetSaladeJugador(user)->GetTabla() + "\nAhora es el turno del jugador X\n");
			}
			else if (resultadoInput == JUEGAO && !GetSaladeJugador(user)->PartidaTerminada())
			{
				GetSaladeJugador(user)->SetTurnoX(false);
				EscribirRespuesta('\n' + GetSaladeJugador(user)->GetTabla() + "\nAhora es el turno del jugador O\n");
			}
		}
		else {
			EscribirRespuesta("No puedes hacer una jugada si la partida no esta en progreso\n");
		}
	}
	else {
		printf("Data: Comando Invalido\n");
		EscribirRespuesta("Perdon, pero el comando ingresado no existe. Intenta usar #help para ver los comandos existentes\n");
	}
}

int main() {
	SOCKET _socket;
	struct sockaddr_in serverDir, clientDir;
	int _clientLongitud, receptor;
	char bufEntrada[BUFLON];
	WSADATA wsa;

	_clientLongitud = sizeof(clientDir);

	std::cout << "Iniciando Winsock... ";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "Fallo de Inicio. Error : " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listo!" << std::endl;

	std::cout << "Creando Socket... ";
	if ((_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		std::cout << "No se pudo crear socket. Error : " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listo!" << std::endl;

	// Preparar la estructura del sockaddr_in
	serverDir.sin_family = AF_INET;
	serverDir.sin_addr.s_addr = INADDR_ANY;
	serverDir.sin_port = htons(PUERTO);

	std::cout << "Enlazando socket... ";
	if (bind(_socket, (struct sockaddr*)&serverDir, sizeof(serverDir)) == SOCKET_ERROR) {
		std::cout << "Enlaze fallido. Error : " << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listo!" << std::endl;

	// Crear las salas de juego
	std::string name;
	for (size_t i = 0; i < SALASCANT; i++) {
		name = "GameRoom" + std::to_string(i);

		listadoSalasJuego.push_back(new SalaTaTeTi(name));
	}
	for (size_t i = 0; i < SALASCANT; i++) {
		listadoSalasJuego[i]->LimpiarEspacios();
	}

	FD_SET fds;
	struct timeval timeVal;
	int selector;
	int idManejada;
	// Mantener un loop para que el servidor reciba llamadas
	while (true) {
		FD_ZERO(&fds);
		FD_SET(_socket, &fds);

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 30000;

		selector = select(_socket, &fds, NULL, NULL, &timeVal);
		if (selector > 0) {
			if ((receptor = recvfrom(_socket, bufEntrada, BUFLON, 0, (struct sockaddr*)&clientDir, &_clientLongitud)) == SOCKET_ERROR) {
				std::cout << "recvfrom() fallo. Error : " << WSAGetLastError() << std::endl;;
				std::exit(EXIT_FAILURE);
			}

			std::cout << "Paquete recibido desde - ";
			std::cout << "IP: " << clientDir.sin_addr.S_un.S_addr << " ";
			std::cout << "Puerto: " << ntohs(clientDir.sin_port) << std::endl;
			idManejada = GetIDusuario(clientDir);

			if (idManejada >= 0)
				std::cout << "Room: " << GetJugador(idManejada)->GetSala() << std::endl;
			std::cout << std::endl;

			if (idManejada >= 0) {
				if (bufEntrada[0] == '#') {
					if (GetJugador(idManejada)->GetEstado() > noJuega)
						ComandosJuego(bufEntrada, idManejada);
					else
						Comandos(bufEntrada, idManejada);
				}
				else {
					std::cout << GetJugador(idManejada)->GetNombre() << ": " << bufEntrada << std::endl;
					EscribirRespuesta(GetJugador(idManejada)->GetNombre() + ": " + bufEntrada);
				}
			}
			else {
				if (bufEntrada[0] == '#' && bufEntrada[1] == 'l' && bufEntrada[2] == 'o' && bufEntrada[3] == 'g' && bufEntrada[4] == 'i' && bufEntrada[5] == 'n' && bufEntrada[6] == ' ') {

					std::string strName;
					for (size_t i = 7; i < BUFLON; i++) {
						if (bufEntrada[i] != '\0')
							strName.push_back(bufEntrada[i]);
						else
							break;
					}

					listadoSocketsUsuarios.push_back(clientDir);
					listadoJugadores.push_back(new Jugador(clientDir, strName, _ID));
					_ID++;
					EscribirRespuesta("Bienvenido jugador " + strName + '\n');

				}
				else {
					std::cout << "Un usuario intento entrar sin estar loggeado" << std::endl;
					EscribirRespuesta("Usuario no registrado, use #login + el nombre que quiera usar\n");
					if (sendto(_socket, bufferRespuesta, BUFLON, 0, (struct sockaddr*)&clientDir, _clientLongitud) == SOCKET_ERROR) {
						std::cout << "sendto() fallo. Error : " << WSAGetLastError() << std::endl;;
						std::exit(EXIT_FAILURE);
					}
				}
			}
			// Responder a los jugadores registrados en sus salas respectivas
			for (size_t i = 0; i < listadoJugadores.size(); i++) {
				if (GetJugador(idManejada)->GetSala() == listadoJugadores[i]->GetSala()) {
					if (sendto(_socket, bufferRespuesta, BUFLON, 0, (struct sockaddr*)&listadoSocketsUsuarios[i], _clientLongitud) == SOCKET_ERROR) {
						std::cout << "sendto() fallo. Error : " << WSAGetLastError() << std::endl;
						std::exit(EXIT_FAILURE);
					}
				}
			}
		}
		else if (selector < 0) {
			std::cout << "ERROR DEL SELECTOR" << std::endl;;
		}
		// Limpiar los buffers
		memset(bufEntrada, '\0', BUFLON);
		memset(bufferRespuesta, '\0', BUFLON);
		fflush(stdin);
	}

	closesocket(_socket);
	WSACleanup();

	for (size_t i = 0; i < listadoSalasJuego.size(); i++)
	{
		delete listadoSalasJuego[i];
	}
	for (size_t i = 0; i < listadoJugadores.size(); i++)
	{
		delete listadoJugadores[i];
	}

	return 0;
}