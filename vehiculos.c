// vehiculos.c - funciones para validar las placas y datos del vehiculo
// Creada por: mathias, jhostin, christian

#include "vehiculos.h" // Incluye vehiculos.h que a su vez incluye matricula.h
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para atoi, atof

// Función para limpiar el buffer
void limpiar_entrada() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// Validar la placa con su debido formato
int validar_placa(char* placa) {
	// Verificar longitud
	if (strlen(placa) != 8) {
		return 0; // No válida
	}
	
	// Verificar que tenga el formato correcto
	// Primeras 3 letras
	for (int i = 0; i < 3; i++) {
		if (placa[i] < 'A' || placa[i] > 'Z') {
			return 0;
		}
	}
	
	// Guión en posición 3
	if (placa[3] != '-') {
		return 0;
	}
	
	// Últimos 4 números
	for (int i = 4; i < 8; i++) {
		if (placa[i] < '0' || placa[i] > '9') {
			return 0;
		}
	}
	
	return 1; // Es válida
}

// Validar cédula solo que tenga 10 números
int validar_cedula(char* cedula) {
	if (strlen(cedula) != 10) {
		return 0;
	}
	
	// Verificar que todos sean números
	for (int i = 0; i < 10; i++) {
		if (cedula[i] < '0' || cedula[i] > '9') {
			return 0;
		}
	}
	
	return 1;
}

// Validar cilindraje del motor
int validar_cilindraje(int cilindraje) {
	// Rangos de cilindraje en cc
	if (cilindraje < 50 || cilindraje > 8000) {
		return 0; // Fuera del rango válido
	}
	return 1; // Es válido
}

// Verificar si el vehículo ya está registrado
int vehiculo_ya_existe(char* placa) {
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		return 0; // Si no existe el archivo, no hay vehículos
	}
	
	char linea[MAX_LINEA2];
	char placa_archivo[10];
	
	while (fgets(linea, sizeof(linea), archivo)) {
		// Asumiendo que la placa es el primer campo en la línea
		sscanf(linea, "%9[^,]", placa_archivo);
		if (strcmp(placa_archivo, placa) == 0) {
			fclose(archivo);
			return 1; // Ya existe
		}
	}
	
	fclose(archivo);
	return 0; // No existe
}

// Función principal para registrar un vehículo
int registrar_vehiculo() {
	char placa[10];
	char cedula[15];
	char nombre[50];
	char tipo[20];
	char subtipo[20];
	int anio;
	float valor;
	int cilindraje;
	
	printf("\n=== REGISTRO DE VEHICULO ===\n");
	
	// Pedir placa con validación en bucle
	do {
		printf("Ingrese la placa (formato ABC-1234): ");
		scanf("%s", placa);
		
		// Convertir a mayúsculas
		for (int i = 0; i < strlen(placa); i++) {
			if (placa[i] >= 'a' && placa[i] <= 'z') {
				placa[i] = placa[i] - 32; // Convertir a mayúscula mediante la tabal ASCII
			}
		}
		
		// Validar placa
		if (!validar_placa(placa)) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("ERROR: Formato de placa incorrecto. Debe ser ABC-1234\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		// Verificar si ya existe
		if (vehiculo_ya_existe(placa)) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("ERROR: Este vehiculo ya esta registrado.\n");
			printf("Intente con otra placa.\n\n");
			continue;
		}
		
		break; // Placa válida
	} while (1);
	
	// Pedir cédula con un bucle
	do {
		printf("Ingrese cedula del propietario (10 digitos): ");
		scanf("%s", cedula);
		
		if (!validar_cedula(cedula)) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("Placa: %s (Validada correctamente)\n", placa);
			printf("ERROR: La cedula debe tener 10 numeros.\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Cédula válida
	} while (1);
	
	// Pedir nombre
	printf("Ingrese nombre del propietario: ");
	limpiar_entrada(); // Limpiar el buffer antes de fgets
	fgets(nombre, sizeof(nombre), stdin);
	nombre[strcspn(nombre, "\n")] = '\0'; // Quitar el \n de fgets de forma segura
	
	// Pedir tipo de vehículo con un bucle
	do {
		printf("Ingrese tipo (PARTICULAR/COMERCIAL): ");
		scanf("%s", tipo);
		
		// Convertir a mayúsculas
		for (int i = 0; i < strlen(tipo); i++) {
			if (tipo[i] >= 'a' && tipo[i] <= 'z') {
				tipo[i] = tipo[i] - 32;
			}
		}
		
		// Validar
		if (strcmp(tipo, "PARTICULAR") != 0 && strcmp(tipo, "COMERCIAL") != 0) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("Placa: %s (Validada)\n", placa);
			printf("Cedula: %s (Validada)\n", cedula);
			printf("Nombre: %s\n", nombre);
			printf("ERROR: Tipo debe ser PARTICULAR o COMERCIAL\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; 
	} while (1);
	
	// Pedir subtipo según el tipo principal
	if (strcmp(tipo, "PARTICULAR") == 0) {
		// Si es PARTICULAR, preguntar si es motocicleta
		do {
			printf("¿Es motocicleta? (SI/NO): ");
			char respuesta[5];
			scanf("%s", respuesta);
			
			// Convertir a mayúsculas
			for (int i = 0; i < strlen(respuesta); i++) {
				if (respuesta[i] >= 'a' && respuesta[i] <= 'z') {
					respuesta[i] = respuesta[i] - 32;
				}
			}
			
			if (strcmp(respuesta, "SI") == 0) {
				strcpy(subtipo, "MOTOCICLETA");
				break;
			} else if (strcmp(respuesta, "NO") == 0) {
				// Si no es motocicleta, preguntar si es liviano o pesado
				do {
					printf("Ingrese subtipo (LIVIANO/PESADO): ");
					scanf("%s", subtipo);
					
					// Convertir a mayúsculas
					for (int i = 0; i < strlen(subtipo); i++) {
						if (subtipo[i] >= 'a' && subtipo[i] <= 'z') {
							subtipo[i] = subtipo[i] - 32;
						}
					}
					
					if (strcmp(subtipo, "LIVIANO") == 0 || strcmp(subtipo, "PESADO") == 0) {
						break; // Subtipo válido
					} else {
						system("cls");
						printf("\n=== REGISTRO DE VEHICULO ===\n");
						printf("Placa: %s (Validada)\n", placa);
						printf("Cedula: %s (Validada)\n", cedula);
						printf("Nombre: %s\n", nombre);
						printf("Tipo: %s (Validado)\n", tipo);
						printf("ERROR: Subtipo debe ser LIVIANO o PESADO\n");
						printf("Intente nuevamente.\n\n");
					}
				} while (1);
				break;
			} else {
				system("cls");
				printf("\n=== REGISTRO DE VEHICULO ===\n");
				printf("Placa: %s (Validada)\n", placa);
				printf("Cedula: %s (Validada)\n", cedula);
				printf("Nombre: %s\n", nombre);
				printf("Tipo: %s (Validado)\n", tipo);
				printf("ERROR: Respuesta debe ser SI o NO\n");
				printf("Intente nuevamente.\n\n");
			}
		} while (1);
	} else {
		// Si es COMERCIAL, no pregunta subtipo, asigna valor por defecto
		strcpy(subtipo, "COMERCIAL");
	}
	
	// Pedir año con validación en bucle
	do {
		printf("Ingrese año del vehiculo: ");
		scanf("%d", &anio);
		
		if (anio < 1990 || anio > 2025) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("Placa: %s (Validada)\n", placa);
			printf("Cedula: %s (Validada)\n", cedula);
			printf("Nombre: %s\n", nombre);
			printf("Tipo: %s (Validado)\n", tipo);
			printf("Subtipo: %s (Validado)\n", subtipo);
			printf("ERROR: Año debe estar entre 1990 y %d\n", ANO_FISCAL); // Usamos ANO_FISCAL
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Año válido, salir del bucle
	} while (1);
	
	// Pedir valor con validación en bucle
	do {
		printf("Ingrese valor del vehiculo: ");
		scanf("%f", &valor);
		
		if (valor <= 0) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("Placa: %s (Validada)\n", placa);
			printf("Cedula: %s (Validada)\n", cedula);
			printf("Nombre: %s\n", nombre);
			printf("Tipo: %s (Validado)\n", tipo);
			printf("Subtipo: %s (Validado)\n", subtipo);
			printf("Año: %d (Validado)\n", anio);
			printf("ERROR: El valor debe ser mayor a 0\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Valor válido
	} while (1);
	
	// Pedir cilindraje del motor un bucle
	do {
		printf("Ingrese cilindraje del motor (en cc): ");
		scanf("%d", &cilindraje);
		
		if (!validar_cilindraje(cilindraje)) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("Placa: %s (Validada)\n", placa);
			printf("Cedula: %s (Validada)\n", cedula);
			printf("Nombre: %s\n", nombre);
			printf("Tipo: %s (Validado)\n", tipo);
			printf("Subtipo: %s (Validado)\n", subtipo);
			printf("Año: %d (Validado)\n", anio);
			printf("Valor: $%.2f (Validado)\n", valor);
			printf("ERROR: El cilindraje debe estar entre 50 y 8000 cc\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Cilindraje válido, salir del bucle
	} while (1);
	
	// Guardar en archivo
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "a");
	if (archivo == NULL) {
		printf("ERROR: No se pudo abrir el archivo %s para escritura.\n", ARCHIVO_VEHICULOS);
		return 0;
	}
	
	// Escribir los datos separados con las comas (agregando subtipo)
	fprintf(archivo, "%s,%s,%s,%s,%s,%d,%.2f,%d\n",
			placa, cedula, nombre, tipo, subtipo, anio, valor, cilindraje);
	
	fclose(archivo);
	
	printf("\n¡Vehiculo registrado exitosamente!\n");
	printf("Placa: %s\n", placa);
	printf("Propietario: %s\n", nombre);
	printf("Tipo: %s\n", tipo);
	printf("Subtipo: %s\n", subtipo);
	printf("Cilindraje: %d cc\n", cilindraje);
	
	return 1;
}

// Función para buscar un vehículo por placa
int buscar_vehiculo() {
	char placa_buscar[10];
	char linea[MAX_LINEA2];
	char placa[10], cedula[15], nombre[50], tipo[20], subtipo[20];
	int anio, cilindraje;
	float valor;
	
	do {
		printf("Ingrese la placa a buscar: ");
		scanf("%s", placa_buscar);
		
		// Convertir a mayúsculas
		for (int i = 0; i < strlen(placa_buscar); i++) {
			if (placa_buscar[i] >= 'a' && placa_buscar[i] <= 'z') {
				placa_buscar[i] = placa_buscar[i] - 32;
			}
		}
		
		// Validar formato de placa
		if (!validar_placa(placa_buscar)) {
			system("cls");
			printf("=== BUSCAR VEHICULO ===\n");
			printf("ERROR: Formato de placa incorrecto. Debe ser ABC-1234\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Placa válida, salir del bucle
	} while (1);
	
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		printf("No hay vehiculos registrados o no se pudo abrir el archivo %s.\n", ARCHIVO_VEHICULOS);
		return 0;
	}
	
	// El formato de lectura debe coincidir con el formato de escritura en registrar_vehiculo
	while (fgets(linea, sizeof(linea), archivo)) {
		// Formato: PLACA,CEDULA,NOMBRE,TIPO,SUBTIPO,ANIO,VALOR,CILINDRAJE
		int items_leidos = sscanf(linea, "%9[^,],%14[^,],%49[^,],%19[^,],%19[^,],%d,%f,%d",
								  placa, cedula, nombre, tipo, subtipo, &anio, &valor, &cilindraje);
		
		if (items_leidos == 8 && strcmp(placa, placa_buscar) == 0) {
			printf("\n=== VEHICULO ENCONTRADO ===\n");
			printf("Placa: %s\n", placa);
			printf("Propietario: %s\n", nombre);
			printf("Cedula: %s\n", cedula);
			printf("Tipo: %s\n", tipo);
			printf("Subtipo: %s\n", subtipo);
			printf("Año: %d\n", anio);
			printf("Valor: $%.2f\n", valor);
			printf("Cilindraje: %d cc\n", cilindraje);
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	printf("Vehiculo con placa %s no encontrado.\n", placa_buscar);
	return 0;
}
// Esta función llena la estructura DatosVehiculo que es esperada por matricula.c
int obtener_datos_vehiculo_para_calculo_desde_archivo(const char* placa_buscada, DatosVehiculo* vehiculo_data) {
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		printf("Error: No se pudo abrir el archivo de vehiculos en '%s'.\n", ARCHIVO_VEHICULOS);
		return 0;
	}
	char linea[MAX_LINEA2];
	char placa_leida[10];
	char cedula[15]; // No se usan pero se leen
	char nombre[50]; // No se usan pero se leen
	char tipo_str[20];
	char subtipo_str[20];
	int anio;      // No se usan pero se leen 
	float avaluo;
	int cilindraje;
	
	while (fgets(linea, sizeof(linea), archivo)) {
		// Formato:PLACA,CEDULA,NOMBRE,TIPO,SUBTIPO,ANIO,AVALUO,CILINDRAJE
		int items_leidos = sscanf(linea, "%9[^,],%14[^,],%49[^,],%19[^,],%19[^,],%d,%f,%d",
								  placa_leida, cedula, nombre, tipo_str, subtipo_str,
								  &anio, &avaluo, &cilindraje);
		
		if (items_leidos == 8) { // Verifica que se hayan leído todos los 8 campos esperados
			if (strcmp(placa_leida, placa_buscada) == 0) {
				// Si la placa coincide, copiar los datos relevantes a la estructura DatosVehiculo
				strcpy(vehiculo_data->placa, placa_leida);
				strcpy(vehiculo_data->tipo, tipo_str);
				strcpy(vehiculo_data->subtipo, subtipo_str);
				vehiculo_data->avaluo = avaluo;
				vehiculo_data->cilindraje = cilindraje;
				// Los campos tiene_multas, valor_multas y meses_retraso se piden al usuario en menu_calculo_matricula
				fclose(archivo);
				return 1; // Vehículo encontrado
			}
		}
	}
	
	fclose(archivo);
	return 0; // Vehículo no encontrado
}
