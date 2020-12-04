#ifndef SALATATETI_H
#define SALATATETI_H
#pragma once
#include "Common.h"

#define NOJUEGA -25
#define JUEGAX	-30
#define JUEGAO	-31
#define OCUPADO -20
#define EMPATE	-10

class SalaTaTeTi
{
private:
	std::string _nombreSala;
	std::string _tabla;
	char _espacios[3][3];
	int _IDjugadorX;
	int _IDjugadorO;
	bool _enProgreso;
	bool _turnoX;
	bool _terminado;
	bool _xRejuega;
	bool _oRejuega;
public:
	SalaTaTeTi(std::string nombresala);
	void LimpiarEspacios();
	bool Reiniciar();
	int MovimientoJugador(const int& id, const char& posicion);
	int CheckGanador();
	std::string GetTabla();
	std::string GetNombre() { return _nombreSala; }
	void SetIDX(const int& id) { _IDjugadorX = id; }
	void SetIDO(const int& id) { _IDjugadorO = id; }
	int GetIDX() { return _IDjugadorX; }
	int GetIDO() { return _IDjugadorO; }
	bool EnProgreso() { return _enProgreso; }
	void SetProgreso(const bool& progreso) { _enProgreso = progreso; }
	bool PartidaTerminada() { return _terminado; }
	void SetSiTermado(const bool& terminado) { _terminado = terminado; }
	bool TurnoX() { return _turnoX; }
	void SetTurnoX(const bool& turno) { _turnoX = turno; }
	bool XRejuega() { return _xRejuega; }
	void SetXRejuega(const bool& rejuega) { _xRejuega = rejuega; }
	bool ORejuega() { return _oRejuega; }
	void SetORejuega(const bool& rejuega) { _oRejuega = rejuega; };
};
#endif