// vehiculos.h - Librera para registro de vehculos
// Creada por: Mathias, Jhostin, Christian
// Fecha: 2025
#ifndef VEHICULOS_H
#define VEHICULOS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // Para toupper, isupper, isdigit

// Constantes bsicas
#define ARCHIVO_VEHICULOS "vehiculos.txt"
#define MAX_LINEA2 250

// CAMBIO: Definir constantes para el rango de aval?o del veh?culo
#define MIN_AVALUO 500.00
#define MAX_AVALUO 250000.00


#include "matricula.h" 

/**
* Funciones principales para gestin de vehculos
*/
int registrar_vehiculo(void);
int buscar_vehiculo(void);

/**
* Funciones para validar datos de vehculos
*/
int validar_placa(const char* placa);
int validar_cedula(const char* cedula);
int validar_cilindraje(int cilindraje);
int validar_nombre(const char* nombre); 
int validar_valor(float valor);         
/**
* Funciones auxiliares
*/
int vehiculo_ya_existe(const char* placa);
void limpiar_pantalla(void);
void convertir_a_mayusculas(char* cadena);

/**
* Obtiene los datos de un vehculo del archivo y los llena en una estructura DatosVehiculo.
*/
int obtener_datos_vehiculo_para_calculo_desde_archivo(const char* placa, DatosVehiculo* vehiculo_data);

#endif // VEHICULOS_H

