#ifndef JUGADOR_H
#define JUGADOR_H
#pragma once
#include "Common.h"

enum EstadoJugador {
	noJuega,
	circulo,
	cruz = 3
};

class Jugador
{
private:
	std::string _nombre;
	std::string _tabla;
	int _IDusuario = -1;
	struct sockaddr_in stuff;
	IN_ADDR _IPusuario;
	USHORT _PUERTOusuario;
	std::string _sala;
	int _estadoJuego;
public:
	Jugador(const struct sockaddr_in& loginip, const std::string& nuevonombre, int ID);
	std::string GetNombre();
	int GetID();
	IN_ADDR GetIP();
	USHORT GetPuerto();
	std::string GetSala();
	void CambiarSala(const std::string& nuevaSala);
	int GetEstado();
	void SetEstado(const int& nuevoEstado);
};
#endif