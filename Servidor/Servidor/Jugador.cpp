#include "Jugador.h"

Jugador::Jugador(const struct sockaddr_in& loginip, const std::string& nuevonombre, int ID) {
	_nombre = nuevonombre;
	_IDusuario = ID;
	_IPusuario = loginip.sin_addr;
	_PUERTOusuario = loginip.sin_port;
	_sala = "general";
	_estadoJuego = noJuega;
}

std::string Jugador::GetNombre()
{
	return _nombre;
}
int Jugador::GetID() 
{
	return _IDusuario;
}
IN_ADDR Jugador::GetIP() 
{
	return _IPusuario;
}
USHORT Jugador::GetPuerto() 
{
	return _PUERTOusuario;
}
std::string Jugador::GetSala() 
{
	return _sala;
}
void Jugador::CambiarSala(const std::string& nuevaSala) 
{
	_sala = nuevaSala;
}
int Jugador::GetEstado() 
{
	return _estadoJuego;
}
void Jugador::SetEstado(const int& nuevoEstado)
{
	_estadoJuego = nuevoEstado;
}