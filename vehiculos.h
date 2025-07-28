// vehiculos.h - Librería para registro de vehículos
// Creada por: mathias,jhostin, christian
// Fecha: 2025
#ifndef VEHICULOS_H
#define VEHICULOS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constantes básicas
#define ARCHIVO_VEHICULOS "vehiculos.txt"
#define MAX_LINEA2 250

// Funciones principales que vamos a usar
int registrar_vehiculo();
int buscar_vehiculo();

// Funciones para validar datos
int validar_placa(char* placa);
int validar_cedula(char* cedula);
int validar_cilindraje(int cilindraje);

// Funciones auxiliares
int vehiculo_ya_existe(char* placa);
void limpiar_entrada();

#endif
