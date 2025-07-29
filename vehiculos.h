/*
 * vehiculos.h - Libreria para registro y validacion de vehiculos
 * 
 * Descripcion: Este archivo contiene las estructuras, constantes y 
 *              prototipos de funciones para el manejo de vehiculos
 *              en el sistema de matriculacion vehicular.
 * 
 * Autores: Mathias, Jhostin, Christian
 * Fecha: 2025
 * Materia: Programacion I - ICCD144
 * Universidad: escuela politecnica nacional
 */

#ifndef VEHICULOS_H
#define VEHICULOS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>    // Para toupper, isupper, isdigit

// ===================================================================
// CONSTANTES DEL SISTEMA
// ===================================================================

// Configuracion de archivos
#define ARCHIVO_VEHICULOS "vehiculos.txt"    // Archivo de almacenamiento
#define MAX_LINEA2 250                       // Longitud maxima de linea

// Limites de avaluo vehicular
#define MIN_AVALUO 500.00                    // Avaluo minimo permitido
#define MAX_AVALUO 250000.00                 // Avaluo maximo permitido

// Incluir libreria de matricula para estructuras compartidas
#include "matricula.h"

// ===================================================================
// CONSTANTES PARA REVISIONES TECNICAS SIMPLIFICADAS
// ===================================================================
#define ARCHIVO_REVISIONES "revisiones.txt"

// ===================================================================
// ESTRUCTURAS SIMPLIFICADAS PARA REVISIONES TECNICAS
// ===================================================================

/*
 * Estructura: RevisionTecnicaSimple
 * Descripcion: Almacena informacion basica de revision tecnica
 */
typedef struct {
    char placa[10];                    // Placa del vehiculo
    char fecha_revision[20];           // Fecha de la revision (DD/MM/AAAA)
    int aprobada;                      // 1 = Aprobada, 0 = No aprobada
    char observaciones[100];           // Observaciones breves
} RevisionTecnicaSimple;

// ===================================================================
// PROTOTIPOS DE FUNCIONES PRINCIPALES
// ===================================================================
/*
 * Funciones principales para gestion de vehiculos
 */
int registrar_vehiculo(void);    // Registra un nuevo vehiculo en el sistema
int buscar_vehiculo(void);       // Busca un vehiculo por placa

// ===================================================================
// PROTOTIPOS DE FUNCIONES DE VALIDACION
// ===================================================================

/*
 * Funciones para validar datos de vehiculos
 */
int validar_placa(const char* placa);         // Valida formato de placa
int validar_cedula(const char* cedula);       // Valida cedula ecuatoriana
int validar_cilindraje(int cilindraje);       // Valida rango de cilindraje
int validar_nombre(const char* nombre);       // Valida nombre del propietario
int validar_valor(float valor);               // Valida rango de avaluo

// ===================================================================
// PROTOTIPOS DE FUNCIONES AUXILIARES
// ===================================================================

/*
 * Funciones auxiliares del sistema
 */
int vehiculo_ya_existe(const char* placa);    // Verifica si placa existe
void limpiar_pantalla(void);                  // Limpia pantalla multiplataforma
void convertir_a_mayusculas(char* cadena);    // Convierte texto a mayusculas

/*
 * Funcion para integracion con sistema de calculos
 */
int obtener_datos_vehiculo_para_calculo_desde_archivo(const char* placa, DatosVehiculo* vehiculo_data);

// Funciones de consulta y reportes
void mostrar_vehiculos_matriculados();
void mostrar_reporte_detallado_vehiculos();

// ===================================================================
// PROTOTIPOS DE FUNCIONES PARA REVISIONES TECNICAS SIMPLIFICADAS
// ===================================================================

/*
 * Funciones principales para manejo de revisiones tecnicas
 */
int registrar_revision_simple();                               // Registra revision simple
int vehiculo_tiene_revision(const char* placa);                // Verifica si tiene revision
int consultar_revision_vehiculo();                             // Consulta revision por placa

/*
 * Funcion principal del proceso de matriculacion
 */
void proceso_matriculacion();                                  // Menu principal integrado
void proceso_matriculacion_final();                           // Matriculacion final del vehiculo

#endif // VEHICULOS_H

