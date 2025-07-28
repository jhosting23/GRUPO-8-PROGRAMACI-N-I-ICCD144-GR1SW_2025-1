// vehiculos.c - Funciones para validar las placas y datos del vehiculo
// Creada por: Mathias, Jhostin, Christian
// Fecha: 2025

#include "vehiculos.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>  

// --- Funciones auxiliares ---
void limpiar_pantalla(void) {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void convertir_a_mayusculas(char* cadena) {
	for (int i = 0; cadena[i]; i++) {
		cadena[i] = toupper((unsigned char)cadena[i]);
	}
}

// --- Funciones de Validaci?n ---
int validar_placa(const char* placa) {
	if (strlen(placa) != 8) return 0;
	for (int i = 0; i < 3; i++) if (!isupper((unsigned char)placa[i])) return 0;
	if (placa[3] != '-') return 0;
	for (int i = 4; i < 8; i++) if (!isdigit((unsigned char)placa[i])) return 0;
	return 1;
}

int validar_cedula(const char* cedula) {
	if (strlen(cedula) != 10) return 0;
	for (int i = 0; i < 10; i++) if (!isdigit((unsigned char)cedula[i])) return 0;
	return 1;
}

int validar_cilindraje(int cilindraje) {
	return (cilindraje >= 50 && cilindraje <= 8000);
}

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

int validar_nombre(const char* nombre) {
	if (strlen(nombre) < 3) return 0;
	for (int i = 0; nombre[i]; i++) {
		if (!isalpha((unsigned char)nombre[i]) && !isspace((unsigned char)nombre[i])) {
			return 0;
		}
	}
	return 1;
}

int validar_valor(float valor) {
	return (valor >= MIN_AVALUO && valor <= MAX_AVALUO);
}


// --- Funci?n Principal de Registro (Con Interfaz Mejorada) ---
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
				printf("\nERROR: Formato de placa incorrecto. Debe ser ABC-1234.\nPresione Enter para reintentar...");
				getchar();
				strcpy(placa, ""); // Limpiar dato incorrecto
			} else if (vehiculo_ya_existe(placa)) {
				printf("\nERROR: Un vehiculo con la placa '%s' ya esta registrado.\nPresione Enter para reintentar...", placa);
				getchar();
				strcpy(placa, "");
			} else { paso_actual++; }
			break;
		case 2: // C?dula
			printf("Ingrese la cedula (10 digitos): ");
			if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%14s", cedula); else continue;
			if (!validar_cedula(cedula)) {
				printf("\nERROR: La cedula debe tener 10 numeros.\nPresione Enter para reintentar...");
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
			printf("Ingrese tipo (PARTICULAR / COMERCIAL): ");
			if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%19s", tipo); else continue;
			convertir_a_mayusculas(tipo);
			if (strcmp(tipo, "PARTICULAR") == 0) {
				printf("   - Es motocicleta? (SI / NO): ");
				if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%4s", buffer); else { strcpy(tipo, ""); continue; }
				convertir_a_mayusculas(buffer);
				if(strcmp(buffer, "SI") == 0) strcpy(subtipo, "MOTOCICLETA");
				else if (strcmp(buffer, "NO") == 0) {
					printf("   - Ingrese subtipo (LIVIANO / PESADO): ");
					if (fgets(buffer, sizeof(buffer), stdin)) sscanf(buffer, "%19s", subtipo); else { strcpy(tipo, ""); continue; }
					convertir_a_mayusculas(subtipo);
					if(strcmp(subtipo, "LIVIANO") != 0 && strcmp(subtipo, "PESADO") != 0) strcpy(tipo, ""); // Error
				} else strcpy(tipo, ""); // Error
			} else if (strcmp(tipo, "COMERCIAL") == 0) {
				strcpy(subtipo, "COMERCIAL");
			} else strcpy(tipo, ""); // Error
			
			if(strlen(tipo) == 0) {
				printf("\nERROR: Tipo o subtipo invalido.\nPresione Enter para reintentar...");
				getchar();
			} else { paso_actual++; }
			break;
		case 5: // A?o
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


// --- Otras Funciones ---
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
			strcpy(vehiculo_data->tipo, tipo_str);
			strcpy(vehiculo_data->subtipo, subtipo_str);
			vehiculo_data->avaluo = avaluo;
			vehiculo_data->cilindraje = cilindraje;
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	return 0;
}
