/*
 * matricula.h - Libreria para calculo de matricula vehicular Ecuador
 * 
 * Descripcion: Este archivo contiene todas las constantes, estructuras y 
 *              prototipos de funciones necesarios para el calculo de 
 *              matricula vehicular segun las tarifas vigentes del Ecuador.
 * 
 * Autores: Mathias, Jhostin, Christian  
 * Fecha: 2025
 * Materia: Programacion I - ICCD144
 * Universidad: Escuela Politecnica Nacional
 */

#ifndef MATRICULA_H
#define MATRICULA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// ===================================================================
// CONSTANTES DEL SISTEMA
// ===================================================================

// Constante del ano fiscal actual
#define ANO_FISCAL 2025

// ===================================================================
// LIMITES Y PORCENTAJES PARA CALCULO DE IMPUESTOS
// ===================================================================
// Limites de avaluo para aplicar impuestos
#define LIMITE_PROPIEDAD 30000.00   // Mas de $30,000 paga impuesto a la propiedad
#define LIMITE_RODAJE 50000.00      // Mas de $50,000 paga impuesto de rodaje

// Porcentajes de impuestos sobre el excedente
#define PORCENTAJE_PROPIEDAD 1.00   // 1% sobre excedente para impuesto propiedad
#define PORCENTAJE_RODAJE 1.00      // 1% sobre excedente para impuesto rodaje

// ===================================================================
// TASAS FIJAS ANT (AGENCIA NACIONAL DE TRANSITO)
// ===================================================================
// Tasas cobradas por la Agencia Nacional de Transito
#define TASA_ANT_PARTICULAR 36.00    // Tasa para vehiculos particulares
#define TASA_ANT_COMERCIAL 41.00     // Tasa para vehiculos comerciales
#define TASA_ANT_MOTOCICLETA 31.00   // Tasa para motocicletas

// ===================================================================
// TASAS FIJAS PREFECTURA DE PICHINCHA
// ===================================================================

// Tasas cobradas por la Prefectura de Pichincha
#define TASA_PREFECTURA_PARTICULAR 18.00    // Tasa para vehiculos particulares
#define TASA_PREFECTURA_COMERCIAL 20.50     // Tasa para vehiculos comerciales
#define TASA_PREFECTURA_MOTOCICLETA 9.30    // Tasa para motocicletas

// ===================================================================
// VALORES REVISION TECNICA VEHICULAR (RTV)
// =================================================================== 
// Costos de revision tecnica vehicular segun tipo de vehiculo
#define VALOR_RTV_LIVIANO 31.00      // Costo RTV para vehiculos livianos
#define VALOR_RTV_PESADO 37.00       // Costo RTV para vehiculos pesados
#define VALOR_RTV_MOTOCICLETA 22.00  // Costo RTV para motocicletas

// Valor del adhesivo (sticker) de matricula
#define VALOR_ADHESIVO 5.00          // Costo del adhesivo de matricula

// ===================================================================
// TASAS SPPAT (SERVICIO DE PROTECCION A LA PROPIEDAD AUTOMOTRIZ)
// ===================================================================

// Tasas SPPAT para motocicletas segun cilindraje
#define SPPAT_MOTO_HASTA_200 16.00   // Motocicletas hasta 200cc
#define SPPAT_MOTO_MAS_200 20.00     // Motocicletas mas de 200cc

// Tasas SPPAT para vehiculos livianos segun cilindraje
#define SPPAT_LIVIANO_HASTA_1500 22.00     // Vehiculos hasta 1500cc
#define SPPAT_LIVIANO_1501_2500 32.00      // Vehiculos de 1501cc a 2500cc
#define SPPAT_LIVIANO_MAS_2500 45.00       // Vehiculos mas de 2500cc

// Tasas SPPAT para vehiculos pesados y comerciales
#define SPPAT_PESADO 65.00           // Vehiculos pesados
#define SPPAT_COMERCIAL 65.00        // Vehiculos comerciales

// ===================================================================
// CONFIGURACION DE RECARGOS POR MORA
// ===================================================================

// Porcentaje de recargo anual por mora en pagos
#define RECARGO_ANUAL_PORCENTAJE 3.0  // 3% anual sobre impuestos

// ===================================================================
// ESTRUCTURAS DE DATOS
// ===================================================================
/*
 * Estructura: ResultadoMatricula
 * Descripcion: Almacena todos los valores calculados para la matricula
 *              de un vehiculo, incluyendo impuestos, tasas y total
 */
typedef struct {
	double impuesto_propiedad;    // Impuesto a la propiedad (SRI)
	double impuesto_rodaje;       // Impuesto de rodaje (AMT)
	double tasa_sppat;           // Tasa SPPAT segun tipo vehiculo
	double tasa_ant;             // Tasa ANT segun tipo vehiculo
	double tasa_prefectura;      // Tasa Prefectura segun tipo vehiculo
	double valor_rtv;            // Valor revision tecnica vehicular
	double valor_adhesivo;       // Valor del adhesivo (sticker)
	double multas_pendientes;    // Multas pendientes de pago
	double recargos_mora;        // Recargos por mora en pagos
	double total_matricula;      // Total final a pagar
} ResultadoMatricula;

/*
 * Estructura: DatosVehiculo
 * Descripcion: Almacena la informacion basica del vehiculo necesaria
 *              para realizar el calculo de matricula
 */
typedef struct {
	char placa[10];              // Placa del vehiculo (formato ABC-1234)
	char cedula[15];             // Cedula del propietario
	char propietario[50];        // Nombre del propietario
	char tipo[20];               // Tipo: PARTICULAR o COMERCIAL
	char subtipo[20];            // Subtipo: LIVIANO, PESADO, MOTOCICLETA
	int ano;                     // Ano del vehiculo
	float avaluo;                // Avaluo comercial del vehiculo
	int cilindraje;              // Cilindraje del motor en cc
	int tiene_multas;            // Indica si tiene multas (0=No, 1=Si)
	double valor_multas;         // Valor total de multas pendientes
	int meses_retraso;           // Meses de retraso en pagos
} DatosVehiculo;

// ===================================================================
// PROTOTIPOS DE FUNCIONES
// ===================================================================

// Funciones del menu principal
void menu_calculo_matricula();
void calcular_matricula_mostrar();

// Funciones de calculo de matricula
ResultadoMatricula calcular_matricula_completa(DatosVehiculo vehiculo);
void mostrar_desglose_matricula(ResultadoMatricula resultado);

// Funciones de generacion de comprobantes
void generar_comprobante_matricula(const char* placa, ResultadoMatricula resultado, DatosVehiculo vehiculo, const char* numero_comprobante);
void guardar_comprobante_archivo(const char* placa, ResultadoMatricula resultado, DatosVehiculo vehiculo, const char* numero_comprobante);

// Funciones auxiliares para comprobantes
void imprimir_linea_decorativa(char caracter, int longitud);
void imprimir_encabezado_comprobante(void);
void imprimir_pie_comprobante(void);

#endif // MATRICULA_H
