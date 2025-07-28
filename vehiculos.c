// vehiculos.c - funciones para validar las placas y datos del vehiculo 
// Creada por: mathias , jhostin , christian 

#include "vehiculos.h"

// Funci�n para limpiar el buffer
void limpiar_entrada() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// Validar la palaca con su debido formato 
int validar_placa(char* placa) {
	// Verificar longitud
	if (strlen(placa) != 8) {
		return 0;  // No v�lida
	}
	
	// Verificar que tenga el formato correcto
	// Primeras 3 letras
	for (int i = 0; i < 3; i++) {
		if (placa[i] < 'A' || placa[i] > 'Z') {
			return 0;
		}
	}
	
	// Gui�n en posici�n 3
	if (placa[3] != '-') {
		return 0;
	}
	
	// �ltimos 4 n�meros
	for (int i = 4; i < 8; i++) {
		if (placa[i] < '0' || placa[i] > '9') {
			return 0;
		}
	}
	
	return 1;  // Es v�lida
}

// Validar c�dula solo que tenga 10 n�meros
int validar_cedula(char* cedula) {
	if (strlen(cedula) != 10) {
		return 0;
	}
	
	// Verificar que todos sean n�meros
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
		return 0;  // Fuera del rango v�lido
	}
	return 1;  // Es v�lido
}

// Verificar si el veh�culo ya est� registrado
int vehiculo_ya_existe(char* placa) {
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		return 0;  // Si no existe el archivo, no hay veh�culos
	}
	
	char linea[MAX_LINEA2];
	char placa_archivo[10];
	
	while (fgets(linea, sizeof(linea), archivo)) {
		sscanf(linea, "%9[^,]", placa_archivo);
		if (strcmp(placa_archivo, placa) == 0) {
			fclose(archivo);
			return 1;  // Ya existe
		}
	}
	
	fclose(archivo);
	return 0;  // No existe
}

// Funci�n principal para registrar un veh�culo
int registrar_vehiculo() {
	char placa[10];
	char cedula[15];
	char nombre[50];
	char tipo[20];
	int anio;
	float valor;
	int cilindraje;
	
	printf("\n=== REGISTRO DE VEHICULO ===\n");
	
	// Pedir placa con validaci�n en bucle
	do {
		printf("Ingrese la placa (formato ABC-1234): ");
		scanf("%s", placa);
		
		// Convertir a may�sculas
		for (int i = 0; i < strlen(placa); i++) {
			if (placa[i] >= 'a' && placa[i] <= 'z') {
				placa[i] = placa[i] - 32;  // Convertir a may�scula mediante la tabal ASCII
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
			printf("ERROR: Este vehiculo ya est� registrado.\n");
			printf("Intente con otra placa.\n\n");
			continue;
		}
		
		break; // Placa v�lida, salir del bucle
	} while (1);
	
	// Pedir c�dula con un blucle 
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
		
		break; // C�dula v�lida, salir del bucle
	} while (1);
	
	// Pedir nombre
	printf("Ingrese nombre del propietario: ");
	limpiar_entrada();
	fgets(nombre, sizeof(nombre), stdin);
	nombre[strlen(nombre)-1] = '\0';  // Quitar el \n
	
	// Pedir tipo de veh�culo con un bucle
	do {
		printf("Ingrese tipo (PARTICULAR/COMERCIAL): ");
		scanf("%s", tipo);
		
		// Convertir a may�sculas
		for (int i = 0; i < strlen(tipo); i++) {
			if (tipo[i] >= 'a' && tipo[i] <= 'z') {
				tipo[i] = tipo[i] - 32;
			}
		}
		
		// Validar tipo b�sico
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
		
		break; // Tipo v�lido, salir del bucle
	} while (1);
	
	// Pedir a�o con validaci�n en bucle
	do {
		printf("Ingrese a�o del vehiculo: ");
		scanf("%d", &anio);
		
		if (anio < 1990 || anio > 2025) {
			system("cls");
			printf("\n=== REGISTRO DE VEHICULO ===\n");
			printf("Placa: %s (Validada)\n", placa);
			printf("Cedula: %s (Validada)\n", cedula);
			printf("Nombre: %s\n", nombre);
			printf("Tipo: %s (Validado)\n", tipo);
			printf("ERROR: A�o debe estar entre 1990 y 2025\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // A�o v�lido, salir del bucle
	} while (1);
	
	// Pedir valor con validaci�n en bucle
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
			printf("A�o: %d (Validado)\n", anio);
			printf("ERROR: El valor debe ser mayor a 0\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Valor v�lido, salir del bucle
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
			printf("A�o: %d (Validado)\n", anio);
			printf("Valor: $%.2f (Validado)\n", valor);
			printf("ERROR: El cilindraje debe estar entre 50 y 8000 cc\n");
			printf("Intente nuevamente.\n\n");
			continue;
		}
		
		break; // Cilindraje v�lido, salir del bucle
	} while (1);
	
	// Guardar en archivo
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "a");
	if (archivo == NULL) {
		printf("ERROR: No se pudo abrir el archivo\n");
		return 0;
	}
	
	// Escribir los datos seprados con las comas 
	fprintf(archivo, "%s,%s,%s,%s,%d,%.2f,%d\n", 
			placa, cedula, nombre, tipo, anio, valor, cilindraje);
	
	fclose(archivo);
	
	printf("\n�Vehiculo registrado exitosamente!\n");
	printf("Placa: %s\n", placa);
	printf("Propietario: %s\n", nombre);
	printf("Tipo: %s\n", tipo);
	printf("Cilindraje: %d cc\n", cilindraje);
	
	return 1;
}

// Funci�n para buscar un veh�culo por placa
int buscar_vehiculo() {
	char placa_buscar[10];
	char linea[MAX_LINEA2];
	char placa[10], cedula[15], nombre[50], tipo[20];
	int anio, cilindraje;
	float valor;
	
	do {
		printf("Ingrese la placa a buscar: ");
		scanf("%s", placa_buscar);
		
		// Convertir a may�sculas
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
		
		break; // Placa v�lida, salir del bucle
	} while (1);
	
	FILE* archivo = fopen(ARCHIVO_VEHICULOS, "r");
	if (archivo == NULL) {
		printf("No hay vehiculos registrados.\n");
		return 0;
	}
	
	// Buscar en el archivo
	while (fgets(linea, sizeof(linea), archivo)) {
		sscanf(linea, "%9[^,],%14[^,],%49[^,],%19[^,],%d,%f,%d",
			   placa, cedula, nombre, tipo, &anio, &valor, &cilindraje);
		
		if (strcmp(placa, placa_buscar) == 0) {
			printf("\n=== VEHICULO ENCONTRADO ===\n");
			printf("Placa: %s\n", placa);
			printf("Propietario: %s\n", nombre);
			printf("Cedula: %s\n", cedula);
			printf("Tipo: %s\n", tipo);
			printf("A�o: %d\n", anio);
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
