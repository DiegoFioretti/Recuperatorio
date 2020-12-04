#include "SalaTaTeTi.h"

SalaTaTeTi::SalaTaTeTi(std::string nombresala)
{
	_nombreSala = nombresala;
	_enProgreso = false;
	_terminado = false;
	srand(time(NULL));
	int x = rand() % 2;
	if (x == 1) { _turnoX = true; }
	else { _turnoX = false; }
	_xRejuega = false;
	_oRejuega = false;
}

void SalaTaTeTi::LimpiarEspacios()
{
	_espacios[0][0] = '1';
	_espacios[0][1] = '2';
	_espacios[0][2] = '3';
	_espacios[1][0] = '4';
	_espacios[1][1] = '5';
	_espacios[1][2] = '6';
	_espacios[2][0] = '7';
	_espacios[2][1] = '8';
	_espacios[2][2] = '9';
}

bool SalaTaTeTi::Reiniciar()
{
	srand(time(NULL));
	int i = rand() % 2;
	if (_terminado == true && _xRejuega == true && _oRejuega == true) {
		LimpiarEspacios();
		_xRejuega = false;
		_oRejuega = false;
		_terminado = false;
		if (i = 1) { _turnoX = true; }
		else { _turnoX = false; }
		return true;
	}
	return false;
}

int SalaTaTeTi::MovimientoJugador(const int& id, const char& posicion)
{
	// Al mandar un comando de posicion se devuelve un int K que dice que ocurrio
	// K > 0 = Un jugador gano, se manda su id
	// K == 0 = No hubo cambios significativos en la partida, la partida sigue normal y se cambia de turno
	//	K == JUEGAX (-30) = Le toca a X jugar
	//	K == JUEGOO (-31) = Le toca a O jugar
	// K < 0 = Es un empate
	// K == NOJUEGA(-25) = El jugador hizo una jugada que no le corresponde y se le tiene que avisar

	if (id == _IDjugadorX && _turnoX == false)
	{
		return NOJUEGA;
	}
	else if (id == _IDjugadorO && _turnoX == true)
	{
		return NOJUEGA;
	}

	int k = 0;

	switch (posicion) {
	case '1':
		if (_espacios[0][0] == '1') {
			if (id == _IDjugadorX)
				_espacios[0][0] = 'X';
			else if (id == _IDjugadorO)
				_espacios[0][0] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;

	case '2':
		if (_espacios[0][1] == '2') {
			if (id == _IDjugadorX)
				_espacios[0][1] = 'X';
			else if (id == _IDjugadorO)
				_espacios[0][1] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '3':
		if (_espacios[0][2] == '3') {
			if (id == _IDjugadorX)
				_espacios[0][2] = 'X';
			else if (id == _IDjugadorO)
				_espacios[0][2] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '4':
		if (_espacios[1][0] == '4') {
			if (id == _IDjugadorX)
				_espacios[1][0] = 'X';
			else if (id == _IDjugadorO)
				_espacios[1][0] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '5':
		if (_espacios[1][1] == '5') {
			if (id == _IDjugadorX)
				_espacios[1][1] = 'X';
			else if (id == _IDjugadorO)
				_espacios[1][1] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '6':
		if (_espacios[1][2] == '6') {
			if (id == _IDjugadorX)
				_espacios[1][2] = 'X';
			else if (id == _IDjugadorO)
				_espacios[1][2] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '7':
		if (_espacios[2][0] == '7') {
			if (id == _IDjugadorX)
				_espacios[2][0] = 'X';
			else if (id == _IDjugadorO)
				_espacios[2][0] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '8':
		if (_espacios[2][1] == '8') {
			if (id == _IDjugadorX)
				_espacios[2][1] = 'X';
			else if (id == _IDjugadorO)
				_espacios[2][1] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	case '9':
		if (_espacios[2][2] == '9') {
			if (id == _IDjugadorX)
				_espacios[2][2] = 'X';
			else if (id == _IDjugadorO)
				_espacios[2][2] = 'O';
		}
		else {
			k = OCUPADO;
		}
		break;
	}

	if (k == OCUPADO)
	{
		return k;
	}

	k = CheckGanador();
	if (k == 0) // Nada cambia
	{
		if (_turnoX && _terminado == false) {
			_turnoX = false;
			k = JUEGAO;
		}
		else if (!_turnoX && _terminado == false) {
			_turnoX = true;
			k = JUEGAX;
		}
	}

	return k;
}

int SalaTaTeTi::CheckGanador()
{
	// Cualquiera de estos casos implica que hay un ganador
	if (_espacios[0][0] == 'X' && _espacios[0][1] == 'X' && _espacios[0][2] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[1][0] == 'X' && _espacios[1][1] == 'X' && _espacios[1][2] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[2][0] == 'X' && _espacios[2][1] == 'X' && _espacios[2][2] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[0][0] == 'X' && _espacios[1][0] == 'X' && _espacios[2][0] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[0][1] == 'X' && _espacios[1][1] == 'X' && _espacios[2][1] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[0][2] == 'X' && _espacios[1][2] == 'X' && _espacios[2][2] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[0][0] == 'X' && _espacios[1][1] == 'X' && _espacios[2][2] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[0][2] == 'X' && _espacios[1][1] == 'X' && _espacios[2][0] == 'X') {
		_terminado = true;
		return _IDjugadorX;
	}
	else if (_espacios[0][0] == 'O' && _espacios[0][1] == 'O' && _espacios[0][2] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[1][0] == 'O' && _espacios[1][1] == 'O' && _espacios[1][2] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[2][0] == 'O' && _espacios[2][1] == 'O' && _espacios[2][2] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[0][0] == 'O' && _espacios[1][0] == 'O' && _espacios[2][0] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[0][1] == 'O' && _espacios[1][1] == 'O' && _espacios[2][1] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[0][2] == 'O' && _espacios[1][2] == 'O' && _espacios[2][2] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[0][0] == 'O' && _espacios[1][1] == 'O' && _espacios[2][2] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	else if (_espacios[0][2] == 'O' && _espacios[1][1] == 'O' && _espacios[2][0] == 'O') {
		_terminado = true;
		return _IDjugadorO;
	}
	// Empate
	else if (_espacios[0][0] != '1' && _espacios[0][1] != '2' && _espacios[0][2] != '3' && _espacios[1][0] != '4' && _espacios[1][1] != '5' && _espacios[1][2] != '6' && _espacios[2][0] != '7' && _espacios[2][1] != '8' && _espacios[2][2] != '9') {
		_terminado = true;
		return -1;
	}
	// No cambio nada
	return 0;
}

std::string SalaTaTeTi::GetTabla()
{
	_tabla.clear();
	_tabla.push_back(_espacios[0][0]);
	_tabla += " | ";
	_tabla.push_back(_espacios[0][1]);
	_tabla += " | ";
	_tabla.push_back(_espacios[0][2]);
	_tabla += " ";
	_tabla.push_back('\n');
	_tabla += "----------\n";
	_tabla.push_back(_espacios[1][0]);
	_tabla += " | ";
	_tabla.push_back(_espacios[1][1]);
	_tabla += " | ";
	_tabla.push_back(_espacios[1][2]);
	_tabla += " ";
	_tabla.push_back('\n');
	_tabla += "----------\n";
	_tabla.push_back(_espacios[2][0]);
	_tabla += " | ";
	_tabla.push_back(_espacios[2][1]);
	_tabla += " | ";
	_tabla.push_back(_espacios[2][2]);
	_tabla += " ";
	_tabla.push_back('\n');

	return _tabla;
}