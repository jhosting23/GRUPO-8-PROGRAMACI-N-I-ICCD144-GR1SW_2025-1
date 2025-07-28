// vehiculos.h - Librería para registro de vehículos
// Creada por: mathias,jhostin, christian
// Fecha: 2025
#ifndef VEHICULOS_H
#define VEHICULOS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para funciones como system()

// Constantes basicas
#define ARCHIVO_VEHICULOS "vehiculos.txt"
#define MAX_LINEA2 250
#include "matricula.h" 

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
// Implementacion de una nueva funcion Para obtener los datos de un vehículo del archivo y llenarlos en una estructura DatosVehiculo
// Necesaria para que matricula.c pueda acceder a los datos del archivo
int obtener_datos_vehiculo_para_calculo_desde_archivo(const char* placa, DatosVehiculo* vehiculo_data);

#endif
