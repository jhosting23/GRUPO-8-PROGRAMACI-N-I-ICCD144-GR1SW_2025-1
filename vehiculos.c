/*
 * vehiculos.c - Funciones para validacion y registro de vehiculos
 * 
 * Descripcion: Este archivo implementa todas las funciones necesarias
 *              para el manejo de vehiculos en el sistema, incluyendo:
 *              - Validacion de datos de vehiculos
 *              - Registro de nuevos vehiculos
 *              - Busqueda de vehiculos existentes
 * 
 * Autores: Mathias, Jhostin, Christian
 * Fecha: 2025
 * Materia: Programacion I - ICCD144
 * Universidad: escuela politecnica nacional
 */

#include "vehiculos.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>
#include <time.h>     // Para funciones de fecha y hora

// ===================================================================
// FUNCIONES AUXILIARES
// ===================================================================

/*
 * Funcion: convertir_a_mayusculas
 * Descripcion: Convierte todos los caracteres de una cadena a mayusculas
 * Parametros: cadena - Cadena a convertir (modificada in-place)
 * Retorno: void
 */
void convertir_a_mayusculas(char* cadena) {
	for (int i = 0; cadena[i]; i++) {
		cadena[i] = toupper((unsigned char)cadena[i]);
	}
}

// ===================================================================
// FUNCIONES DE VALIDACION
// ===================================================================
/*
 * Funcion: validar_placa
 * Descripcion: Valida el formato de placa vehicular ecuatoriana (ABC-1234)
 * Parametros: placa - Cadena con la placa a validar
 * Retorno: 1 si es valida, 0 si no es valida
 */
int validar_placa(const char* placa) {
	// Verificar longitud exacta
	if (strlen(placa) != 8) return 0;
	
	// Verificar primeros 3 caracteres son letras mayusculas
	for (int i = 0; i < 3; i++) {
		if (!isupper((unsigned char)placa[i])) return 0;
	}
	
	// Verificar guion en posicion 4
	if (placa[3] != '-') return 0;
	
	// Verificar ultimos 4 caracteres son digitos
	for (int i = 4; i < 8; i++) {
		if (!isdigit((unsigned char)placa[i])) return 0;
	}
	
	return 1;
}

/*
 * Funcion: validar_cedula
 * Descripcion: Valida cedula ecuatoriana usando algoritmo oficial
 * Parametros: cedula - Cadena con la cedula a validar
 * Retorno: 1 si es valida, codigo de error negativo si no es valida
 */

int validar_cedula(const char* cedula) {
	// Verificar que tenga exactamente 10 digitos
	if (strlen(cedula) != 10) return -1; // Error: longitud incorrecta
	
	// Verificar que todos los caracteres sean numeros
	for (int i = 0; i < 10; i++) {
		if (!isdigit((unsigned char)cedula[i])) return -2; // Error: contiene caracteres no numericos
	}
	
	// Convertir string a array de enteros para facilitar calculos
	int digitos[10];
	for (int i = 0; i < 10; i++) {
		digitos[i] = cedula[i] - '0';
	}
	
	// Validar que los dos primeros digitos correspondan a una provincia (01-24)
	int provincia = digitos[0] * 10 + digitos[1];
	if (provincia < 1 || provincia > 24) return -3; // Error: provincia invalida
	
	// Validar que el tercer digito sea menor a 6 (0,1,2,3,4,5)
	if (digitos[2] >= 6) return -4; // Error: tercer digito invalido
	
	// Algoritmo de validacion Modulo 10
	// Coeficientes para multiplicar cada digito
	int coeficientes[9] = {2, 1, 2, 1, 2, 1, 2, 1, 2};
	int suma = 0;
	
	// Multiplicar cada digito por su coeficiente correspondiente
	for (int i = 0; i < 9; i++) {
		int producto = digitos[i] * coeficientes[i];
		// Si el producto es mayor a 9, sumar sus digitos
		if (producto > 9) {
			producto = producto - 9;
		}
		suma += producto;
	}
	
	// Calcular el digito verificador
	int digito_verificador = suma % 10;
	if (digito_verificador != 0) {
		digito_verificador = 10 - digito_verificador;
	}
	
	// Comparar con el decimo digito de la cedula
	if (digito_verificador != digitos[9]) return -5; // Error: digito verificador incorrecto
	
	return 1; // Cedula valida
}

/*
 * Funcion: obtener_precio_vehiculo
 * Descripcion: Obtiene el precio del vehiculo segun su tipo
 * Parametros: tipo - Tipo de vehiculo (1-4)
 * Retorno: Precio del vehiculo en USD
 */

/*
 * Funcion: validar_cilindraje
 * Descripcion: Valida que el cilindraje este dentro del rango permitido
 * Parametros: cilindraje - Valor del cilindraje a validar
 * Retorno: 1 si es valido, 0 si no es valido
 */
int validar_cilindraje(int cilindraje) {
	return (cilindraje >= 50 && cilindraje <= 8000);
}

/*
 * Funcion: vehiculo_ya_existe
 * Descripcion: Verifica si un vehiculo ya esta registrado en el sistema
 * Parametros: placa - Placa del vehiculo a verificar
 * Retorno: 1 si existe, 0 si no existe
 */
int vehiculo_ya_existe(const char* placa) {
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) return 0;
	
	char linea[MAX_LINEA2], placa_archivo[10];
	while (fgets(linea, sizeof(linea), archivo)) {
		sscanf(linea, "%9[^,]", placa_archivo);
		if (strcmp(placa_archivo, placa) == 0) {
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	return 0;
}

/*
 * Funcion: validar_nombre
 * Descripcion: Valida que el nombre contenga solo letras y espacios
 * Parametros: nombre - Nombre del propietario a validar
 * Retorno: 1 si es valido, 0 si no es valido
 */
int validar_nombre(const char* nombre) {
	if (strlen(nombre) < 3) return 0;
	
	for (int i = 0; nombre[i]; i++) {
		if (!isalpha((unsigned char)nombre[i]) && !isspace((unsigned char)nombre[i])) {
			return 0;
		}
	}
	return 1;
}

/*
 * Funcion: validar_valor
 * Descripcion: Valida que el valor del vehiculo este dentro del rango permitido
 * Parametros: valor - Valor del vehiculo a validar
 * Retorno: 1 si es valido, 0 si no es valido
 */
int validar_valor(float valor) {
	return (valor >= MIN_AVALUO && valor <= MAX_AVALUO);
}


/*
 * Funcion: registrar_vehiculo
 * Descripcion: Registra un nuevo vehiculo en el sistema validando todos los datos
 * Parametros: Ninguno
 * Retorno: 1 si el registro fue exitoso, 0 si hubo error
 */
int registrar_vehiculo(void) {
	char placa[10] = "", cedula[15] = "", nombre[50] = "", tipo[20] = "", subtipo[20] = "";
	int anio = 0, cilindraje = 0;
	float valor = 0.0;
	char buffer[100];
	int paso_actual = 1;
	
	while(paso_actual <= 7) {
		limpiar_pantalla();
		printf("=== REGISTRO DE VEHICULO (Paso %d de 7) ===\n", paso_actual);
		printf("---------------------------------------------------\n");
		// Muestra los datos que ya han sido validados
		if (strlen(placa) > 0) printf("1. Placa: %s\n", placa);
		if (strlen(cedula) > 0) printf("2. Cedula: %s\n", cedula);
		if (strlen(nombre) > 0) printf("3. Propietario: %s\n", nombre);
		if (strlen(tipo) > 0) printf("4. Tipo/Subtipo: %s / %s\n", tipo, subtipo);
		if (anio > 0) printf("5. Ano: %d\n", anio);
		if (valor > 0) printf("6. Avaluo: $%.2f\n", valor);
		printf("---------------------------------------------------\n\n");
		
		switch(paso_actual) {
		case 1: // Placa
			printf("Ingrese la placa (formato ABC-1234): ");
			if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%9s", placa); else continue;
			convertir_a_mayusculas(placa);
			if (!validar_placa(placa)) {
				printf("\nERROR: Formato de placa incorrecto. Debe tener el siguiente formato (ABC-1234).\nPresione Enter para reintentar...");
				getchar();
				strcpy(placa, ""); // Limpiar dato incorrecto
			} else if (vehiculo_ya_existe(placa)) {
				printf("\nERROR: Un vehiculo con la placa '%s' ya esta registrado.\nPresione Enter para reintentar...", placa);
				getchar();
				strcpy(placa, "");
			} else { paso_actual++; }
			break;
		case 2: // Cedula
			printf("Ingrese la cedula (10 digitos): ");
			if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%14s", cedula); else continue;
			int resultado_validacion = validar_cedula(cedula);
			if (resultado_validacion != 1) {
				switch (resultado_validacion) {
					case -1:
						printf("\nERROR: La cedula debe tener exactamente 10 digitos.\nPresione Enter para reintentar...");
						break;
					case -2:
						printf("\nERROR: La cedula solo debe contener numeros, no letras ni caracteres especiales.\nPresione Enter para reintentar...");
						break;
					case -3:
						printf("\nERROR: Los dos primeros digitos deben corresponder a una provincia valida (01-24).\nPresione Enter para reintentar...");
						break;
					case -4:
						printf("\nERROR: El tercer digito debe ser menor a 6 (0,1,2,3,4,5).\nPresione Enter para reintentar...");
						break;
					case -5:
						printf("\nERROR: El formato de la cedula es incorrecto (digito verificador invalido).\nPresione Enter para reintentar...");
						break;
					default:
						printf("\nERROR: Cedula invalida.\nPresione Enter para reintentar...");
						break;
				}
				getchar();
				strcpy(cedula, "");
			} else { paso_actual++; }
			break;
		case 3: // Nombre
			printf("Ingrese el nombre del propietario: ");
			if (fgets(nombre, sizeof(nombre), stdin)) nombre[strcspn(nombre, "\n")] = '\0'; else continue;
			if (!validar_nombre(nombre)) {
				printf("\nERROR: El nombre solo puede contener letras y espacios.\nPresione Enter para reintentar...");
				getchar();
				strcpy(nombre, "");
			} else { paso_actual++; }
			break;
		case 4: // Tipo y Subtipo
			printf("Seleccione el tipo de vehiculo:\n");
			printf("1. PARTICULAR\n");
			printf("2. COMERCIAL\n");
			printf("Ingrese su opcion (1 o 2): ");
			
			int opcion_tipo = 0;
			if (fgets(buffer, sizeof(buffer), stdin)) {
				if (sscanf(buffer, "%d", &opcion_tipo) != 1) opcion_tipo = 0;
			}
			
			if (opcion_tipo == 1) {
				strcpy(tipo, "PARTICULAR");
				printf("\n�Es motocicleta?\n");
				printf("1. SI\n");
				printf("2. NO\n");
				printf("Ingrese su opcion (1 o 2): ");
				
				int opcion_moto = 0;
				if (fgets(buffer, sizeof(buffer), stdin)) {
					if (sscanf(buffer, "%d", &opcion_moto) != 1) opcion_moto = 0;
				}
				
				if (opcion_moto == 1) {
					strcpy(subtipo, "MOTOCICLETA");
				} else if (opcion_moto == 2) {
					printf("\nSeleccione el subtipo:\n");
					printf("1. LIVIANO\n");
					printf("2. PESADO\n");
					printf("Ingrese su opcion (1 o 2): ");
					
					int opcion_subtipo = 0;
					if (fgets(buffer, sizeof(buffer), stdin)) {
						if (sscanf(buffer, "%d", &opcion_subtipo) != 1) opcion_subtipo = 0;
					}
					
					if (opcion_subtipo == 1) {
						strcpy(subtipo, "LIVIANO");
					} else if (opcion_subtipo == 2) {
						strcpy(subtipo, "PESADO");
					} else {
						strcpy(tipo, ""); // Error
					}
				} else {
					strcpy(tipo, ""); // Error
				}
			} else if (opcion_tipo == 2) {
				strcpy(tipo, "COMERCIAL");
				strcpy(subtipo, "COMERCIAL");
			} else {
				strcpy(tipo, ""); // Error
			}
			
			if (strlen(tipo) == 0) {
				printf("\nERROR: Opcion invalida. Debe seleccionar 1 o 2.\nPresione Enter para reintentar...");
				getchar();
			} else { 
				paso_actual++; 
			}
			break;
		case 5: // Ano
			printf("Ingrese el ano del vehiculo (1990-%d): ", ANO_FISCAL);
			if (fgets(buffer, sizeof(buffer), stdin)) { if(sscanf(buffer, "%d", &anio) != 1) anio = 0; }
			if (anio < 1990 || anio > ANO_FISCAL) {
				printf("\nERROR: Ano fuera del rango permitido.\nPresione Enter para reintentar...");
				getchar();
				anio = 0;
			} else { paso_actual++; }
			break;
		case 6: // Valor
			printf("Ingrese el avaluo ($%.2f - $%.2f): ", MIN_AVALUO, MAX_AVALUO);
			if (fgets(buffer, sizeof(buffer), stdin)) { if(sscanf(buffer, "%f", &valor) != 1) valor = 0.0; }
			if (!validar_valor(valor)) {
				printf("\nERROR: Valor fuera del rango permitido.\nPresione Enter para reintentar...");
				getchar();
				valor = 0.0;
			} else { paso_actual++; }
			break;
		case 7: // Cilindraje
			printf("Ingrese el cilindraje del motor (en cc): ");
			if (fgets(buffer, sizeof(buffer), stdin)) { if(sscanf(buffer, "%d", &cilindraje) != 1) cilindraje = 0; }
			if (!validar_cilindraje(cilindraje)) {
				printf("\nERROR: Cilindraje fuera del rango (50-8000cc).\nPresione Enter para reintentar...");
				getchar();
				cilindraje = 0;
			} else { paso_actual++; }
			break;
		}
	}
	
	// --- Guardar en Archivo ---
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "a");
	if (archivo == NULL) {
		printf("\nERROR CRITICO: No se pudo abrir el archivo %s.\n", ARCHIVO_VEHICULOS);
		return 0;
	}
	fprintf(archivo, "%s,%s,%s,%s,%s,%d,%.2f,%d\n", placa, cedula, nombre, tipo, subtipo, anio, valor, cilindraje);
	fclose(archivo);
	
	limpiar_pantalla();
	printf("=== VEHICULO REGISTRADO CON EXITO ===\n\n");
	printf(" Placa: %s\n Propietario: %s\n", placa, nombre);
	printf("\nPresione Enter para volver al menu...");
	getchar();
	
	return 1;
}


/*
 * Funcion: buscar_vehiculo
 * Descripcion: Busca un vehiculo por placa y muestra su informacion
 * Parametros: Ninguno
 * Retorno: 1 si se encontro el vehiculo, 0 si no se encontro
 */
int buscar_vehiculo(void) {
	char placa_buscar[10], linea[MAX_LINEA2];
	char placa[10], cedula[15], nombre[50], tipo[20], subtipo[20];
	int anio, cilindraje;
	float valor;
	char buffer[100];
	
	limpiar_pantalla();
	printf("=== BUSCAR VEHICULO POR PLACA ===\n");
	do {
		limpiar_pantalla();
		printf("Ingrese la placa a buscar (formato ABC-1234): ");
		if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%9s", placa_buscar);
		else continue;
		convertir_a_mayusculas(placa_buscar);
		
		if (!validar_placa(placa_buscar)) {
			printf("   ERROR: Formato de placa incorrecto. Intente de nuevo.\n");
			continue;
		}
		break;
	} while (1);
	
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		printf("No hay vehiculos registrados o no se pudo abrir el archivo %s.\n", ARCHIVO_VEHICULOS);
		return 0;
	}
	
	while (fgets(linea, sizeof(linea), archivo)) {
		int items_leidos = sscanf(linea, "%9[^,],%14[^,],%49[^,],%19[^,],%19[^,],%d,%f,%d",
								  placa, cedula, nombre, tipo, subtipo, &anio, &valor, &cilindraje);
		
		if (items_leidos == 8 && strcmp(placa, placa_buscar) == 0) {
			printf("\n--- VEHICULO ENCONTRADO ---\n");
			printf(" Placa:        %s\n", placa);
			printf(" Propietario:  %s\n", nombre);
			printf(" Cedula:       %s\n", cedula);
			printf(" Tipo:         %s\n", tipo);
			printf(" Subtipo:      %s\n", subtipo);
			printf(" Ano:          %d\n", anio);
			printf(" Valor:        $%.2f\n", valor);
			printf(" Cilindraje:   %d cc\n", cilindraje);
			printf("---------------------------\n");
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	printf("\nVehiculo con placa '%s' no fue encontrado.\n", placa_buscar);
	return 0;
}

/**Aqui lo que se hace es que se busca los datos del vehiculo que usurio desea mediante la placa , se abre el txt y se procede
a guardar lso datos en la estructura daatos vehiculos**/

int obtener_datos_vehiculo_para_calculo_desde_archivo(const char* placa_buscada, DatosVehiculo* vehiculo_data) {
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		printf("Error: No se pudo abrir el archivo de vehiculos en '%s'.\n", ARCHIVO_VEHICULOS);
		return 0;
	}
	char linea[MAX_LINEA2];
	char placa_leida[10], cedula[15], nombre[50], tipo_str[20], subtipo_str[20];
	int anio, cilindraje;
	float avaluo;
	
	while (fgets(linea, sizeof(linea), archivo)) {
		int items_leidos = sscanf(linea, "%9[^,],%14[^,],%49[^,],%19[^,],%19[^,],%d,%f,%d",
								  placa_leida, cedula, nombre, tipo_str, subtipo_str,
								  &anio, &avaluo, &cilindraje);
		
		if (items_leidos == 8 && strcmp(placa_leida, placa_buscada) == 0) {
			strcpy(vehiculo_data->placa, placa_leida);
			strcpy(vehiculo_data->cedula, cedula);
			strcpy(vehiculo_data->propietario, nombre);
			strcpy(vehiculo_data->tipo, tipo_str);
			strcpy(vehiculo_data->subtipo, subtipo_str);
			vehiculo_data->ano = anio;
			vehiculo_data->avaluo = avaluo;
			vehiculo_data->cilindraje = cilindraje;
			vehiculo_data->tiene_multas = 0;
			vehiculo_data->valor_multas = 0.0;
			vehiculo_data->meses_retraso = 0;
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	return 0;
}

// ===================================================================
// FUNCIONES DE CONSULTA Y REPORTES
// ===================================================================

/*
 * Funcion: mostrar_vehiculos_matriculados
 * Descripcion: Muestra un listado de todos los vehiculos que tienen
 *              comprobantes de matricula generados
 * Parametros: ninguno
 * Retorno: void
 */
void mostrar_vehiculos_matriculados() {
    system("cls");
    printf("\n");
    printf("===========================================================================\n");
    printf("                    LISTADO DE VEHICULOS MATRICULADOS\n");
    printf("                       AGENCIA NACIONAL DE TRANSITO\n");
    printf("===========================================================================\n");
    printf("\n");
    
    // Verificar si existe el archivo de vehiculos matriculados
    FILE* archivo = fopen("vehiculos_matriculados.txt", "r");
    if (!archivo) {
        printf("No se encontraron vehiculos matriculados.\n");
        printf("El archivo de vehiculos matriculados no existe.\n");
        printf("\nPresione Enter para continuar...");
        getchar();
        return;
    }
    
    printf("%-12s %-20s %-25s %-15s %-12s %-15s\n", 
           "PLACA", "CERTIFICADO", "PROPIETARIO", "TIPO VEHICULO", "FECHA", "ESTADO");
    printf("---------------------------------------------------------------------------\n");
    
    char linea[500];
    int contador = 0;
    
    while (fgets(linea, sizeof(linea), archivo)) {
        // Parsear la linea del vehiculo matriculado
        char certificado[50], placa[20], cedula[15], propietario[100], tipo[20];
        char subtipo[20], fecha_matricula[20], estado[20];
        int ano, cilindraje;
        float valor;
        
        // Formato: certificado|placa|cedula|propietario|tipo|ano|valor|cilindraje|subtipo|fecha_matricula|estado
        if (sscanf(linea, "%49[^|]|%19[^|]|%14[^|]|%99[^|]|%19[^|]|%d|%f|%d|%19[^|]|%19[^|]|%19[^\n]",
                   certificado, placa, cedula, propietario, tipo, &ano, &valor, &cilindraje, 
                   subtipo, fecha_matricula, estado) == 11) {
            
            contador++;
            
            // Formatear tipo completo
            char tipo_completo[100];
            snprintf(tipo_completo, sizeof(tipo_completo), "%s-%s", tipo, subtipo);
            
            // Mostrar la linea del vehiculo matriculado
            printf("%-12s %-20.20s %-25.25s %-15.15s %-12s %-15s\n",
                   placa, certificado, propietario, tipo_completo, fecha_matricula, estado);
        }
    }
    
    fclose(archivo);
    
    if (contador == 0) {
        printf("No se encontraron vehiculos matriculados.\n");
    } else {
        printf("---------------------------------------------------------------------------\n");
        printf("Total de vehiculos matriculados: %d\n", contador);
    }
    
    printf("\nPresione Enter para continuar...");
    getchar();
}

/*
 * Funcion: mostrar_reporte_detallado_vehiculos
 * Descripcion: Muestra un reporte detallado de los vehiculos matriculados
 *              con informacion adicional como fechas, estados de pago, etc.
 * Parametros: ninguno
 * Retorno: void
 */
void mostrar_reporte_detallado_vehiculos() {
    system("cls");
    printf("\n");
    printf("===========================================================================\n");
    printf("                  REPORTE DETALLADO DE VEHICULOS MATRICULADOS\n");
    printf("                       AGENCIA NACIONAL DE TRANSITO\n");
    printf("===========================================================================\n");
    printf("\n");
    
    // Verificar si existe el archivo de comprobantes
    FILE* archivo = fopen("comprobantes/comprobantes.txt", "r");
    if (!archivo) {
        printf("No se encontraron vehiculos matriculados.\n");
        printf("El archivo de comprobantes no existe.\n");
        printf("\nPresione Enter para continuar...");
        getchar();
        return;
    }
    
    char linea[500];
    int contador = 0;
    float total_recaudado = 0;
    int pagados = 0, pendientes = 0, vencidos = 0;
    
    while (fgets(linea, sizeof(linea), archivo)) {
        // Parsear la linea del comprobante
        char placa[20], numero_comprobante[50], propietario[100], tipo[50], subtipo[50];
        char fecha_emision[20], fecha_vencimiento[20];
        float total;
        int estado;
        
        if (sscanf(linea, "%19[^|]|%49[^|]|%99[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%f|%d",
                   placa, numero_comprobante, propietario, tipo, subtipo, 
                   fecha_emision, fecha_vencimiento, &total, &estado) == 9) {
            
            contador++;
            
            printf("===========================================================================\n");
            printf("VEHICULO #%d\n", contador);
            printf("===========================================================================\n");
            printf("  Placa:                %s\n", placa);
            printf("  Numero de Comprobante: %s\n", numero_comprobante);
            printf("  Propietario:          %s\n", propietario);
            printf("  Tipo de Vehiculo:     %s - %s\n", tipo, subtipo);
            printf("  Fecha de Emision:     %s\n", fecha_emision);
            printf("  Fecha de Vencimiento: %s\n", fecha_vencimiento);
            printf("  Total a Pagar:        $%.2f\n", total);
            
            // Determinar estado y contabilizar
            char estado_str[20];
            switch(estado) {
                case 0: 
                    strcpy(estado_str, "PENDIENTE DE PAGO"); 
                    pendientes++;
                    break;
                case 1: 
                    strcpy(estado_str, "PAGADO"); 
                    pagados++;
                    total_recaudado += total;
                    break;
                case 2: 
                    strcpy(estado_str, "VENCIDO"); 
                    vencidos++;
                    break;
                default: 
                    strcpy(estado_str, "ESTADO DESCONOCIDO");
            }
            
            printf("  Estado:               %s\n", estado_str);
            
            // Verificar estado de revision tecnica
            if (vehiculo_tiene_revision(placa)) {
                printf("  Revision Tecnica:     APROBADA\n");
            } else {
                printf("  Revision Tecnica:     PENDIENTE\n");
            }
            
            printf("\n");
        }
    }
    
    fclose(archivo);
    
    if (contador == 0) {
        printf("No se encontraron vehiculos matriculados.\n");
    } else {
        printf("===========================================================================\n");
        printf("                              RESUMEN ESTADISTICO\n");
        printf("===========================================================================\n");
        printf("  Total de Vehiculos Matriculados: %d\n", contador);
        printf("  Comprobantes Pagados:            %d\n", pagados);
        printf("  Comprobantes Pendientes:         %d\n", pendientes);
        printf("  Comprobantes Vencidos:           %d\n", vencidos);
        printf("  Total Recaudado:                 $%.2f\n", total_recaudado);
        printf("  Porcentaje de Pago:              %.1f%%\n", 
               contador > 0 ? (float)pagados / contador * 100 : 0);
        printf("===========================================================================\n");
    }
    
    printf("\nPresione Enter para continuar...");
    getchar();
}

// ===================================================================
// IMPLEMENTACION DE FUNCIONES PARA REVISIONES TECNICAS SIMPLIFICADAS
// ===================================================================

/*
 * Funcion: registrar_revision_simple
 * Descripcion: Registra de forma simple si un vehiculo aprobo la revision tecnica
 * Parametros: Ninguno
 * Retorno: 1 si el registro fue exitoso, 0 si hubo error
 */
int registrar_revision_simple() {
	RevisionTecnicaSimple revision;
	char buffer[100];
	
	limpiar_pantalla();
	printf("=== REGISTRAR REVISION TECNICA ===\n");
	printf("===================================\n\n");
	
	// Solicitar placa del vehiculo
	printf("Ingrese la placa del vehiculo: ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la placa.\n");
		pausar();
		return 0;
	}
	sscanf(buffer, "%9s", revision.placa);
	convertir_a_mayusculas(revision.placa);
	
	// Validar que el vehiculo existe
	if (!vehiculo_ya_existe(revision.placa)) {
		printf("\nError: El vehiculo con placa %s no esta registrado en el sistema.\n", revision.placa);
		pausar();
		return 0;
	}
	
	// Verificar si ya tiene revision registrada
	if (vehiculo_tiene_revision(revision.placa)) {
		printf("\nEl vehiculo %s ya tiene una revision tecnica registrada.\n", revision.placa);
		printf("Desea actualizar el registro? (S/N): ");
		if (!fgets(buffer, sizeof(buffer), stdin)) {
			pausar();
			return 0;
		}
		if (buffer[0] != 'S' && buffer[0] != 's') {
			printf("Operacion cancelada.\n");
			pausar();
			return 0;
		}
	}
	
	// Obtener fecha de revision
	printf("\nIngrese la fecha de revision (DD/MM/AAAA): ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la fecha.\n");
		pausar();
		return 0;
	}
	sscanf(buffer, "%19s", revision.fecha_revision);
	
	// Preguntar si aprobo la revision
	printf("\nEl vehiculo APROBO la revision tecnica? (S/N): ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		revision.aprobada = 0;
	} else {
		revision.aprobada = (buffer[0] == 'S' || buffer[0] == 's') ? 1 : 0;
	}
	
	// Obtener observaciones (opcional)
	printf("\nObservaciones (opcional): ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		strcpy(revision.observaciones, "Sin observaciones");
	} else {
		buffer[strcspn(buffer, "\n")] = 0;
		if (strlen(buffer) == 0) {
			strcpy(revision.observaciones, "Sin observaciones");
		} else {
			strncpy(revision.observaciones, buffer, 99);
			revision.observaciones[99] = '\0';
		}
	}
	
	// Guardar en archivo
	FILE* archivo = fopen(ARCHIVO_REVISIONES, "a");
	if (archivo == NULL) {
		printf("\nError: No se pudo abrir el archivo de revisiones.\n");
		pausar();
		return 0;
	}
	
	fprintf(archivo, "%s,%s,%d,%s\n",
			revision.placa, revision.fecha_revision, 
			revision.aprobada, revision.observaciones);
	
	fclose(archivo);
	
	// Mostrar resultado
	printf("\n=== REVISION TECNICA REGISTRADA ===\n");
	printf("Placa: %s\n", revision.placa);
	printf("Fecha: %s\n", revision.fecha_revision);
	printf("Estado: %s\n", revision.aprobada ? "APROBADA" : "NO APROBADA");
	printf("Observaciones: %s\n", revision.observaciones);
	
	printf("\nRevision registrada exitosamente!\n");
	pausar();
	return 1;
}

/*
 * Funcion: vehiculo_tiene_revision
 * Descripcion: Verifica si un vehiculo tiene revision tecnica aprobada
 * Parametros: placa - Placa del vehiculo a verificar
 * Retorno: 1 si tiene revision aprobada, 0 si no la tiene
 */
int vehiculo_tiene_revision(const char* placa) {
	FILE* archivo = fopen(ARCHIVO_REVISIONES, "r");
	if (archivo == NULL) return 0;
	
	char linea[200];
	while (fgets(linea, sizeof(linea), archivo)) {
		char placa_archivo[10];
		int aprobada;
		sscanf(linea, "%9[^,],%*[^,],%d", placa_archivo, &aprobada);
		
		if (strcmp(placa_archivo, placa) == 0 && aprobada == 1) {
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	return 0;
}

/*
 * Funcion: consultar_revision_vehiculo
 * Descripcion: Consulta la revision tecnica de un vehiculo especifico
 * Parametros: Ninguno
 * Retorno: 1 si encontro revision, 0 si no encontro
 */
int consultar_revision_vehiculo() {
	char placa[10], buffer[100];
	
	limpiar_pantalla();
	printf("=== CONSULTAR REVISION TECNICA ===\n");
	printf("===================================\n\n");
	
	printf("Ingrese la placa del vehiculo: ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la placa.\n");
		pausar();
		return 0;
	}
	sscanf(buffer, "%9s", placa);
	convertir_a_mayusculas(placa);
	
	// Verificar que el vehiculo existe
	if (!vehiculo_ya_existe(placa)) {
		printf("\nError: El vehiculo con placa %s no esta registrado.\n", placa);
		pausar();
		return 0;
	}
	
	FILE* archivo = fopen(ARCHIVO_REVISIONES, "r");
	if (archivo == NULL) {
		printf("\nNo hay revisiones registradas en el sistema.\n");
		pausar();
		return 0;
	}
	
	printf("\n=== REVISION TECNICA DEL VEHICULO %s ===\n", placa);
	printf("=========================================\n");
	
	char linea[200];
	int encontrada = 0;
	
	while (fgets(linea, sizeof(linea), archivo)) {
		RevisionTecnicaSimple rev;
		sscanf(linea, "%9[^,],%19[^,],%d,%99[^\n]",
			   rev.placa, rev.fecha_revision, &rev.aprobada, rev.observaciones);
		
		if (strcmp(rev.placa, placa) == 0) {
			encontrada = 1;
			printf("\nFecha de revision: %s\n", rev.fecha_revision);
			printf("Estado: %s\n", rev.aprobada ? "APROBADA" : "NO APROBADA");
			printf("Observaciones: %s\n", rev.observaciones);
			printf("Apto para matricular: %s\n", rev.aprobada ? "SI" : "NO");
			break;
		}
	}
	
	fclose(archivo);
	
	if (!encontrada) {
		printf("\nNo se encontro revision tecnica para el vehiculo %s.\n", placa);
		printf("El vehiculo debe realizar la revision tecnica antes de matricular.\n");
	}
	
	pausar();
	return encontrada;
}

/*
 * Funcion: proceso_matriculacion
 * Descripcion: Proceso integrado de matriculacion (revision + calculo + pago)
 * Parametros: Ninguno
 * Retorno: void
 */
void proceso_matriculacion() {
	char placa[10], buffer[100];
	DatosVehiculo vehiculo_data;
	ResultadoMatricula resultado;
	
	limpiar_pantalla();
	printf("=== PROCESO DE MATRICULACION VEHICULAR ===\n");
	printf("===========================================\n\n");
	
	// Paso 1: Solicitar placa
	printf("PASO 1: IDENTIFICACION DEL VEHICULO\n");
	printf("-----------------------------------\n");
	printf("Ingrese la placa del vehiculo: ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la placa.\n");
		pausar();
		return;
	}
	sscanf(buffer, "%9s", placa);
	convertir_a_mayusculas(placa);
	
	// Verificar que el vehiculo existe
	if (!vehiculo_ya_existe(placa)) {
		printf("\nError: El vehiculo con placa %s no esta registrado.\n", placa);
		printf("Debe registrar el vehiculo primero.\n");
		pausar();
		return;
	}
	
	printf("Vehiculo encontrado: %s\n", placa);
	
	// Paso 2: Verificar revision tecnica
	printf("\nPASO 2: VERIFICACION DE REVISION TECNICA\n");
	printf("----------------------------------------\n");
	
	if (!vehiculo_tiene_revision(placa)) {
		printf("El vehiculo NO tiene revision tecnica aprobada.\n");
		printf("Desea registrar la revision tecnica ahora? (S/N): ");
		if (!fgets(buffer, sizeof(buffer), stdin)) {
			printf("Proceso cancelado.\n");
			pausar();
			return;
		}
		
		if (buffer[0] == 'S' || buffer[0] == 's') {
			// Registro rapido de revision
			printf("\nREGISTRO RAPIDO DE REVISION TECNICA:\n");
			printf("El vehiculo aprobo la revision tecnica? (S/N): ");
			if (!fgets(buffer, sizeof(buffer), stdin)) {
				printf("Proceso cancelado.\n");
				pausar();
				return;
			}
			
			if (buffer[0] != 'S' && buffer[0] != 's') {
				printf("No se puede continuar sin revision tecnica aprobada.\n");
				pausar();
				return;
			}
			
			// Guardar revision aprobada
			FILE* archivo = fopen(ARCHIVO_REVISIONES, "a");
			if (archivo) {
				time_t t = time(NULL);
				struct tm tm = *localtime(&t);
				fprintf(archivo, "%s,%02d/%02d/%04d,1,Registro durante matriculacion\n",
						placa, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
				fclose(archivo);
				printf("Revision tecnica registrada como APROBADA.\n");
			}
		} else {
			printf("No se puede continuar sin revision tecnica.\n");
			pausar();
			return;
		}
	} else {
		printf("El vehiculo TIENE revision tecnica aprobada.\n");
	}
	
	// Paso 3: Obtener datos del vehiculo para calculo
	printf("\nPASO 3: CALCULO DE MATRICULA\n");
	printf("----------------------------\n");
	
	if (!obtener_datos_vehiculo_para_calculo_desde_archivo(placa, &vehiculo_data)) {
		printf("Error: No se pudieron obtener los datos del vehiculo.\n");
		pausar();
		return;
	}
	
	// Calcular matricula
	resultado = calcular_matricula_completa(vehiculo_data);
	
	printf("Calculando matricula para %s...\n", placa);
	printf("\nRESUMEN DEL CALCULO:\n");
	printf("Impuesto a la Propiedad: $%.2f\n", resultado.impuesto_propiedad);
	printf("Impuesto de Rodaje: $%.2f\n", resultado.impuesto_rodaje);
	printf("Tasas y servicios: $%.2f\n", 
		   resultado.tasa_sppat + resultado.tasa_ant + resultado.tasa_prefectura + 
		   resultado.valor_rtv + resultado.valor_adhesivo);
	printf("TOTAL A PAGAR: $%.2f\n", resultado.total_matricula);
	
	// Paso 4: Confirmar y procesar pago
	printf("\nPASO 4: PROCESAMIENTO DE PAGO\n");
	printf("-----------------------------\n");
	printf("Desea proceder con el pago? (S/N): ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Proceso cancelado.\n");
		pausar();
		return;
	}
	
	if (buffer[0] != 'S' && buffer[0] != 's') {
		printf("Proceso de matriculacion cancelado.\n");
		pausar();
		return;
	}
	
	// Generar comprobante de pago simple
	printf("\nGenerando comprobante de pago...\n");
	
	// Generar numero de comprobante
	char numero_comprobante[50];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(numero_comprobante, "MAT-%s-%04d%02d%02d-%03d", 
			placa, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, rand() % 1000);
	
	// Mostrar comprobante
	printf("\n=== COMPROBANTE DE MATRICULA PAGADA ===\n");
	printf("=======================================\n");
	printf("Numero de Comprobante: %s\n", numero_comprobante);
	printf("Placa: %s\n", placa);
	printf("Fecha: %02d/%02d/%04d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	printf("Total Pagado: $%.2f\n", resultado.total_matricula);
	printf("Estado: PAGADO\n");
	printf("=======================================\n");
	printf("SU VEHICULO YA PUEDE CIRCULAR LEGALMENTE\n");
	printf("=======================================\n");
	
	// Guardar en archivo de comprobantes pagados
	FILE* archivo_pagados = fopen("matriculas_pagadas.txt", "a");
	if (archivo_pagados) {
		fprintf(archivo_pagados, "%s,%s,%02d/%02d/%04d,%.2f,PAGADO\n",
				numero_comprobante, placa, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				resultado.total_matricula);
		fclose(archivo_pagados);
	}
	
	printf("\nMatriculacion completada exitosamente!\n");
	pausar();
}

/*
 * Funcion: proceso_matriculacion_final
 * Descripcion: Matricula final del vehiculo verificando que tenga pago y revision tecnica
 * Parametros: ninguno
 * Retorno: void
 */
void proceso_matriculacion_final() {
	char placa[20];
	char buffer[100];
	DatosVehiculo vehiculo;
	
	system("cls");
	printf("\n");
	printf("=======================================================\n");
	printf("           MATRICULACION FINAL DE VEHICULO\n");
	printf("           AGENCIA NACIONAL DE TRANSITO\n");
	printf("=======================================================\n");
	printf("\n");
	
	// Solicitar placa del vehiculo
	printf("Ingrese la placa del vehiculo a matricular: ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la placa del vehiculo.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Limpiar salto de linea y convertir a mayusculas
	buffer[strcspn(buffer, "\n")] = 0;
	strcpy(placa, buffer);
	convertir_a_mayusculas(placa);
	
	// Verificar que el vehiculo existe
	if (!obtener_datos_vehiculo_para_calculo_desde_archivo(placa, &vehiculo)) {
		printf("Error: Vehiculo con placa '%s' no encontrado.\n", placa);
		printf("Debe registrar el vehiculo primero.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Verificar que el vehiculo tenga pago realizado
	FILE* archivo_pagos = fopen("matriculas_pagadas.txt", "r");
	int pago_encontrado = 0;
	if (archivo_pagos) {
		char linea[200];
		while (fgets(linea, sizeof(linea), archivo_pagos)) {
			char placa_archivo[20];
			// Probar ambos formatos: nuevo (|) y viejo (,)
			if (sscanf(linea, "%*[^|]|%[^|]", placa_archivo) == 1 || 
				sscanf(linea, "%*[^,],%[^,]", placa_archivo) == 1) {
				if (strcmp(placa_archivo, placa) == 0) {
					pago_encontrado = 1;
					break;
				}
			}
		}
		fclose(archivo_pagos);
	}
	
	if (!pago_encontrado) {
		printf("Error: El vehiculo '%s' no tiene el pago de matricula registrado.\n", placa);
		printf("Debe procesar el pago primero en el modulo de pagos.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Verificar que el vehiculo tenga revision tecnica aprobada
	if (!vehiculo_tiene_revision(placa)) {
		printf("Error: El vehiculo '%s' no tiene revision tecnica aprobada.\n", placa);
		printf("Debe pasar la revision tecnica primero.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Todo esta en orden, proceder con la matriculacion final
	printf("\nVERIFICACION COMPLETADA:\n");
	printf("-------------------------------------------------------\n");
	printf("Vehiculo: %s (%s %s)\n", vehiculo.placa, vehiculo.tipo, vehiculo.subtipo);
	printf("Propietario: %s\n", vehiculo.propietario);
	printf("Pago: VERIFICADO\n");
	printf("Revision tecnica: APROBADA\n");
	printf("-------------------------------------------------------\n");
	
	printf("\nProceder con la matriculacion final? (S/N): ");
	if (!fgets(buffer, sizeof(buffer), stdin)) {
		printf("Error al leer la confirmacion.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	if (buffer[0] != 'S' && buffer[0] != 's') {
		printf("Matriculacion cancelada.\n");
		printf("\nPresione Enter para continuar...");
		getchar();
		return;
	}
	
	// Generar certificado de matriculacion
	time_t t = time(NULL);
	struct tm* fecha = localtime(&t);
	char numero_matricula[50];
	sprintf(numero_matricula, "CERT-%s-%04d%02d%02d-%03d", 
			placa, 
			fecha->tm_year + 1900,
			fecha->tm_mon + 1,
			fecha->tm_mday,
			rand() % 1000);
	
	// Mostrar certificado de matriculacion
	printf("\n");
	printf("=======================================================\n");
	printf("           CERTIFICADO DE MATRICULACION\n");
	printf("           AGENCIA NACIONAL DE TRANSITO\n");
	printf("                    ECUADOR\n");
	printf("=======================================================\n");
	printf("\n");
	printf("DATOS DEL VEHICULO:\n");
	printf("-------------------------------------------------------\n");
	printf("Numero de Certificado: %s\n", numero_matricula);
	printf("Placa: %s\n", vehiculo.placa);
	printf("Tipo: %s\n", vehiculo.tipo);
	printf("Subtipo: %s\n", vehiculo.subtipo);
	printf("Ano: %d\n", vehiculo.ano);
	printf("Avaluo: $%.2f\n", vehiculo.avaluo);
	printf("Cilindraje: %d cc\n", vehiculo.cilindraje);
	printf("\n");
	printf("DATOS DEL PROPIETARIO:\n");
	printf("-------------------------------------------------------\n");
	printf("Cedula: %s\n", vehiculo.cedula);
	printf("Nombre: %s\n", vehiculo.propietario);
	printf("\n");
	printf("CERTIFICACION:\n");
	printf("-------------------------------------------------------\n");
	printf("Fecha de matriculacion: %02d/%02d/%04d\n", 
			fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1900);
	printf("Valido hasta: %02d/%02d/%04d\n", 
			fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1901);
	printf("Estado: MATRICULADO\n");
	printf("\n");
	printf("=======================================================\n");
	printf("      SU VEHICULO ESTA LEGALMENTE MATRICULADO\n");
	printf("           PUEDE CIRCULAR SIN RESTRICCIONES\n");
	printf("=======================================================\n");
	
	// Guardar certificado en archivo
	FILE* archivo_matriculados = fopen("vehiculos_matriculados.txt", "a");
	if (archivo_matriculados) {
		fprintf(archivo_matriculados, "%s|%s|%s|%s|%s|%d|%.2f|%d|%s|%02d/%02d/%04d|MATRICULADO\n",
				numero_matricula, vehiculo.placa, vehiculo.cedula, vehiculo.propietario,
				vehiculo.tipo, vehiculo.ano, vehiculo.avaluo, vehiculo.cilindraje,
				vehiculo.subtipo, fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1900);
		fclose(archivo_matriculados);
		printf("\nCertificado guardado en archivo 'vehiculos_matriculados.txt'\n");
	} else {
		printf("\nAdvertencia: No se pudo guardar el certificado en archivo.\n");
	}
	
	printf("\nDesea guardar una copia del certificado? (S/N): ");
	if (fgets(buffer, sizeof(buffer), stdin) && (buffer[0] == 'S' || buffer[0] == 's')) {
		// Crear carpeta si no existe
		system("mkdir certificados 2>nul");
		
		char nombre_archivo[100];
		sprintf(nombre_archivo, "certificados/certificado_%s_%04d%02d%02d.txt", 
				placa, fecha->tm_year + 1900, fecha->tm_mon + 1, fecha->tm_mday);
		
		FILE* archivo_cert = fopen(nombre_archivo, "w");
		if (archivo_cert) {
			fprintf(archivo_cert, "=======================================================\n");
			fprintf(archivo_cert, "           CERTIFICADO DE MATRICULACION\n");
			fprintf(archivo_cert, "           AGENCIA NACIONAL DE TRANSITO\n");
			fprintf(archivo_cert, "                    ECUADOR\n");
			fprintf(archivo_cert, "=======================================================\n\n");
			fprintf(archivo_cert, "DATOS DEL VEHICULO:\n");
			fprintf(archivo_cert, "-------------------------------------------------------\n");
			fprintf(archivo_cert, "Numero de Certificado: %s\n", numero_matricula);
			fprintf(archivo_cert, "Placa: %s\n", vehiculo.placa);
			fprintf(archivo_cert, "Tipo: %s\n", vehiculo.tipo);
			fprintf(archivo_cert, "Subtipo: %s\n", vehiculo.subtipo);
			fprintf(archivo_cert, "Ano: %d\n", vehiculo.ano);
			fprintf(archivo_cert, "Avaluo: $%.2f\n", vehiculo.avaluo);
			fprintf(archivo_cert, "Cilindraje: %d cc\n\n", vehiculo.cilindraje);
			fprintf(archivo_cert, "DATOS DEL PROPIETARIO:\n");
			fprintf(archivo_cert, "-------------------------------------------------------\n");
			fprintf(archivo_cert, "Cedula: %s\n", vehiculo.cedula);
			fprintf(archivo_cert, "Nombre: %s\n\n", vehiculo.propietario);
			fprintf(archivo_cert, "CERTIFICACION:\n");
			fprintf(archivo_cert, "-------------------------------------------------------\n");
			fprintf(archivo_cert, "Fecha de matriculacion: %02d/%02d/%04d\n", 
					fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1900);
			fprintf(archivo_cert, "Valido hasta: %02d/%02d/%04d\n", 
					fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1901);
			fprintf(archivo_cert, "Estado: MATRICULADO\n\n");
			fprintf(archivo_cert, "=======================================================\n");
			fprintf(archivo_cert, "      SU VEHICULO ESTA LEGALMENTE MATRICULADO\n");
			fprintf(archivo_cert, "           PUEDE CIRCULAR SIN RESTRICCIONES\n");
			fprintf(archivo_cert, "=======================================================\n");
			
			fclose(archivo_cert);
			printf("Certificado guardado en '%s'\n", nombre_archivo);
		}
	}
	
	printf("\nMatriculacion final completada exitosamente!\n");
	printf("\nPresione Enter para continuar...");
	getchar();
}
