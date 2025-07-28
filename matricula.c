// matricula.c - Implementacin del clculo de matrcula vehicular Ecuador
// Creada por: mathias, jhostin, christian
// Fecha: 2025

#include "matricula.h" 
#include "vehiculos.h" // Necesario para obtener datos del veh?culo
#include <stdio.h>   
#include <string.h>   
#include <stdlib.h>   
// --- FUNCIONES DE C?LCULO ---
// Estas funciones ahora usan las constantes de matricula.h

double calcular_impuesto_propiedad(double avaluo) {
	if (avaluo > LIMITE_PROPIEDAD) {
		return (avaluo - LIMITE_PROPIEDAD) * (PORCENTAJE_PROPIEDAD / 100.0);
	}
	return 0.0;
}

double calcular_impuesto_rodaje(double avaluo) {
	if (avaluo > LIMITE_RODAJE) {
		return (avaluo - LIMITE_RODAJE) * (PORCENTAJE_RODAJE / 100.0);
	}
	return 0.0;
}

double calcular_tasa_sppat(char* tipo, char* subtipo, int cilindraje) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) {
		return (cilindraje <= 200) ? SPPAT_MOTO_HASTA_200 : SPPAT_MOTO_MAS_200;
	}
	if (strcmp(tipo, "COMERCIAL") == 0) return SPPAT_COMERCIAL;
	if (strcmp(subtipo, "PESADO") == 0) return SPPAT_PESADO;
	
	if (cilindraje <= 1500) return SPPAT_LIVIANO_HASTA_1500;
	if (cilindraje <= 2500) return SPPAT_LIVIANO_1501_2500;
	return SPPAT_LIVIANO_MAS_2500;
}

double calcular_tasa_ant(char* tipo, char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) return TASA_ANT_MOTOCICLETA;
	if (strcmp(tipo, "COMERCIAL") == 0) return TASA_ANT_COMERCIAL;
	return TASA_ANT_PARTICULAR;
}

double calcular_tasa_prefectura(char* tipo, char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) return TASA_PREFECTURA_MOTOCICLETA;
	if (strcmp(tipo, "COMERCIAL") == 0) return TASA_PREFECTURA_COMERCIAL;
	return TASA_PREFECTURA_PARTICULAR;
}

double calcular_valor_rtv(char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) return VALOR_RTV_MOTOCICLETA;
	if (strcmp(subtipo, "PESADO") == 0) return VALOR_RTV_PESADO;
	return VALOR_RTV_LIVIANO;
}

double calcular_recargos_mora(double impuesto_propiedad, double impuesto_rodaje, int meses_retraso) {
	if (meses_retraso <= 0) return 0.0;
	double base_calculo = impuesto_propiedad + impuesto_rodaje;
	double recargo_anual = base_calculo * (RECARGO_ANUAL_PORCENTAJE / 100.0);
	return (recargo_anual / 12.0) * meses_retraso;
}

ResultadoMatricula calcular_matricula_completa(DatosVehiculo vehiculo) {
	ResultadoMatricula res = {0};
	res.impuesto_propiedad = calcular_impuesto_propiedad(vehiculo.avaluo);
	res.impuesto_rodaje = calcular_impuesto_rodaje(vehiculo.avaluo);
	res.tasa_sppat = calcular_tasa_sppat(vehiculo.tipo, vehiculo.subtipo, vehiculo.cilindraje);
	res.tasa_ant = calcular_tasa_ant(vehiculo.tipo, vehiculo.subtipo);
	res.tasa_prefectura = calcular_tasa_prefectura(vehiculo.tipo, vehiculo.subtipo);
	res.valor_rtv = calcular_valor_rtv(vehiculo.subtipo);
	res.valor_adhesivo = VALOR_ADHESIVO;
	res.multas_pendientes = vehiculo.tiene_multas ? vehiculo.valor_multas : 0.0;
	res.recargos_mora = calcular_recargos_mora(res.impuesto_propiedad, res.impuesto_rodaje, vehiculo.meses_retraso);
	
	res.total_matricula = res.impuesto_propiedad + res.impuesto_rodaje + res.tasa_sppat + 
		res.tasa_ant + res.tasa_prefectura + res.valor_rtv + 
		res.valor_adhesivo + res.multas_pendientes + res.recargos_mora;
	return res;
}

// --- FUNCIONES DE INTERFAZ ---
void mostrar_desglose_matricula(ResultadoMatricula res) {
	printf("\n=== DESGLOSE DE MATRICULA %d ===\n", ANO_FISCAL);
	printf("+-----------------------------------------+\n");
	printf("| IMPUESTOS:\n");
	printf("|   Impuesto a la Propiedad (SRI): $%-8.2f |\n", res.impuesto_propiedad);
	printf("|   Impuesto al Rodaje (AMT):      $%-8.2f |\n", res.impuesto_rodaje);
	printf("+-----------------------------------------+\n");
	printf("| TASAS:\n");
	printf("|   Tasa SPPAT:                    $%-8.2f |\n", res.tasa_sppat);
	printf("|   Tasa ANT:                      $%-8.2f |\n", res.tasa_ant);
	printf("|   Tasa Prefectura:               $%-8.2f |\n", res.tasa_prefectura);
	printf("+-----------------------------------------+\n");
	printf("| SERVICIOS Y OTROS:\n");
	printf("|   Revision Tecnica (RTV):        $%-8.2f |\n", res.valor_rtv);
	printf("|   Adhesivo (Sticker):            $%-8.2f |\n", res.valor_adhesivo);
	printf("+-----------------------------------------+\n");
	printf("| ADICIONALES (si aplica):\n");
	printf("|   Multas Pendientes:             $%-8.2f |\n", res.multas_pendientes);
	printf("|   Recargos por Mora:             $%-8.2f |\n", res.recargos_mora);
	printf("+-----------------------------------------+\n");
	printf("| TOTAL A PAGAR:                   $%-8.2f |\n", res.total_matricula);
	printf("+-----------------------------------------+\n");
}

void menu_calculo_matricula() {
	char placa[10];
	DatosVehiculo vehiculo = {0};
	char buffer[100];
	
	// Paso 1: Obtener una placa v?lida y existente
	while (1) {
		limpiar_pantalla();
		printf("=== CALCULO DE MATRICULA VEHICULAR ===\n");
		printf("Ingrese la placa del vehiculo (o '0' para volver): ");
		if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%9s", placa); else continue;
		
		if (strcmp(placa, "0") == 0) return; // Salir si el usuario ingresa 0
		
		convertir_a_mayusculas(placa);
		
		if (!validar_placa(placa)) {
			printf("\nERROR: Formato de placa incorrecto. Debe ser ABC-1234.\nPresione Enter para continuar...");
			getchar();
			continue;
		}
		
		if (obtener_datos_vehiculo_para_calculo_desde_archivo(placa, &vehiculo)) {
			break; // Veh?culo encontrado, salir del bucle
		} else {
			printf("\nERROR: Vehiculo con placa '%s' no encontrado.\nPresione Enter para continuar...", placa);
			getchar();
		}
	}
	
	// Paso 2: Obtener datos adicionales (multas, retrasos)
	limpiar_pantalla();
	printf("=== VEHICULO ENCONTRADO ===\n");
	printf("Placa: %s, Tipo: %s, Avaluo: $%.2f\n\n", vehiculo.placa, vehiculo.tipo, vehiculo.avaluo);
	
	while (1) {
		printf("Tiene multas pendientes? (1=Si, 0=No): ");
		if (fgets(buffer, sizeof(buffer), stdin)) {
			if (sscanf(buffer, "%d", &vehiculo.tiene_multas) == 1) {
				if (vehiculo.tiene_multas == 1 || vehiculo.tiene_multas == 0) break;
			}
		}
		printf("   ERROR: Ingrese 1 para Si o 0 para No.\n");
	}
	
	if (vehiculo.tiene_multas) {
		while (1) {
			printf("Ingrese el valor total de multas: $");
			if (fgets(buffer, sizeof(buffer), stdin)) {
				if (sscanf(buffer, "%lf", &vehiculo.valor_multas) == 1) {
					if (vehiculo.valor_multas > 0) break;
				}
			}
			printf("   ERROR: El valor de la multa debe ser un numero mayor a cero.\n");
		}
	}
	
	while (1) {
		printf("Cuantos meses de retraso tiene? (0 si no tiene): ");
		if (fgets(buffer, sizeof(buffer), stdin)) {
			if (sscanf(buffer, "%d", &vehiculo.meses_retraso) == 1) {
				if (vehiculo.meses_retraso >= 0) break;
			}
		}
		printf("   ERROR: Los meses de retraso no pueden ser negativos.\n");
	}
	
	// Paso 3: Calcular y mostrar resultados
	ResultadoMatricula resultado = calcular_matricula_completa(vehiculo);
	limpiar_pantalla();
	printf("=== CALCULO FINALIZADO PARA PLACA: %s ===\n", vehiculo.placa);
	mostrar_desglose_matricula(resultado);
}
