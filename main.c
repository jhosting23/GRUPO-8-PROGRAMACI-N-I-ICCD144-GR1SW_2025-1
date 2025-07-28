// main.c - Archivo principal del Sistema de Matriculaci?n Vehicular
// Creada por: Mathias, Jhostin, Christian
// Fecha: 2025
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

// Incluimos nuestras libreras
#include "vehiculos.h"
#include "matricula.h"

// Constantes y definiciones
#define ARCHIVO_USUARIOS "usuarios.txt"
#define MAX_LINEA 100
#define MAX_INTENTOS 3

// Incluir las librer?as necesarias para ocultar la contrase?a.
#ifdef _WIN32
#include <conio.h> // Para getch() en Windows
#else
#include <termios.h> // Para configurar la terminal en Linux/macOS
#include <unistd.h>  // Para STDIN_FILENO
#endif

// --- Prototipos de funciones definidas en este archivo ---
void pausar();
void obtener_contrasena_oculta(char* buffer, int tamano);
int usuario_existe(const char* usuario);
int registrar_usuario();
int validar_credenciales(const char* usuario, const char* contrasena);
int iniciar_sesion();
void menu_vehiculos();
void mostrar_tarifas_vigentes();


// --- Funciones de Utilidad ---

// Funci?n multiplataforma para leer una contrase?a y mostrar asteriscos.
void obtener_contrasena_oculta(char* buffer, int tamano) {
	int i = 0;
	char c;
	
#ifdef _WIN32
	// M?todo para Windows usando conio.h
	while (i < tamano - 1) {
		c = getch();
		if (c == '\r' || c == '\n') { // Tecla Enter
			break;
		} else if (c == '\b') { // Tecla de retroceso (backspace)
			if (i > 0) {
				i--;
				printf("\b \b"); // Mueve el cursor hacia atr?s, escribe un espacio y vuelve atr?s
			}
		} else {
			buffer[i++] = c;
			printf("*");
		}
	}
#else
	//  Linux/macOS usando termios.h
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt); // Obtener configuraci?n actual de la terminal
	newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON); // Desactivar eco y modo can?nico
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Aplicar nueva configuraci?n
	
	while (i < tamano - 1) {
		c = getchar();
		if (c == '\n') { // Tecla Enter
			break;
		} else if (c == 127) { // Tecla de retroceso (backspace)
			if (i > 0) {
				i--;
				printf("\b \b");
			}
		} else {
			buffer[i++] = c;
			printf("*");
		}
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaurar configuraci?n original
#endif
	
	buffer[i] = '\0'; // Terminar la cadena
	printf("\n"); // Nueva l?nea despu?s de presionar Enter
}

void pausar() {
	printf("\nPresione Enter para continuar...");
	getchar(); // Espera una pulsaci?n de tecla
}


// --- Funciones de Autenticaci?n ---

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
	char buffer[100];
	
	limpiar_pantalla();
	printf("=== Registro de Nuevo Usuario ===\n");
	
	printf("Ingrese nombre de usuario: ");
	if (fgets(buffer, sizeof(buffer), stdin)) {
		sscanf(buffer, "%49s", usuario);
	}
	
	if (usuario_existe(usuario)) {
		printf("\nERROR: El usuario '%s' ya existe.\n", usuario);
		pausar();
		return 0;
	}
	
	printf("Ingrese contrasena: ");
	obtener_contrasena_oculta(contrasena, 50);
	
	FILE* file = fopen(ARCHIVO_USUARIOS, "a");
	if (!file) {
		perror("No se pudo abrir el archivo de usuarios");
		pausar();
		return 0;
	}
	
	fprintf(file, "%s,%s\n", usuario, contrasena);
	fclose(file);
	printf("\nUsuario registrado con exito.\n");
	pausar();
	return 1;
}

int validar_credenciales(const char* usuario, const char* contrasena) {
	FILE* file = fopen(ARCHIVO_USUARIOS, "r");
	if (!file) return 0;
	
	char linea[MAX_LINEA];
	while (fgets(linea, sizeof(linea), file)) {
		char usuar_tamano[50], contra_tamano[50];
		// Asegurarse de quitar el \n de la contrase?a le?da del archivo
		sscanf(linea, "%49[^,],%49[^\n]", usuar_tamano, contra_tamano);
		if (strcmp(usuar_tamano, usuario) == 0 && strcmp(contra_tamano, contrasena) == 0) {
			fclose(file);
			return 1;
		}
	}
	fclose(file);
	return 0;
}

int iniciar_sesion() {
	char usuario[50], contrasena[50];
	char buffer[100];
	int intentos = 0;
	
	while (intentos < MAX_INTENTOS) {
		limpiar_pantalla();
		printf("=== Inicio de Sesion ===\n");
		printf("Usuario: ");
		if (fgets(buffer, sizeof(buffer), stdin)) {
			sscanf(buffer, "%49s", usuario);
		}
		
		printf("Contrasena: ");
		obtener_contrasena_oculta(contrasena, 50);
		
		if (validar_credenciales(usuario, contrasena)) {
			limpiar_pantalla();
			printf("==============================================\n");
			printf("        BIENVENIDO AL SISTEMA\n");
			printf("    Sistema de Matriculacion Vehicular\n");
			printf("==============================================\n");
			printf("        Usuario: %s\n", usuario);
			printf("==============================================\n\n");
			printf("Acceso concedido exitosamente.\n");
			pausar();
			return 1;
		} else {
			printf("\nCredenciales incorrectas. Intento %d de %d.\n", ++intentos, MAX_INTENTOS);
			pausar();
		}
	}
	
	printf("\nDemasiados intentos fallidos. El programa se cerrara.\n");
	pausar();
	return 0;
}


// --- Men?s del Sistema ---

void menu_vehiculos() {
	int opcion;
	char buffer[10];
	
	do {
		limpiar_pantalla();
		printf("=== MENU PRINCIPAL ===\n");
		printf("1. Registrar nuevo vehiculo\n");
		printf("2. Buscar vehiculo por placa\n");
		printf("3. Calcular matricula vehicular\n");
		printf("4. Mostrar tarifas vigentes\n");
		printf("0. Cerrar Sesion y Volver al Menu de Inicio\n");
		printf("--------------------------------------------\n");
		printf("Seleccione una opcion: ");
		
		if (fgets(buffer, sizeof(buffer), stdin)) {
			if (sscanf(buffer, "%d", &opcion) != 1) opcion = -1; // Opci?n inv?lida si no es n?mero
		} else {
			opcion = -1;
		}
		
		switch (opcion) {
		case 1: registrar_vehiculo(); break;
		case 2: buscar_vehiculo(); pausar(); break;
		case 3: menu_calculo_matricula(); pausar(); break;
		case 4: mostrar_tarifas_vigentes(); pausar(); break;
		case 0: printf("\nCerrando sesion...\n"); break;
		default: printf("\nOpcion invalida. Intente de nuevo.\n"); pausar();
		}
	} while (opcion != 0);
}

void mostrar_tarifas_vigentes() {
	limpiar_pantalla();
	printf("=== TARIFAS VIGENTES PARA MATRICULACION %d ===\n", ANO_FISCAL);
	printf("==============================================\n\n");
	printf("TASAS ANT (Agencia Nacional de Transito):\n");
	printf(" Vehiculos Particulares:      $%.2f\n", TASA_ANT_PARTICULAR);
	printf(" Vehiculos Comerciales:       $%.2f\n", TASA_ANT_COMERCIAL);
	printf(" Motocicletas:                $%.2f\n", TASA_ANT_MOTOCICLETA);
	printf("\n");
	printf("TASAS PREFECTURA DE PICHINCHA:\n");
	printf(" Vehiculos Particulares:      $%.2f\n", TASA_PREFECTURA_PARTICULAR);
	printf(" Vehiculos Comerciales:       $%.2f\n", TASA_PREFECTURA_COMERCIAL);
	printf(" Motocicletas:                $%.2f\n", TASA_PREFECTURA_MOTOCICLETA);
	printf("\n");
	printf("REVISION TECNICA VEHICULAR (RTV):\n");
	printf(" Vehiculos Livianos:          $%.2f\n", VALOR_RTV_LIVIANO);
	printf(" Vehiculos Pesados:           $%.2f\n", VALOR_RTV_PESADO);
	printf(" Motocicletas:                $%.2f\n", VALOR_RTV_MOTOCICLETA);
	printf("\n");
	printf("OTROS VALORES:\n");
	printf(" Adhesivo (Sticker):          $%.2f\n", VALOR_ADHESIVO);
	printf(" Recargo anual por mora:      %.1f%%\n", RECARGO_ANUAL_PORCENTAJE);
	printf("\n");
}

// --- Funci?n Principal de Ejecuci?n ---
int main() {
	setlocale(LC_ALL, ""); // Para caracteres especiales como tildes
	int opcion;
	char buffer[10];
	
	do {
		limpiar_pantalla();
		printf("=========================================\n");
		printf("   SISTEMA DE MATRICULACION VEHICULAR\n");
		printf("              %d\n", ANO_FISCAL);
		printf("=========================================\n\n");
		printf("1. Iniciar sesion\n");
		printf("2. Registrar nuevo usuario\n");
		printf("0. Salir del programa\n");
		printf("-----------------------------------------\n");
		printf("Seleccione una opcion: ");
		
		if (fgets(buffer, sizeof(buffer), stdin)) {
			if (sscanf(buffer, "%d", &opcion) != 1) opcion = -1;
		} else {
			opcion = -1;
		}
		
		switch (opcion) {
		case 1:
			if (iniciar_sesion()) {
				menu_vehiculos();
			}
			break;
		case 2:
			registrar_usuario();
			break;
		case 0:
			limpiar_pantalla();
			printf("=== GRACIAS POR USAR EL SISTEMA ===\n");
			printf("Desarrollado por: Mathias, Jhostin, Christian\n");
			printf("Hasta luego!\n");
			break;
		default:
			printf("\nOpcion invalida. Intente de nuevo.\n");
			pausar();
		}
	} while (opcion != 0);
	
	return 0;
}
