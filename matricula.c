/*
 * matricula.c - Implementacion del calculo de matricula vehicular Ecuador
 * 
 * Descripcion: Este archivo implementa todas las funciones necesarias para
 *              calcular la matricula vehicular en Ecuador, incluyendo:
 *              - Calculo de impuestos y tasas
 *              - Generacion de comprobantes
 *              - Interfaz de usuario para calculos
 * 
 *
 * Mathias, Jhostin, Christian
 * Fecha: 2025
 * Materia: Programacion I - ICCD144
 * Universidad: escuela politecnica nacional
 */

#include "matricula.h" 
#include "vehiculos.h"    // Necesario para obtener datos del vehiculo
#include "pagos.h"        // Necesario para guardar comprobantes en sistema de pagos
#include <stdio.h>   
#include <string.h>   
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <direct.h>       // Para _mkdir en Windows
#include <sys/stat.h>     // Para verificar si existe la carpeta

// ===================================================================
// FUNCIONES DE CALCULO DE IMPUESTOS Y TASAS
// ===================================================================

/*
 * Funcion: calcular_impuesto_propiedad
 * Descripcion: Calcula el impuesto a la propiedad vehicular segun SRI
 *              Solo se aplica si el avaluo supera el limite establecido
 * Parametros: avaluo - Valor comercial del vehiculo
 * Retorno: Monto del impuesto a pagar (0 si no aplica)
 */
double calcular_impuesto_propiedad(double avaluo) {
	if (avaluo > LIMITE_PROPIEDAD) {
		return (avaluo - LIMITE_PROPIEDAD) * (PORCENTAJE_PROPIEDAD / 100.0);
	}
	return 0.0;
}

/*
 * Funcion: calcular_impuesto_rodaje
 * Descripcion: Calcula el impuesto de rodaje vehicular segun AMT
 *              Solo se aplica si el avaluo supera el limite establecido
 * Parametros: avaluo - Valor comercial del vehiculo
 * Retorno: Monto del impuesto a pagar (0 si no aplica)
 */
double calcular_impuesto_rodaje(double avaluo) {
	if (avaluo > LIMITE_RODAJE) {
		return (avaluo - LIMITE_RODAJE) * (PORCENTAJE_RODAJE / 100.0);
	}
	return 0.0;
}

/*
 * Funcion: calcular_tasa_sppat
 * Descripcion: Calcula la tasa SPPAT segun el tipo de vehiculo y cilindraje
 * Parametros: 
 *   - tipo: Tipo de vehiculo (PARTICULAR/COMERCIAL)
 *   - subtipo: Subtipo (LIVIANO/PESADO/MOTOCICLETA)
 *   - cilindraje: Cilindraje del motor en cc
 * Retorno: Monto de la tasa SPPAT
 */

double calcular_tasa_sppat(char* tipo, char* subtipo, int cilindraje) {
	// Verificar si es motocicleta
	if (strcmp(subtipo, "MOTOCICLETA") == 0) {
		return (cilindraje <= 200) ? SPPAT_MOTO_HASTA_200 : SPPAT_MOTO_MAS_200;
	}
	
	// Verificar si es vehiculo comercial
	if (strcmp(tipo, "COMERCIAL") == 0) return SPPAT_COMERCIAL;
	
	// Verificar si es vehiculo pesado
	if (strcmp(subtipo, "PESADO") == 0) return SPPAT_PESADO;
	
	// Para vehiculos livianos, aplicar segun cilindraje
	if (cilindraje <= 1500) return SPPAT_LIVIANO_HASTA_1500;
	if (cilindraje <= 2500) return SPPAT_LIVIANO_1501_2500;
	return SPPAT_LIVIANO_MAS_2500;
}

/*
 * Funcion: calcular_tasa_ant
 * Descripcion: Calcula la tasa ANT segun el tipo de vehiculo
 * Parametros:
 *   - tipo: Tipo de vehiculo (PARTICULAR/COMERCIAL)
 *   - subtipo: Subtipo (LIVIANO/PESADO/MOTOCICLETA)
 * Retorno: Monto de la tasa ANT
 */
double calcular_tasa_ant(char* tipo, char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) return TASA_ANT_MOTOCICLETA;
	if (strcmp(tipo, "COMERCIAL") == 0) return TASA_ANT_COMERCIAL;
	return TASA_ANT_PARTICULAR;
}

/*
 * Funcion: calcular_tasa_prefectura
 * Descripcion: Calcula la tasa de prefectura segun el tipo de vehiculo
 * Parametros:
 *   - tipo: Tipo de vehiculo (PARTICULAR/COMERCIAL)
 *   - subtipo: Subtipo (LIVIANO/PESADO/MOTOCICLETA)
 * Retorno: Monto de la tasa de prefectura
 */
double calcular_tasa_prefectura(char* tipo, char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) return TASA_PREFECTURA_MOTOCICLETA;
	if (strcmp(tipo, "COMERCIAL") == 0) return TASA_PREFECTURA_COMERCIAL;
	return TASA_PREFECTURA_PARTICULAR;
}

/*
 * Funcion: calcular_valor_rtv
 * Descripcion: Calcula el valor de revision tecnica vehicular segun subtipo
 * Parametros: subtipo - Subtipo del vehiculo (LIVIANO/PESADO/MOTOCICLETA)
 * Retorno: Monto del valor RTV
 */
double calcular_valor_rtv(char* subtipo) {
	if (strcmp(subtipo, "MOTOCICLETA") == 0) return VALOR_RTV_MOTOCICLETA;
	if (strcmp(subtipo, "PESADO") == 0) return VALOR_RTV_PESADO;
	return VALOR_RTV_LIVIANO;
}

/*
 * Funcion: calcular_recargos_mora
 * Descripcion: Calcula los recargos por mora en pagos de impuestos
 * Parametros:
 *   - impuesto_propiedad: Monto del impuesto a la propiedad
 *   - impuesto_rodaje: Monto del impuesto de rodaje
 *   - meses_retraso: Numero de meses de retraso en el pago
 * Retorno: Monto total de recargos por mora
 */

double calcular_recargos_mora(double impuesto_propiedad, double impuesto_rodaje, int meses_retraso) {
	if (meses_retraso <= 0) return 0.0;
	
	// Calcular base sobre la cual se aplican recargos
	double base_calculo = impuesto_propiedad + impuesto_rodaje;
	
	// Calcular recargo anual y convertir a mensual
	double recargo_anual = base_calculo * (RECARGO_ANUAL_PORCENTAJE / 100.0);
	return (recargo_anual / 12.0) * meses_retraso;
}

/*
 * Funcion: calcular_matricula_completa
 * Descripcion: Realiza el calculo completo de matricula vehicular
 *              incluyendo todos los impuestos, tasas y recargos
 * Parametros: vehiculo - Estructura con datos del vehiculo
 * Retorno: Estructura con todos los valores calculados
 */
ResultadoMatricula calcular_matricula_completa(DatosVehiculo vehiculo) {
	ResultadoMatricula res = {0};
	
	// Calcular impuestos
	res.impuesto_propiedad = calcular_impuesto_propiedad(vehiculo.avaluo);
	res.impuesto_rodaje = calcular_impuesto_rodaje(vehiculo.avaluo);
	
	// Calcular tasas
	res.tasa_sppat = calcular_tasa_sppat(vehiculo.tipo, vehiculo.subtipo, vehiculo.cilindraje);
	res.tasa_ant = calcular_tasa_ant(vehiculo.tipo, vehiculo.subtipo);
	res.tasa_prefectura = calcular_tasa_prefectura(vehiculo.tipo, vehiculo.subtipo);
	
	// Calcular servicios
	res.valor_rtv = calcular_valor_rtv(vehiculo.subtipo);
	res.valor_adhesivo = VALOR_ADHESIVO;
	
	// Calcular adicionales
	res.multas_pendientes = vehiculo.tiene_multas ? vehiculo.valor_multas : 0.0;
	res.recargos_mora = calcular_recargos_mora(res.impuesto_propiedad, res.impuesto_rodaje, vehiculo.meses_retraso);
	
	// Calcular total final
	res.total_matricula = res.impuesto_propiedad + res.impuesto_rodaje + res.tasa_sppat + 
		res.tasa_ant + res.tasa_prefectura + res.valor_rtv + 
		res.valor_adhesivo + res.multas_pendientes + res.recargos_mora;
	
	return res;
}

// ===================================================================
// FUNCIONES DE INTERFAZ DE USUARIO
// ===================================================================
/*
 * Funcion: mostrar_desglose_matricula
 * Descripcion: Muestra el desglose detallado de todos los costos de matricula
 * Parametros: res - Estructura con los resultados del calculo
 * Retorno: void
 */
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

/*
 * Funcion: menu_calculo_matricula
 * Descripcion: Menu principal para el calculo de matricula vehicular
 *              Guia al usuario a traves de todo el proceso de calculo
 * Parametros: Ninguno
 * Retorno: void
 */

void menu_calculo_matricula() {
	char placa[10];
	DatosVehiculo vehiculo = {0};
	char buffer[100];
	
	// Paso 1: Obtener una placa valida y existente
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
			break; // Vehiculo encontrado, salir del bucle
		} else {
			printf("\nERROR: Vehiculo con placa '%s' no encontrado.\nPresione Enter para continuar...", placa);
			getchar();
		}
	}
	
	// Paso 2: Obtener datos adicionales (multas, retrasos)
	limpiar_pantalla();
	printf("=== VEHICULO ENCONTRADO ===\n");
	printf("Placa: %s, Tipo: %s, Avaluo: $%.2f\n\n", vehiculo.placa, vehiculo.tipo, vehiculo.avaluo);
	
	// Solicitar informacion sobre multas
	while (1) {
		printf("Tiene multas pendientes? (1=Si, 0=No): ");
		if (fgets(buffer, sizeof(buffer), stdin)) {
			if (sscanf(buffer, "%d", &vehiculo.tiene_multas) == 1) {
				if (vehiculo.tiene_multas == 1 || vehiculo.tiene_multas == 0) break;
			}
		}
		printf("   ERROR: Ingrese 1 para Si o 0 para No.\n");
	}
	
	// Si tiene multas, solicitar el valor
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
	
	// Solicitar informacion sobre retrasos
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
	
	// Paso 4: Preguntar si desea generar comprobante
	printf("\nDesea generar el comprobante completo? (S/N): ");
	char respuesta;
	if (fgets(buffer, sizeof(buffer), stdin)) {
		sscanf(buffer, "%c", &respuesta);
		if (respuesta == 'S' || respuesta == 's') {
			// Generar numero de comprobante una sola vez
			char numero_comprobante[50];
			generar_numero_comprobante(numero_comprobante, vehiculo.placa);
			
			// Mostrar comprobante con el numero generado
			generar_comprobante_matricula(vehiculo.placa, resultado, vehiculo, numero_comprobante);
			
			// Tambien guardar en el sistema de pagos con el mismo numero
			if (guardar_comprobante_sistema(vehiculo.placa, resultado, vehiculo, numero_comprobante)) {
				printf("\nComprobante guardado en el sistema de pagos.\n");
				printf("Puede usar la placa %s para realizar el pago posteriormente.\n", vehiculo.placa);
			}
		}
	}
	
	printf("\nPresione Enter para continuar...");
	getchar();
}

// ===================================================================
// FUNCIONES DE GENERACION DE COMPROBANTES
// ===================================================================

/*
 * Funcion: imprimir_linea_decorativa
 * Descripcion: Imprime una linea decorativa con el caracter especificado
 * Parametros:
 *   - caracter: Caracter a usar para la linea
 *   - longitud: Longitud de la linea en caracteres
 * Retorno: void
 */
void imprimir_linea_decorativa(char caracter, int longitud) {
	for (int i = 0; i < longitud; i++) {
		printf("%c", caracter);
	}
	printf("\n");
}

/*
 * Funcion: imprimir_encabezado_comprobante
 * Descripcion: Imprime el encabezado del comprobante con fecha y hora
 * Parametros: Ninguno
 * Retorno: void
 */
void imprimir_encabezado_comprobante(void) {
	imprimir_linea_decorativa('=', 55);
	printf("          COMPROBANTE DE MATRICULA VEHICULAR\n");
	printf("           AGENCIA NACIONAL DE TRANSITO\n");
	imprimir_linea_decorativa('=', 55);
	
	// Obtener fecha y hora actual
	time_t t = time(NULL);
	struct tm *fecha_actual = localtime(&t);
	
	printf("Fecha: %02d/%02d/%d\n", fecha_actual->tm_mday, 
		   fecha_actual->tm_mon + 1, fecha_actual->tm_year + 1900);
	printf("Hora:  %02d:%02d:%02d\n", fecha_actual->tm_hour,
		   fecha_actual->tm_min, fecha_actual->tm_sec);
}

/*
 * Funcion: imprimir_pie_comprobante
 * Descripcion: Imprime el pie del comprobante con informacion importante
 * Parametros: Ninguno
 * Retorno: void
 */
void imprimir_pie_comprobante(void) {
	printf("\nINFORMACION IMPORTANTE:\n");
	imprimir_linea_decorativa('-', 55);
	printf("- Este comprobante debe ser conservado\n");
	printf("- Valido por 30 dias para realizar el pago\n");
	printf("- Presente este comprobante en ventanilla\n");
	imprimir_linea_decorativa('=', 55);
}

/*
 * Funcion: generar_comprobante_matricula
 * Descripcion: Genera y muestra el comprobante completo de matricula
 * Parametros:
 *   - placa: Placa del vehiculo
 *   - resultado: Estructura con los resultados del calculo
 *   - vehiculo: Datos del vehiculo
 *   - numero_comprobante: Numero del comprobante a mostrar
 * Retorno: void
 */
void generar_comprobante_matricula(const char* placa, ResultadoMatricula resultado, DatosVehiculo vehiculo, const char* numero_comprobante) {
	// Limpiar pantalla
	limpiar_pantalla();
	
	// Imprimir encabezado
	imprimir_encabezado_comprobante();
	
	// Mostrar placa prominentemente
	printf("Comprobante No: %s\n", numero_comprobante);
	imprimir_linea_decorativa('=', 55);
	printf("                    PLACA: %s\n", vehiculo.placa);
	imprimir_linea_decorativa('=', 55);
	
	// Datos del vehiculo
	printf("\nDATOS DEL VEHICULO:\n");
	imprimir_linea_decorativa('-', 55);
	printf("Placa:                      %s\n", vehiculo.placa);
	printf("Tipo:                       %s\n", vehiculo.tipo);
	printf("Subtipo:                    %s\n", vehiculo.subtipo);
	printf("Avaluo:                     $%.2f\n", vehiculo.avaluo);
	printf("Cilindraje:                 %d cc\n", vehiculo.cilindraje);
	printf("\n");
	
	// Desglose detallado
	printf("DESGLOSE DE COSTOS:\n");
	imprimir_linea_decorativa('-', 55);
	printf("Impuesto a la Propiedad:    $%12.2f\n", resultado.impuesto_propiedad);
	printf("Impuesto de Rodaje:         $%12.2f\n", resultado.impuesto_rodaje);
	printf("Tasa SPPAT:                 $%12.2f\n", resultado.tasa_sppat);
	printf("Tasa ANT:                   $%12.2f\n", resultado.tasa_ant);
	printf("Tasa Prefectura:            $%12.2f\n", resultado.tasa_prefectura);
	printf("Valor RTV:                  $%12.2f\n", resultado.valor_rtv);
	printf("Valor Adhesivo:             $%12.2f\n", resultado.valor_adhesivo);
	
	if (resultado.multas_pendientes > 0) {
		printf("Multas Pendientes:          $%12.2f\n", resultado.multas_pendientes);
	}
	
	if (resultado.recargos_mora > 0) {
		printf("Recargos por Mora:          $%12.2f\n", resultado.recargos_mora);
	}
	
	imprimir_linea_decorativa('-', 55);
	printf("TOTAL A PAGAR:              $%12.2f\n", resultado.total_matricula);
	imprimir_linea_decorativa('=', 55);
	
	// Imprimir pie
	imprimir_pie_comprobante();
	
	// Preguntar si desea guardar
	printf("\nDesea guardar este comprobante en archivo? (S/N): ");
	char respuesta;
	char buffer[10];
	if (fgets(buffer, sizeof(buffer), stdin)) {
		sscanf(buffer, "%c", &respuesta);
		if (respuesta == 'S' || respuesta == 's') {
			guardar_comprobante_archivo(placa, resultado, vehiculo, numero_comprobante);
			printf("Comprobante guardado exitosamente en 'comprobantes/comprobante_%s.txt'.\n", placa);
		}
	}
	
	printf("\nPresione Enter para continuar...");
	getchar();
}

// Funcion para guardar comprobante en archivo
void guardar_comprobante_archivo(const char* placa, ResultadoMatricula resultado, DatosVehiculo vehiculo, const char* numero_comprobante) {
	// Crear carpeta de comprobantes si no existe
	struct stat st = {0};
	if (stat("comprobantes", &st) == -1) {
		_mkdir("comprobantes");
	}
	
	// Crear nombre de archivo con la placa en la carpeta de comprobantes
	char nombre_archivo[100];
	sprintf(nombre_archivo, "comprobantes/comprobante_%s.txt", placa);
	
	FILE *archivo = fopen(nombre_archivo, "a");
	if (archivo == NULL) {
		printf("Error: No se pudo crear el archivo de comprobantes.\n");
		printf("Verifique que tenga permisos de escritura en esta carpeta.\n");
		return;
	}
	
	// Obtener fecha actual
	time_t t = time(NULL);
	struct tm *fecha_actual = localtime(&t);
	
	// Validar que localtime funciono correctamente
	if (fecha_actual == NULL) {
		printf("Error: No se pudo obtener la fecha actual.\n");
		fclose(archivo);
		return;
	}
	
	// Escribir comprobante en archivo
	fprintf(archivo, "=======================================================\n");
	fprintf(archivo, "          COMPROBANTE DE MATRICULA VEHICULAR\n");
	fprintf(archivo, "           AGENCIA NACIONAL DE TRANSITO\n");
	fprintf(archivo, "                    PLACA: %s\n", vehiculo.placa);
	fprintf(archivo, "=======================================================\n");
	fprintf(archivo, "Fecha: %02d/%02d/%d %02d:%02d:%02d\n\n", 
			fecha_actual->tm_mday, fecha_actual->tm_mon + 1, 
			fecha_actual->tm_year + 1900, fecha_actual->tm_hour,
			fecha_actual->tm_min, fecha_actual->tm_sec);
	
	// Datos del vehiculo
	fprintf(archivo, "DATOS DEL VEHICULO:\n");
	fprintf(archivo, "-------------------------------------------------------\n");
	fprintf(archivo, "Placa: %s\n", vehiculo.placa);
	fprintf(archivo, "Tipo: %s\n", vehiculo.tipo);
	fprintf(archivo, "Subtipo: %s\n", vehiculo.subtipo);
	fprintf(archivo, "Avaluo: $%.2f\n", vehiculo.avaluo);
	fprintf(archivo, "Cilindraje: %d cc\n\n", vehiculo.cilindraje);
	
	// Desglose de costos
	fprintf(archivo, "DESGLOSE DE COSTOS:\n");
	fprintf(archivo, "-------------------------------------------------------\n");
	fprintf(archivo, "Impuesto a la Propiedad: $%.2f\n", resultado.impuesto_propiedad);
	fprintf(archivo, "Impuesto de Rodaje: $%.2f\n", resultado.impuesto_rodaje);
	fprintf(archivo, "Tasa SPPAT: $%.2f\n", resultado.tasa_sppat);
	fprintf(archivo, "Tasa ANT: $%.2f\n", resultado.tasa_ant);
	fprintf(archivo, "Tasa Prefectura: $%.2f\n", resultado.tasa_prefectura);
	fprintf(archivo, "Valor RTV: $%.2f\n", resultado.valor_rtv);
	fprintf(archivo, "Valor Adhesivo: $%.2f\n", resultado.valor_adhesivo);
	
	if (resultado.multas_pendientes > 0) {
		fprintf(archivo, "Multas Pendientes: $%.2f\n", resultado.multas_pendientes);
	}
	
	if (resultado.recargos_mora > 0) {
		fprintf(archivo, "Recargos por Mora: $%.2f\n", resultado.recargos_mora);
	}
	
	fprintf(archivo, "-------------------------------------------------------\n");
	fprintf(archivo, "TOTAL A PAGAR: $%.2f\n", resultado.total_matricula);
	fprintf(archivo, "=======================================================\n\n");
	
	// Cerrar archivo de forma segura
	if (fclose(archivo) == EOF) {
		printf("Advertencia: Error al cerrar el archivo de comprobantes.\n");
	}
}

/*
 * Funcion: calcular_matricula_mostrar
 * Descripcion: Interfaz principal para calcular el valor de matricula de un vehiculo
 *              Solicita la placa, busca el vehiculo, calcula la matricula y 
 *              pregunta si desea generar comprobante
 * Parametros: ninguno
 * Retorno: void
 */
void calcular_matricula_mostrar() {
	char placa[20];
	char buffer[100];
	DatosVehiculo vehiculo;
	ResultadoMatricula resultado;
	
	system("cls");
	printf("\n");
	printf("=======================================================\n");
	printf("           CALCULO DE VALOR DE MATRICULA\n");
	printf("           AGENCIA NACIONAL DE TRANSITO\n");
	printf("=======================================================\n");
	printf("\n");
	
	// Solicitar placa del vehiculo
	printf("Ingrese la placa del vehiculo: ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la placa del vehiculo.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Limpiar salto de linea
	buffer[strcspn(buffer, "\n")] = 0;
	strcpy(placa, buffer);
	
	// Convertir a mayusculas
	for (int i = 0; placa[i]; i++) {
		placa[i] = toupper(placa[i]);
	}
	
	// Buscar vehiculo en el archivo
	if (!obtener_datos_vehiculo_para_calculo_desde_archivo(placa, &vehiculo)) {
		printf("Error: Vehiculo con placa '%s' no encontrado.\n", placa);
		printf("Debe registrar el vehiculo primero antes de calcular la matricula.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Calcular matricula
	resultado = calcular_matricula_completa(vehiculo);
	
	// Mostrar resultado del calculo
	printf("\n");
	printf("CALCULO DE MATRICULA COMPLETADO:\n");
	printf("=======================================================\n");
	printf("Vehiculo: %s (%s %s)\n", vehiculo.placa, vehiculo.tipo, vehiculo.subtipo);
	printf("Avaluo comercial: $%.2f\n", vehiculo.avaluo);
	printf("\n");
	printf("DESGLOSE DE COSTOS:\n");
	printf("-------------------------------------------------------\n");
	printf("Impuesto a la Propiedad:    $%8.2f\n", resultado.impuesto_propiedad);
	printf("Impuesto de Rodaje:         $%8.2f\n", resultado.impuesto_rodaje);
	printf("Tasa SPPAT:                 $%8.2f\n", resultado.tasa_sppat);
	printf("Tasa ANT:                   $%8.2f\n", resultado.tasa_ant);
	printf("Tasa Prefectura:            $%8.2f\n", resultado.tasa_prefectura);
	printf("Valor RTV:                  $%8.2f\n", resultado.valor_rtv);
	printf("Valor Adhesivo:             $%8.2f\n", resultado.valor_adhesivo);
	
	if (resultado.multas_pendientes > 0) {
		printf("Multas Pendientes:          $%8.2f\n", resultado.multas_pendientes);
	}
	
	if (resultado.recargos_mora > 0) {
		printf("Recargos por Mora:          $%8.2f\n", resultado.recargos_mora);
	}
	
	printf("-------------------------------------------------------\n");
	printf("TOTAL A PAGAR:              $%8.2f\n", resultado.total_matricula);
	printf("=======================================================\n");
	printf("\n");
	
	// Preguntar si desea generar comprobante
	printf("Desea generar comprobante de pago? (S/N): ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la respuesta.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	if (buffer[0] == 'S' || buffer[0] == 's') {
		// Generar numero de comprobante
		char numero_comprobante[50];
		time_t t = time(NULL);
		struct tm* fecha = localtime(&t);
		sprintf(numero_comprobante, "MAT-%s-%04d%02d%02d-%03d", 
				placa, 
				fecha->tm_year + 1900,
				fecha->tm_mon + 1,
				fecha->tm_mday,
				rand() % 1000);
		
		// Generar comprobante completo
		generar_comprobante_matricula(placa, resultado, vehiculo, numero_comprobante);
		
		// Guardar en el sistema de pagos
		if (guardar_comprobante_sistema(placa, resultado, vehiculo, numero_comprobante)) {
			printf("\nComprobante generado exitosamente!\n");
			printf("Ahora puede proceder al pago usando la placa: %s\n", placa);
		} else {
			printf("\nAdvertencia: No se pudo guardar en el sistema de pagos.\n");
		}
	}
	
	printf("\nPresione Enter para continuar...");
	getchar();
}
