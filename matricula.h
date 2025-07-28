// matricula.h - Librería para cálculo de matrícula vehicular 
// Creada por: mathias, jhostin, christian
// Fecha: 2025
#ifndef MATRICULA_H
#define MATRICULA_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -----VARIABLES GLOBALES----
// Constantes del año fiscal 2025
#define ANO_FISCAL 2025

// LÍMITES PARA CÁLCULO 
#define LIMITE_PROPIEDAD 30000.00  // Más de $30,000.01 paga 1%
#define LIMITE_RODAJE 50000.00     // Más de $50,000.01 paga 1%
#define PORCENTAJE_PROPIEDAD 1.00  // 1% sobre excedente
#define PORCENTAJE_RODAJE 1.00     // 1% sobre excedente

// VARIABLES GLOBALES - Tasas fijas 2025
extern double TASA_ANT_PARTICULAR;
extern double TASA_ANT_COMERCIAL;
extern double TASA_ANT_MOTOCICLETA;

extern double TASA_PREFECTURA_PARTICULAR;
extern double TASA_PREFECTURA_COMERCIAL;
extern double TASA_PREFECTURA_MOTOCICLETA;

extern double VALOR_RTV_LIVIANO;
extern double VALOR_RTV_PESADO;
extern double VALOR_RTV_MOTOCICLETA;

extern double VALOR_ADHESIVO;

// VARIABLES GLOBALES - Tasas SPPAT por cilindraje
extern double SPPAT_MOTO_HASTA_200;
extern double SPPAT_MOTO_MAS_200;
extern double SPPAT_LIVIANO_HASTA_1500;
extern double SPPAT_LIVIANO_1501_2500;
extern double SPPAT_LIVIANO_MAS_2500;
extern double SPPAT_PESADO;
extern double SPPAT_COMERCIAL;

// VARIABLES GLOBALES - Recargos
extern double RECARGO_ANUAL_PORCENTAJE;
extern double INTERES_MORA_DIARIO;

// Estructura para almacenar el resultado del cálculo
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

// Estructura para datos del vehículo para cálculo
typedef struct {
	char placa[10];
	char tipo[20];       
	char subtipo[20];     
	float avaluo;      
	int cilindraje;     
	int tiene_multas;     // 0=No, 1=Sí 
	double valor_multas;  // Valor de multas pendientes
	int meses_retraso;    // Meses de retraso
} DatosVehiculo;

// ---DECLARACIÓN DE FUNCIONES ---
// Función principal para calcular matrícula
ResultadoMatricula calcular_matricula_completa(DatosVehiculo vehiculo);

// Funciones de cálculo individual (ahora simplificadas)
double calcular_impuesto_propiedad(double avaluo);
double calcular_impuesto_rodaje(double avaluo);
double calcular_tasa_sppat(char* tipo, char* subtipo, int cilindraje);
double calcular_tasa_ant(char* tipo, char* subtipo);
double calcular_tasa_prefectura(char* tipo, char* subtipo);
double calcular_valor_rtv(char* subtipo);
double calcular_recargos_mora(double impuesto_propiedad, double impuesto_rodaje, int meses_retraso);

void mostrar_desglose_matricula(ResultadoMatricula resultado);

// Función para obtener datos del vehículo para cálculo
extern int obtener_datos_vehiculo_para_calculo_desde_archivo(const char* placa, DatosVehiculo* vehiculo_data);

void menu_calculo_matricula(); // Función del menú

// Función para actualizar valores anuales (mantenimiento)
void actualizar_valores_anuales(int nuevo_ano);

#endif
