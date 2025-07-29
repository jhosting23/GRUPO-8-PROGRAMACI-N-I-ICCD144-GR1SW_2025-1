/*
 * main.c - Archivo principal del Sistema de Matriculacion Vehicular
 * 
 * Descripcion: Este archivo contiene la implementacion principal del sistema
 *              de matriculacion vehicular para Ecuador, incluyendo:
 *              - Sistema de autenticacion de usuarios
 *              - Menu principal del programa
 *              - Interfaz de login moderna
 *              - Validacion de credenciales
 * 
 * Autores: Mathias, Jhostin, Christian
 * Fecha: 2025
 *Programacion I - ICCD144
 * Universidad: Escuela politecnica nacional
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

// Incluimos nuestras librerias personalizadas
#include "vehiculos.h"
#include "matricula.h"
#include "pagos.h"

// Constantes y definiciones del sistema
#define ARCHIVO_USUARIOS "usuarios.txt"
#define MAX_LINEA 100
#define MAX_INTENTOS 3

// Incluir las librerias necesarias para ocultar la contrasena
#ifdef _WIN32
#include <conio.h>  // Para getch() en Windows
#else
#include <termios.h> // Para configurar la terminal en Linux/macOS
#include <unistd.h>  // Para STDIN_FILENO
#endif

// ===================================================================
// PROTOTIPOS DE FUNCIONES DEFINIDAS EN ESTE ARCHIVO
// ===================================================================
// Funciones de utilidad general
void pausar();
void obtener_contrasena_oculta(char* buffer, int tamano);

// Funciones de gestion de usuarios
int usuario_existe(const char* usuario);
int registrar_usuario();
int validar_credenciales(const char* usuario, const char* contrasena);

// Funciones del sistema de login
int iniciar_sesion();
void dibujar_login_moderno();
void dibujar_formulario_login(const char* usuario_actual, int fase);

// Funciones del menu principal
void menu_vehiculos();
void mostrar_tarifas_vigentes();

// ===================================================================
// IMPLEMENTACION DE FUNCIONES DE UTILIDAD
// ===================================================================

/*
 * Funcion: limpiar_pantalla
 * Descripcion: Limpia la pantalla de la consola de forma multiplataforma
 * Parametros: Ninguno
 * Retorno: void
 */
void limpiar_pantalla() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

/*
 * Funcion: obtener_contrasena_oculta
 * Descripcion: Lee una contrasena del usuario y la muestra como asteriscos
 *              para mayor seguridad. Funciona en Windows y Linux/macOS.
 * Parametros: 
 *   - buffer: Array donde se guardara la contrasena
 *   - tamano: Tamaño maximo del buffer
 * Retorno: void
 */
void obtener_contrasena_oculta(char* buffer, int tamano) {
	int i = 0;
	char c;
	
#ifdef _WIN32
	// Implementacion para Windows usando conio.h
	while (i < tamano - 1) {
		c = getch();
		if (c == '\r' || c == '\n') { // Tecla Enter
			break;
		} else if (c == '\b') { // Tecla de retroceso (backspace)
			if (i > 0) {
				i--;
				printf("\b \b"); // Mueve cursor atras, escribe espacio y vuelve atras
			}
		} else {
			buffer[i++] = c;
			printf("*"); // Mostrar asterisco en lugar del caracter real
		}
	}
#else
	// Implementacion para Linux/macOS usando termios.h
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt); // Obtener configuracion actual de terminal
	newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON); // Desactivar eco y modo canonico
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Aplicar nueva configuracion
	
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
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaurar configuracion original
#endif
	
	buffer[i] = '\0'; // Terminar la cadena con caracter nulo
	printf("\n"); // Nueva linea despues de presionar Enter
}

/*
 * Funcion: pausar
 * Descripcion: Pausa la ejecucion del programa hasta que el usuario presione Enter
 * Parametros: Ninguno
 * Retorno: void
 */

void pausar() {
	printf("\nPresione Enter para continuar...");
	getchar(); // Espera una pulsacion de tecla
}

// ===================================================================
// IMPLEMENTACION DE FUNCIONES DE AUTENTICACION
// ===================================================================

/*
 * Funcion: dibujar_login_moderno
 * Descripcion: Dibuja el marco decorativo del sistema de login
 * Parametros: Ninguno
 * Retorno: void
 */
void dibujar_login_moderno() {
	printf("================================================================\n");
	printf("||                                                            ||\n");
	printf("||                            LOGIN                           ||\n");
	printf("||                                                            ||\n");
	printf("||              SISTEMA DE MATRICULACION VEHICULAR            ||\n");
	printf("||                            2025                            ||\n");
	printf("||                                                            ||\n");
	printf("================================================================\n");
}

/*
 * Funcion: dibujar_formulario_login
 * Descripcion: Dibuja el formulario de login con campos de usuario y contrasena
 * Parametros:
 *   - usuario_actual: Nombre del usuario actual (puede estar vacio)
 *   - fase: Fase del login (1=solo usuario, 2=usuario y contrasena)
 * Retorno: void
 */
void dibujar_formulario_login(const char* usuario_actual, int fase) {
	printf("\n");
	printf("    +----------------------------------------------------------+\n");
	printf("    |                                                          |\n");
	printf("    |  Usuario:                                                |\n");
	printf("    |  +----------------------------------------------------+  |\n");
	printf("    |  | %-50s |  |\n", usuario_actual);
	printf("    |  +----------------------------------------------------+  |\n");
	printf("    |                                                          |\n");
	
	if (fase >= 2) {
		printf("    |  Contrasena:                                             |\n");
		printf("    |  +----------------------------------------------------+  |\n");
		printf("    |  | %-50s |  |\n", "");
		printf("    |  +----------------------------------------------------+  |\n");
		printf("    |                                                          |\n");
	}
	
	printf("    +----------------------------------------------------------+\n");
}

/*
 * Funcion: usuario_existe
 * Descripcion: Verifica si un usuario existe en el archivo de usuarios
 * Parametros:
 *   - usuario: Nombre del usuario a verificar
 * Retorno: 1 si existe, 0 si no existe
 */

int usuario_existe(const char* usuario) {
	FILE* file = fopen(ARCHIVO_USUARIOS, "r");
	if (!file) return 0; // Si no se puede abrir el archivo, el usuario no existe
	
	char linea[MAX_LINEA];
	while (fgets(linea, sizeof(linea), file)) {
		char nombre_archivo[50];
		sscanf(linea, "%49[^,]", nombre_archivo);
		if (strcmp(nombre_archivo, usuario) == 0) {
			fclose(file);
			return 1; // Usuario encontrado
		}
	}
	fclose(file);
	return 0; // Usuario no encontrado
}

/*
 * Funcion: registrar_usuario
 * Descripcion: Registra un nuevo usuario en el sistema
 * Parametros: Ninguno (solicita datos al usuario)
 * Retorno: 1 si el registro fue exitoso, 0 si hubo error
 */

int registrar_usuario() {
	char usuario[50], contrasena[50];
	char buffer[100];
	
	limpiar_pantalla();
	printf("=== REGISTRO DE NUEVO USUARIO ===\n");
	
	printf("Ingrese nombre de usuario: ");
	if (fgets(buffer, sizeof(buffer), stdin)) {
		sscanf(buffer, "%49s", usuario);
	}
	
	// Verificar si el usuario ya existe
	if (usuario_existe(usuario)) {
		printf("\nERROR: El usuario '%s' ya existe.\n", usuario);
		pausar();
		return 0;
	}
	
	printf("Ingrese contrasena: ");
	obtener_contrasena_oculta(contrasena, 50);
	
	// Intentar abrir el archivo para escritura
	FILE* file = fopen(ARCHIVO_USUARIOS, "a");
	if (!file) {
		perror("No se pudo abrir el archivo de usuarios");
		pausar();
		return 0;
	}
	
	// Guardar el usuario en el archivo
	fprintf(file, "%s,%s\n", usuario, contrasena);
	fclose(file);
	printf("\nUsuario registrado con exito.\n");
	pausar();
	return 1;
}

/*
 * Funcion: validar_credenciales
 * Descripcion: Valida las credenciales de un usuario contra el archivo
 * Parametros:
 *   - usuario: Nombre del usuario
 *   - contrasena: Contrasena del usuario
 * Retorno: 1 si las credenciales son validas, 0 si no lo son
 */

int validar_credenciales(const char* usuario, const char* contrasena) {
	FILE* file = fopen(ARCHIVO_USUARIOS, "r");
	if (!file) return 0; // Si no se puede abrir el archivo, credenciales invalidas
	
	char linea[MAX_LINEA];
	while (fgets(linea, sizeof(linea), file)) {
		char usuar_tamano[50], contra_tamano[50];
		// Leer usuario y contrasena del archivo, removiendo el salto de linea
		sscanf(linea, "%49[^,],%49[^\n]", usuar_tamano, contra_tamano);
		if (strcmp(usuar_tamano, usuario) == 0 && strcmp(contra_tamano, contrasena) == 0) {
			fclose(file);
			return 1; // Credenciales validas
		}
	}
	fclose(file);
	return 0; // Credenciales invalidas
}

/*
 * Funcion: iniciar_sesion
 * Descripcion: Maneja el proceso completo de inicio de sesion del usuario
 *              Incluye validacion de credenciales, reintentos y bloqueo
 * Parametros: Ninguno
 * Retorno: 1 si el login fue exitoso, 0 si fallo o se cancelo
 */

int iniciar_sesion() {
	char usuario[50] = "";
	char contrasena[50] = "";
	char buffer[100];
	int intentos = 0;
	
	while (intentos < MAX_INTENTOS) {
		limpiar_pantalla();
		
		// Mostrar intentos restantes si no es el primer intento
		if (intentos > 0) {
			printf("*** CREDENCIALES INCORRECTAS ***\n");
			printf("Intentos restantes: %d\n\n", MAX_INTENTOS - intentos);
		}
		
		// Dibujar interfaz principal del login
		dibujar_login_moderno();
		
		// FASE 1: Capturar usuario con opciones adicionales
		dibujar_formulario_login("", 1);
		printf("\n    Escriba su usuario y presione Enter:\n");
		printf("    (Escriba '0' para registrar un nuevo usuario o 'SALIR' para cerrar)\n");
		printf("    > ");
		
		if (fgets(buffer, sizeof(buffer), stdin)) {
			sscanf(buffer, "%49s", usuario);
		}
		
		// Opcion para registrar nuevo usuario
		if (strcmp(usuario, "0") == 0) {
			registrar_usuario();
			continue;
		}
		
		// Opcion para salir del programa
		if (strcmp(usuario, "SALIR") == 0 || strcmp(usuario, "salir") == 0) {
			limpiar_pantalla();
			printf("===============================================\n");
			printf("||                                           ||\n");
			printf("||        GRACIAS POR USAR EL SISTEMA        ||\n");
			printf("||                                           ||\n");
			printf("||      Desarrollado por: Mathias,           ||\n");
			printf("||        Jhostin, Christian                 ||\n");
			printf("||                                           ||\n");
			printf("||             Hasta luego!                  ||\n");
			printf("||                                           ||\n");
			printf("===============================================\n");
			return 0;
		}
		
		// Validar que el usuario no este vacio
		if (strlen(usuario) == 0) {
			printf("\n    ERROR: El usuario no puede estar vacio.\n");
			printf("    Presione Enter para continuar...");
			getchar();
			continue;
		}
		
		// FASE 2: Mostrar usuario ingresado y solicitar contrasena
		limpiar_pantalla();
		dibujar_login_moderno();
		dibujar_formulario_login(usuario, 2);
		
		printf("\n    Escriba su contrasena y presione Enter:\n");
		printf("    > ");
		
		// Obtener contrasena de forma oculta
		obtener_contrasena_oculta(contrasena, 50);
		
		// Validar las credenciales ingresadas
		if (validar_credenciales(usuario, contrasena)) {
			// Login exitoso - Mostrar pantalla de bienvenida
			limpiar_pantalla();
			printf("===============================================\n");
			printf("||                                           ||\n");
			printf("||            ACCESO CONCEDIDO               ||\n");
			printf("||                                           ||\n");
			printf("===============================================\n");
			printf("||                                           ||\n");
			printf("||          BIENVENIDO AL SISTEMA            ||\n");
			printf("||                                           ||\n");
			printf("||        Sistema de Matriculacion           ||\n");
			printf("||              Vehicular                    ||\n");
			printf("||                2025                       ||\n");
			printf("||                                           ||\n");
			printf("||    Usuario: %-29s ||\n", usuario);
			printf("||    Fecha de acceso: Hoy                   ||\n");
			printf("||                                           ||\n");
			printf("||      Desarrollado por: Mathias,           ||\n");
			printf("||        Jhostin, Christian                 ||\n");
			printf("||                                           ||\n");
			printf("===============================================\n");
			
			printf("\n    Cargando sistema");
			
			// Animacion de carga con puntos
			for (int i = 0; i < 5; i++) {
				printf(".");
				fflush(stdout);
#ifdef _WIN32
				system("timeout /t 1 /nobreak >nul 2>&1");
#else
				usleep(100000); // 0.1 segundos
#endif
			}
			
			printf("\n\n    Sistema cargado exitosamente!\n");
			pausar();
			return 1;
			
		} else {
			// Credenciales incorrectas - Mostrar mensaje de error
			intentos++;
			limpiar_pantalla();
			
			printf("==================================================================\n");
			printf("||                                                              ||\n");
			printf("||                        ACCESO DENEGADO                       ||\n");
			printf("||                                                              ||\n");
			printf("||                Usuario o contrasena incorrectos              ||\n");
			printf("||                                                              ||\n");
			printf("||                    Intento %d de %d                            ||\n", intentos, MAX_INTENTOS);
			printf("||                                                              ||\n");
			printf("==================================================================\n");
			
			if (intentos < MAX_INTENTOS) {
				printf("\n    Reintentando en 2 segundos...\n");
#ifdef _WIN32
				system("timeout /t 2 >nul 2>&1");
#else
				system("sleep 2");
#endif
			}
		}
	}
	
	// Sistema bloqueado por demasiados intentos fallidos
	limpiar_pantalla();
	printf("==================================================================\n");
	printf("||                                                              ||\n");
	printf("||                      SISTEMA BLOQUEADO                       ||\n");
	printf("||                                                              ||\n");
	printf("||                  Demasiados intentos fallidos                ||\n");
	printf("||                                                              ||\n");
	printf("||                    El programa se cerrara                    ||\n");
	printf("||                                                              ||\n");
	printf("||                   Contacte al administrador                  ||\n");
	printf("||                                                              ||\n");
	printf("==================================================================\n");
	
	pausar();
	return 0;
}

// ===================================================================
// IMPLEMENTACION DE MENUS DEL SISTEMA
// ===================================================================

/*
 * Funcion: menu_vehiculos
 * Descripcion: Muestra el menu principal del sistema de vehiculos
 *              Permite acceder a todas las funcionalidades del sistema
 * Parametros: Ninguno
 * Retorno: void
 */

void menu_vehiculos() {
	int opcion;
	char buffer[10];
	
	do {
		limpiar_pantalla();
		printf("===============================================\n");
		printf("||                                           ||\n");
		printf("||               MENU PRINCIPAL              ||\n");
		printf("||                                           ||\n");
		printf("||        Sistema de Matriculacion           ||\n");
		printf("||              Vehicular                    ||\n");
		printf("||                                           ||\n");
		printf("===============================================\n");
		
		printf("\n    +----------------------------------------------------------+\n");
		printf("    |                   OPCIONES DISPONIBLES                   |\n");
		printf("    +----------------------------------------------------------+\n");
		printf("    |                                                          |\n");
		printf("    |    1. Registrar nuevo vehiculo                           |\n");
		printf("    |    2. Buscar vehiculo por placa                          |\n");
		printf("    |    3. Calcular valor de matricula                        |\n");
		printf("    |    4. Procesar pago de matricula                         |\n");
		printf("    |    5. Registrar revision tecnica                         |\n");
		printf("    |    6. Matricular vehiculo                                |\n");
		printf("    |    7. Ver vehiculos matriculados                         |\n");
		printf("    |    8. Mostrar tarifas vigentes                           |\n");
		printf("    |    0. Cerrar sesion y volver al menu                     |\n");
		printf("    |                                                          |\n");
		printf("    +----------------------------------------------------------+\n");
		
		printf("\n    Seleccione una opcion: ");
		
		// Leer opcion del usuario de forma segura
		if (fgets(buffer, sizeof(buffer), stdin)) {
			if (sscanf(buffer, "%d", &opcion) != 1) opcion = -1;
		} else {
			opcion = -1;
		}
		
		// Procesar la opcion seleccionada
		switch (opcion) {
		case 1: 
			registrar_vehiculo(); 
			break;
		case 2: 
			buscar_vehiculo(); 
			pausar(); 
			break;
		case 3: 
			calcular_matricula_mostrar(); 
			break;
		case 4: 
			menu_pagos(); 
			break;
		case 5: 
			registrar_revision_simple(); 
			break;
		case 6: 
			proceso_matriculacion_final(); 
			break;
		case 7: 
			mostrar_vehiculos_matriculados(); 
			break;
		case 8: 
			mostrar_tarifas_vigentes(); 
			pausar(); 
			break;
		case 0: 
			printf("\n    Cerrando sesion...\n");
			printf("    Sesion cerrada exitosamente.\n");
			pausar();
			break;
		default: 
			printf("\n    ERROR: Opcion invalida. Intente de nuevo.\n");
			pausar();
		}
	} while (opcion != 0);
}

/*
 * Funcion: mostrar_tarifas_vigentes
 * Descripcion: Muestra las tarifas actuales para matriculacion vehicular
 * Parametros: Ninguno
 * Retorno: void
 */

void mostrar_tarifas_vigentes() {
	limpiar_pantalla();
	printf("===============================================\n");
	printf("||                                           ||\n");
	printf("||         TARIFAS VIGENTES PARA             ||\n");
	printf("||           MATRICULACION 2025              ||\n");
	printf("||                                           ||\n");
	printf("===============================================\n\n");
	
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

// ===================================================================
// FUNCION PRINCIPAL DEL PROGRAMA
// ===================================================================

/*
 * Funcion: main
 * Descripcion: Funcion principal del programa. Controla el flujo general
 *              del sistema de matriculacion vehicular
 * Parametros: Ninguno
 * Retorno: 0 si el programa termina correctamente
 */
int main() {
	// Configurar la codificacion de caracteres para caracteres especiales
	setlocale(LC_ALL, "");
	
	// Bucle principal del programa
	while (1) {
		// Intentar iniciar sesion
		if (iniciar_sesion()) {
			// Si el login es exitoso, mostrar el menu de vehiculos
			menu_vehiculos();
		} else {
			// Si el usuario decide salir o el sistema se bloquea, terminar programa
			break;
		}
	}
	
	return 0; // Terminar programa exitosamente
}
