/*
 * pagos.h - Libreria para el procesamiento de pagos de matricula
 * 
 * Descripcion: Este archivo contiene las estructuras, constantes y 
 *              prototipos de funciones para el manejo de pagos de
 *              matricula vehicular en el sistema.
 * 
 * Autores: Mathias, Jhostin, Christian
 * Fecha: 2025
 * Materia: Programacion I - ICCD144
 * Universidad: Escuela Politecnica Nacional
 */

#ifndef PAGOS_H
#define PAGOS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "matricula.h"

// Declaracion de funciones externas necesarias
int validar_cedula(const char* cedula);

// ===================================================================
// CONSTANTES DEL SISTEMA DE PAGOS
// ===================================================================

// Configuracion de archivos y carpetas
#define CARPETA_COMPROBANTES "comprobantes"
#define CARPETA_PAGOS "pagos"
#define ARCHIVO_COMPROBANTES "comprobantes/comprobantes.txt"
#define ARCHIVO_PAGOS "pagos/pagos.txt"
#define MAX_COMPROBANTE 50

// Estados de comprobante
#define ESTADO_PENDIENTE 0
#define ESTADO_PAGADO 1
#define ESTADO_VENCIDO 2

// Tipos de pago
#define TIPO_EFECTIVO 1
#define TIPO_TARJETA 2
#define TIPO_TRANSFERENCIA 3

// Validez del comprobante en dias
#define DIAS_VALIDEZ_COMPROBANTE 30

// ===================================================================
// ESTRUCTURAS DE DATOS
// ===================================================================

/*
 * Estructura: ComprobanteMatricula
 * Descripcion: Almacena la informacion de un comprobante de matricula
 *              generado por el sistema
 */
typedef struct {
    char numero_comprobante[50];      // Numero unico del comprobante
    char placa[10];                   // Placa del vehiculo
    char fecha_emision[20];           // Fecha de emision del comprobante
    char fecha_vencimiento[20];       // Fecha de vencimiento para pago
    double monto_total;               // Monto total a pagar
    int estado;                       // Estado del comprobante (0=pendiente, 1=pagado, 2=vencido)
    DatosVehiculo vehiculo;           // Datos del vehiculo
    ResultadoMatricula resultado;     // Resultado del calculo de matricula
} ComprobanteMatricula;

/*
 * Estructura: RegistroPago
 * Descripcion: Almacena la informacion de un pago realizado
 */
typedef struct {
    char numero_comprobante[50];      // Numero del comprobante pagado
    char placa[10];                   // Placa del vehiculo
    char fecha_pago[20];              // Fecha y hora del pago
    double monto_pagado;              // Monto pagado
    int tipo_pago;                    // Tipo de pago (1=efectivo, 2=tarjeta, 3=transferencia)
    char referencia_pago[50];         // Referencia del pago (numero de tarjeta, transaccion, etc.)
    char cedula_pagador[15];          // Cedula de quien realiza el pago
    char nombre_pagador[100];         // Nombre de quien realiza el pago
} RegistroPago;

// ===================================================================
// PROTOTIPOS DE FUNCIONES
// ===================================================================

// Funciones principales del modulo de pagos
void menu_pagos();
int procesar_pago_por_placa();

// Funciones de validacion
int comprobante_vigente(const char* fecha_vencimiento);

// Funciones de archivos
int guardar_comprobante_sistema(const char* placa, ResultadoMatricula resultado, DatosVehiculo vehiculo, const char* numero_comprobante);
int guardar_registro_pago(RegistroPago pago);
int actualizar_estado_comprobante(const char* numero_comprobante, int nuevo_estado);
int obtener_datos_propietario(const char* placa, char* cedula, char* nombre);

// Funciones de generacion de comprobantes de pago
// (Funciones removidas para simplificar el sistema)

// Funciones de consulta
int consultar_estado_por_placa();

// Funciones auxiliares
void obtener_fecha_actual(char* fecha);
void calcular_fecha_vencimiento(char* fecha_vencimiento, int dias);
void generar_numero_comprobante(char* numero, const char* placa);
void crear_carpetas_sistema();

// Funciones de interfaz
void mostrar_estado_comprobante(int estado);
void pausar_sistema();

#endif // PAGOS_H