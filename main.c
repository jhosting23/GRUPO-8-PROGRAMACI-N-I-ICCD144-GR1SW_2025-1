#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h> //Le dice al programa que use la configuracion local del sistema 
#include "vehiculos.h"  // Incluimos nuestra libreria 


#define ARCHIVO_USUARIOS "usuarios.txt"
#define MAX_LINEA 100
#define MAX_INTENTOS 3

// FUNCIONES DEL SISTEMA DE AUTENTICACI�N 
void limpiar_pantalla() {
	system("cls");
}
void pausar() {
	printf("\nPresione Enter para continuar...");
	while(getchar() != '\n');
	getchar();
}
int usuario_existe(const char* usuario) {
	FILE* file = fopen(ARCHIVO_USUARIOS, "r");
	if (!file) return 0;
	
	char linea[MAX_LINEA];
	while (fgets(linea, sizeof(linea), file)) {
		char nombre_archivo[50];
		sscanf(linea, "%49[^,]", nombre_archivo);
		if (strcmp(nombre_archivo, usuario) == 0) {
			fclose(file);
			return 1;
		}
	}
	fclose(file);
	return 0;
}

int registrar_usuario() {
	char usuario[50], contrasena[50];
	
	limpiar_pantalla();
	printf("=== Registro de Usuario ===\n");
	printf("Ingrese nombre de usuario: ");
	scanf("%49s", usuario);
	
	if (usuario_existe(usuario)) {
		printf("Error: El usuario ya existe.\n");
		pausar();
		return 0;
	}
	
	printf("Ingrese contrase�a: ");
	scanf("%49s", contrasena);
	
	FILE* file = fopen(ARCHIVO_USUARIOS, "a");
	if (!file) {
		perror("No se pudo abrir el archivo de usuarios");
		pausar();
		return 0;
	}
	
	fprintf(file, "%s,%s\n", usuario, contrasena);
	fclose(file);
	printf("Usuario registrado con exito.\n");
	pausar();
	return 1;
}

int validar_credenciales(const char* usuario, const char* contrasena) {
	FILE* file = fopen(ARCHIVO_USUARIOS, "r");
	if (!file) {
		perror("No se pudo abrir el archivo de usuarios");
		return 0;
	}
	
	char linea[MAX_LINEA];
	while (fgets(linea, sizeof(linea), file)) {
		char usuar_Tamano[50], contra_Tamano[50];
		sscanf(linea, "%49[^,],%49[^\n]", usuar_Tamano, contra_Tamano);
		if (strcmp(usuar_Tamano, usuario) == 0 && strcmp(contra_Tamano, contrasena) == 0) {
			fclose(file);
			return 1;
		}
	}
	
	fclose(file);
	return 0;
}

int iniciar_sesion() {
	char usuario[50], contrasena[50];
	int intentos = 0;
	
	limpiar_pantalla();
	printf("=== Inicio de Sesi�n ===\n");
	
	while (intentos < MAX_INTENTOS) {
		printf("Usuario: ");
		scanf("%49s", usuario);
		printf("Contrase�a: ");
		scanf("%49s", contrasena);
		
		if (validar_credenciales(usuario, contrasena)) {
			limpiar_pantalla();
			printf("==============================================\n");
			printf("        BIENVENIDO AL SISTEMA\n");
			printf("   Sistema de Matriculacion Vehicular\n");
			printf("==============================================\n");
			printf("        Usuario: %s\n", usuario);
			printf("==============================================\n\n");
			printf("Acceso concedido exitosamente.\n");
			pausar();
			return 1;
		} else {
			printf("Credenciales incorrectas. Intento %d de %d.\n", ++intentos, MAX_INTENTOS);
			if (intentos < MAX_INTENTOS) {
				printf("Intente nuevamente:\n");
			}
		}
	}
	
	printf("Demasiados intentos fallidos. Intente m�s tarde.\n");
	pausar();
	return 0;
}

// MEN� DEL SISTEMA DE VEH�CULOS 
void menu_vehiculos() {
	int opcion;
	
	do {
		limpiar_pantalla();
		printf("=== SISTEMA DE MATRICULACION VEHICULAR ===\n");
		printf("1. Registrar nuevo vehiculo\n");
		printf("2. Buscar vehiculo por placa\n");
		printf("3. Calcular matricula (Pr�ximamente)\n");
		printf("0. Volver al menu principal\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		
		switch (opcion) {
		case 1:
			limpiar_pantalla();
			registrar_vehiculo();  // Funci�n de nuestra librer�a
			pausar();
			break;
			
		case 2:
			limpiar_pantalla();
			buscar_vehiculo();  // Funci�n de nuestra librer�a
			pausar();
			break;
			
		case 3:
			limpiar_pantalla();
			printf("=== CALCULO DE MATRICULA ===\n");
			printf("Usaremos los datos del archivo vehiculos.txt\n");
			pausar();
			break;
			
		case 0:
			printf("Volviendo al menu principal...\n");
			break;
			
		default:
			printf("Opcion invalida.\n");
			pausar();
		}
	} while (opcion != 0);
}

// FUNCI�N MAIN PRINCIPAL
int main() {
	setlocale(LC_ALL, "");
	int opcion;
	
	printf("=== SISTEMA DE MATRICULACION VEHICULAR ===\n");
	printf("=========================================\n\n");
	
	do {
		limpiar_pantalla();
		printf("=== Sistema de Matriculacion Vehicular ===\n");
		printf("1. Registrar nuevo usuario\n");
		printf("2. Iniciar sesion\n");
		printf("0. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		
		switch (opcion) {
		case 1:
			registrar_usuario();
			break;
			
		case 2:
			if (iniciar_sesion()) {
				// Llamar al men� de veh�culos usando nuestra librer�a
				menu_vehiculos();
			}
			break;
			
		case 0:
			limpiar_pantalla();
			printf("=== GRACIAS POR USAR EL SISTEMA ===\n");
			printf("Sistema de Matriculacion Vehicular\n");
			printf("�Hasta luego!\n");
			printf("==================================\n");
			break;
			
		default:
			printf("Opci�n inv�lida.\n");
			pausar();
		}
	} while (opcion != 0);
	
	return 0;
}
