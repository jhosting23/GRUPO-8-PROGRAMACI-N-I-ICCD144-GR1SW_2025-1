// matricula.c - Implementación del cálculo de matrícula vehicular Ecuador
// Creada por: mathias, jhostin, christian
// Fecha: 2025

#include "matricula.h" 
#include "vehiculos.h" 
#include <stdio.h>   
#include <string.h>   
#include <stdlib.h>   

//INICIALIZACIÓN DE VARIABLES GLOBALES 

// VARIABLES GLOBALES - Tasas fijas 2025
double TASA_ANT_PARTICULAR = 36.00;
double TASA_ANT_COMERCIAL = 41.00;
double TASA_ANT_MOTOCICLETA = 31.00;

double TASA_PREFECTURA_PARTICULAR = 18.00;
double TASA_PREFECTURA_COMERCIAL = 20.50;
double TASA_PREFECTURA_MOTOCICLETA = 9.30;

double VALOR_RTV_LIVIANO = 31.00;
double VALOR_RTV_PESADO = 37.00;
double VALOR_RTV_MOTOCICLETA = 22.00;

double VALOR_ADHESIVO = 5.00;

// VARIABLES GLOBALES - Tasas SPPAT por cilindraje
double SPPAT_MOTO_HASTA_200 = 16.00;
double SPPAT_MOTO_MAS_200 = 20.00;
double SPPAT_LIVIANO_HASTA_1500 = 22.00;
double SPPAT_LIVIANO_1501_2500 = 32.00;
double SPPAT_LIVIANO_MAS_2500 = 45.00;
double SPPAT_PESADO = 65.00;
double SPPAT_COMERCIAL = 65.00;

// VARIABLES GLOBALES - Recargos
double RECARGO_ANUAL_PORCENTAJE = 3.0; // 3% anual
double INTERES_MORA_DIARIO = 0.001; // 0.1% diario

// -----IMPLEMENTACIÓN DE FUNCIONES SIMPLIFICADAS --------

// Función SIMPLIFICADA para calcular el Impuesto a la Propiedad
double calcular_impuesto_propiedad(double avaluo) {
	if (avaluo > LIMITE_PROPIEDAD) {
		double excedente = avaluo - LIMITE_PROPIEDAD;
		return excedente * (PORCENTAJE_PROPIEDAD / 100.0);
	}
	return 0.0; // Sin impuesto si está por debajo del límite
}

// Función SIMPLIFICADA para calcular el Impuesto al Rodaje
double calcular_impuesto_rodaje(double avaluo) {
	if (avaluo > LIMITE_RODAJE) {
		double excedente = avaluo - LIMITE_RODAJE;
		return excedente * (PORCENTAJE_RODAJE / 100.0);
	}
	return 0.0; // Sin impuesto si está por debajo del límite
}

// Función para calcular SPPAT usando las variables globales
double calcular_tasa_sppat(char* tipo, char* subtipo, int cilindraje) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) {
		if (cilindraje <= 200) {
			return SPPAT_MOTO_HASTA_200;
		} else {
			return SPPAT_MOTO_MAS_200;
		}
	} else if (strcmp(tipo, "COMERCIAL") == 0) {
		return SPPAT_COMERCIAL;
	} else if (strcmp(subtipo, "PESADO") == 0) {
		return SPPAT_PESADO;
	} else { // Vehículos livianos particulares
		if (cilindraje <= 1500) {
			return SPPAT_LIVIANO_HASTA_1500;
		} else if (cilindraje <= 2500) {
			return SPPAT_LIVIANO_1501_2500;
		} else {
			return SPPAT_LIVIANO_MAS_2500;
		}
	}
}

// Función para calcular Tasa ANT usando las variables globales
double calcular_tasa_ant(char* tipo, char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) {
		return TASA_ANT_MOTOCICLETA;
	} else if (strcmp(tipo, "COMERCIAL") == 0) {
		return TASA_ANT_COMERCIAL;
	} else {
		return TASA_ANT_PARTICULAR;
	}
}

// Función para calcular Tasa Prefectura usando las variables globales
double calcular_tasa_prefectura(char* tipo, char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) {
		return TASA_PREFECTURA_MOTOCICLETA;
	} else if (strcmp(tipo, "COMERCIAL") == 0) {
		return TASA_PREFECTURA_COMERCIAL;
	} else {
		return TASA_PREFECTURA_PARTICULAR;
	}
}

// Función para calcular RTV usando las variables globales
double calcular_valor_rtv(char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) {
		return VALOR_RTV_MOTOCICLETA;
	} else if (strcmp(subtipo, "PESADO") == 0) {
		return VALOR_RTV_PESADO;
	} else {
		return VALOR_RTV_LIVIANO;
	}
}

// Función para calcular recargos por mora usando las variables globales
double calcular_recargos_mora(double impuesto_propiedad, double impuesto_rodaje, int meses_retraso) {
	if (meses_retraso <= 0) return 0.0;
	
	double base_calculo = impuesto_propiedad + impuesto_rodaje;
	double recargo_anual = base_calculo * (RECARGO_ANUAL_PORCENTAJE / 100.0);
	double recargo_mensual = recargo_anual / 12.0;
	
	return recargo_mensual * meses_retraso;
}

// Función principal que usa todas las variables globales
ResultadoMatricula calcular_matricula_completa(DatosVehiculo vehiculo) {
	ResultadoMatricula resultado = {0};
	
	// Calcular cada componente usando las fórmulas simplificadas
	resultado.impuesto_propiedad = calcular_impuesto_propiedad(vehiculo.avaluo);
	resultado.impuesto_rodaje = calcular_impuesto_rodaje(vehiculo.avaluo);
	resultado.tasa_sppat = calcular_tasa_sppat(vehiculo.tipo, vehiculo.subtipo, vehiculo.cilindraje);
	resultado.tasa_ant = calcular_tasa_ant(vehiculo.tipo, vehiculo.subtipo);
	resultado.tasa_prefectura = calcular_tasa_prefectura(vehiculo.tipo, vehiculo.subtipo);
	resultado.valor_rtv = calcular_valor_rtv(vehiculo.subtipo);
	resultado.valor_adhesivo = VALOR_ADHESIVO;
	resultado.multas_pendientes = vehiculo.tiene_multas ? vehiculo.valor_multas : 0.0;
	resultado.recargos_mora = calcular_recargos_mora(resultado.impuesto_propiedad,
													 resultado.impuesto_rodaje,
													 vehiculo.meses_retraso);
	
	// Calcular total
	resultado.total_matricula = resultado.impuesto_propiedad + resultado.impuesto_rodaje +
		resultado.tasa_sppat + resultado.tasa_ant +
		resultado.tasa_prefectura + resultado.valor_rtv +
		resultado.valor_adhesivo + resultado.multas_pendientes +
		resultado.recargos_mora;
	
	return resultado;
}

// Función para mostrar el desglose completo
void mostrar_desglose_matricula(ResultadoMatricula resultado) {
	printf("\n=== DESGLOSE DE MATRICULA %d ===\n", ANO_FISCAL);
	printf("+-----------------------------------------+\n");
	printf("¦ IMPUESTOS:                              ¦\n");
	printf("¦ • Impuesto a la Propiedad (SRI): $%.2f ¦\n", resultado.impuesto_propiedad);
	printf("¦ • Impuesto al Rodaje (AMT):      $%.2f ¦\n", resultado.impuesto_rodaje);
	printf("+-----------------------------------------¦\n");
	printf("¦ TASAS:                                  ¦\n");
	printf("¦ • Tasa SPPAT (ANT):              $%.2f ¦\n", resultado.tasa_sppat);
	printf("¦ • Tasa ANT:                      $%.2f ¦\n", resultado.tasa_ant);
	printf("¦ • Tasa Prefectura Pichincha:     $%.2f ¦\n", resultado.tasa_prefectura);
	printf("+-----------------------------------------¦\n");
	printf("¦ SERVICIOS:                              ¦\n");
	printf("¦ • Revision Tecnica (RTV):        $%.2f ¦\n", resultado.valor_rtv);
	printf("¦ • Adhesivo (Sticker):            $%.2f ¦\n", resultado.valor_adhesivo);
	printf("+-----------------------------------------¦\n");
	printf("¦ ADICIONALES:                            ¦\n");
	printf("¦ • Multas Pendientes:             $%.2f ¦\n", resultado.multas_pendientes);
	printf("¦ • Recargos por Mora:             $%.2f ¦\n", resultado.recargos_mora);
	printf("+-----------------------------------------¦\n");
	printf("¦ TOTAL A PAGAR:                   $%.2f ¦\n", resultado.total_matricula);
	printf("+-----------------------------------------+\n");
}

// Función para obtener datos del vehículo desde el archivo (usando vehiculos.h)
int obtener_datos_vehiculo_para_calculo(const char* placa, DatosVehiculo* vehiculo_data) {
	return obtener_datos_vehiculo_para_calculo_desde_archivo(placa, vehiculo_data);
}

// Menú principal para cálculo de matrícula
void menu_calculo_matricula() {
	char placa[10];
	DatosVehiculo vehiculo = {0};
	ResultadoMatricula resultado;
	
	printf("=== CALCULO DE MATRICULA VEHICULAR ===\n");
	printf("Ingrese la placa del vehiculo (formato ABC-1234): ");
	scanf("%s", placa);
	
	// Convertir la placa ingresada a mayúsculas
	for (int i = 0; i < strlen(placa); i++) {
		if (placa[i] >= 'a' && placa[i] <= 'z') {
			placa[i] = placa[i] - 32;
		}
	}
	
	// Intentar obtener los datos del vehículo desde el archivo
	if (!obtener_datos_vehiculo_para_calculo(placa, &vehiculo)) {
		printf("Error: Vehiculo con placa '%s' no encontrado en el archivo de datos.\n", placa);
		printf("Asegurese de que el archivo '%s' existe y contenga el vehiculo.\n", ARCHIVO_VEHICULOS);
		return;
	}
	
	// Preguntar por multas y retrasos después de obtener los datos base
	printf("\n¿Tiene multas pendientes? (1=Si, 0=No): ");
	scanf("%d", &vehiculo.tiene_multas);
	
	if (vehiculo.tiene_multas) {
		printf("Ingrese el valor total de multas: $");
		scanf("%lf", &vehiculo.valor_multas);
	}
	
	printf("¿Cuantos meses de retraso tiene? (0=ninguno): ");
	scanf("%d", &vehiculo.meses_retraso);
	
	// Calcular matrícula usando las fórmulas simplificadas
	resultado = calcular_matricula_completa(vehiculo);
	
	// Mostrar resultado
	system("cls"); 
	printf("=== VEHICULO: %s ===\n", vehiculo.placa);
	printf("Tipo: %s - Subtipo: %s\n", vehiculo.tipo, vehiculo.subtipo);
	printf("Avaluo: $%.2f - Cilindraje: %d cc\n", vehiculo.avaluo, vehiculo.cilindraje);
	
	mostrar_desglose_matricula(resultado);
}

// Función para actualizar valores anuales (mantenimiento del sistema)
void actualizar_valores_anuales(int nuevo_ano) {
	printf("=== ACTUALIZACION DE VALORES PARA EL AÑO %d ===\n", nuevo_ano);
	printf("ATENCION: Esta funcion permite actualizar las variables globales.\n");
	printf("Se recomienda consultar las fuentes oficiales antes de actualizar.\n");
	
	printf("Valores actualizados para el año %d\n", nuevo_ano);
}
