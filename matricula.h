// matricula.h - Librera para clculo de matrcula vehicular 
// Creada por: Mathias, Jhostin, Christian
// Fecha: 2025
#ifndef MATRICULA_H
#define MATRICULA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Constante del a?o fiscal
#define ANO_FISCAL 2025

// LMITES PARA CLCULO DE IMPUESTOS
#define LIMITE_PROPIEDAD 30000.00  // Ms de $30,000 paga impuesto
#define LIMITE_RODAJE 50000.00     // Ms de $50,000 paga impuesto
#define PORCENTAJE_PROPIEDAD 1.00  // 1% sobre excedente
#define PORCENTAJE_RODAJE 1.00     // 1% sobre excedente

// TASAS FIJAS ANT (Agencia Nacional de Tr?nsito)
#define TASA_ANT_PARTICULAR 36.00
#define TASA_ANT_COMERCIAL 41.00
#define TASA_ANT_MOTOCICLETA 31.00

// TASAS FIJAS PREFECTURA
#define TASA_PREFECTURA_PARTICULAR 18.00
#define TASA_PREFECTURA_COMERCIAL 20.50
#define TASA_PREFECTURA_MOTOCICLETA 9.30

// VALORES FIJOS RTV (Revisi?n T?cnica Vehicular) 
#define VALOR_RTV_LIVIANO 31.00
#define VALOR_RTV_PESADO 37.00
#define VALOR_RTV_MOTOCICLETA 22.00
#define VALOR_ADHESIVO 5.00

// TASAS SPPAT 
#define SPPAT_MOTO_HASTA_200 16.00
#define SPPAT_MOTO_MAS_200 20.00
#define SPPAT_LIVIANO_HASTA_1500 22.00
#define SPPAT_LIVIANO_1501_2500 32.00
#define SPPAT_LIVIANO_MAS_2500 45.00
#define SPPAT_PESADO 65.00
#define SPPAT_COMERCIAL 65.00

// RECARGOS POR MORA
#define RECARGO_ANUAL_PORCENTAJE 3.0 // 3% anual

// --- ESTRUCTURAS DE DATOS ---
// Estructura para almacenar el resultado del clculo
typedef struct {
	double impuesto_propiedad;
	double impuesto_rodaje;
	double tasa_sppat;
	double tasa_ant;
	double tasa_prefectura;
	double valor_rtv;
	double valor_adhesivo;
	double multas_pendientes;
	double recargos_mora;
	double total_matricula;
} ResultadoMatricula;

// Estructura para datos del vehculo para clculo
typedef struct {
	char placa[10];
	char tipo[20];       
	char subtipo[20];     
	float avaluo;      
	int cilindraje;     
	int tiene_multas;     // 0=No, 1=S 
	double valor_multas;  // Valor de multas pendientes
	int meses_retraso;  
} DatosVehiculo;


// --- DECLARACIN DE FUNCIONES ---
void menu_calculo_matricula(); // Men principal para el c?lculo
void mostrar_desglose_matricula(ResultadoMatricula resultado);
ResultadoMatricula calcular_matricula_completa(DatosVehiculo vehiculo);
#endif
