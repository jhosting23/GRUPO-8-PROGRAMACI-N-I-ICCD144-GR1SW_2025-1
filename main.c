#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h> // Le dice al programa que use la configuracion local del sistema

// Incluimos nuestras librerías
#include "vehiculos.h"  // Contiene funciones para registrar y buscar vehículos
#include "matricula.h"  // Contiene funciones para calcular la matrícula y sus tablas

// Constantes y funciones para el sistema de autenticación de usuarios
#define ARCHIVO_USUARIOS "usuarios.txt"
#define MAX_LINEA 100 // Usado para leer líneas del archivo de usuarios
#define MAX_INTENTOS 3

// FUNCIONES DEL SISTEMA DE AUTENTICACIÓN
void limpiar_pantalla() {
	system("cls");
}

void pausar() {
	printf("\nPresione Enter para continuar...");
	while (getchar() != '\n' && getchar() != EOF);
	getchar(); // Esperar la pulsación de Enter
}

int usuario_existe(const char* usuario) {
	FILE* file = fopen(ARCHIVO_USUARIOS, "r");
	if (!file) return 0; // Si el archivo no existe, no hay usuarios registrados
	
	char linea[MAX_LINEA];
	while (fgets(linea, sizeof(linea), file)) {
		char nombre_archivo[50];
		// Lee hasta la primera coma o 49 caracteres para el nombre de usuario
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
	
	printf("Ingrese contraseña: ");
	scanf("%49s", contrasena);
	
	FILE* file = fopen(ARCHIVO_USUARIOS, "a");
	if (!file) {
		perror("No se pudo abrir el archivo de usuarios"); // Imprime el error del sistema
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
	printf("=== Inicio de Sesion ===\n");
	
	while (intentos < MAX_INTENTOS) {
		printf("Usuario: ");
		scanf("%49s", usuario);
		printf("Contrasena: ");
		scanf("%49s", contrasena);
		
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
			printf("Credenciales incorrectas. Intento %d de %d.\n", ++intentos, MAX_INTENTOS);
			if (intentos < MAX_INTENTOS) {
				printf("Intente nuevamente:\n");
			}
		}
	}
	
	printf("Demasiados intentos fallidos. Intente mas tarde.\n");
	pausar();
	return 0;
}

// MENÚ DEL SISTEMA DE VEHÍCULOS CON CÁLCULO DE MATRÍCULA INTEGRADO
void menu_vehiculos() {
	int opcion;
	
	do {
		limpiar_pantalla();
		printf("=== SISTEMA DE MATRICULACION VEHICULAR ===\n");
		printf("1. Registrar nuevo vehiculo\n");
		printf("2. Buscar vehiculo por placa\n");
		printf("3. Calcular matricula vehicular\n");
		printf("4. Mostrar tarifas vigentes\n");
		printf("0. Volver al menu principal\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
		case 1:
			limpiar_pantalla();
			registrar_vehiculo();  // Función de la librería vehiculos.h
			pausar();
			break;
			
		case 2:
			limpiar_pantalla();
			buscar_vehiculo();  // Función de la librería vehiculos.h
			pausar();
			break;
			
		case 3:
			limpiar_pantalla();
			menu_calculo_matricula();  // Función de la librería matricula.h
			pausar();
			break;
			
		case 4:
			limpiar_pantalla();
			mostrar_tarifas_vigentes(); // Función local en main.c
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

// FUNCIÓN PARA MOSTRAR LAS TARIFAS VIGENTES
void mostrar_tarifas_vigentes() {
	
	printf("=== TARIFAS VIGENTES PARA MATRICULACION %d ===\n", ANO_FISCAL);
	printf("==============================================\n\n");
	
	printf("TASAS ANT (Agencia Nacional de Transito):\n");
	printf("• Vehiculos Particulares:      $%.2f\n", TASA_ANT_PARTICULAR);
	printf("• Vehiculos Comerciales:       $%.2f\n", TASA_ANT_COMERCIAL);
	printf("• Motocicletas:                $%.2f\n", TASA_ANT_MOTOCICLETA);
	printf("\n");
	
	printf("TASAS PREFECTURA DE PICHINCHA:\n");
	printf("• Vehiculos Particulares:      $%.2f\n", TASA_PREFECTURA_PARTICULAR);
	printf("• Vehiculos Comerciales:       $%.2f\n", TASA_PREFECTURA_COMERCIAL);
	printf("• Motocicletas:                $%.2f\n", TASA_PREFECTURA_MOTOCICLETA);
	printf("\n");
	
	printf("REVISION TECNICA VEHICULAR (RTV):\n");
	printf("• Vehiculos Livianos:          $%.2f\n", VALOR_RTV_LIVIANO);
	printf("• Vehiculos Pesados:           $%.2f\n", VALOR_RTV_PESADO);
	printf("• Motocicletas:                $%.2f\n", VALOR_RTV_MOTOCICLETA);
	printf("\n");
	
	printf("TASAS SPPAT (Sistema de Pagos):\n");
	printf("• Motos hasta 200cc:           $%.2f\n", SPPAT_MOTO_HASTA_200);
	printf("• Motos mas de 200cc:          $%.2f\n", SPPAT_MOTO_MAS_200);
	printf("• Livianos hasta 1500cc:       $%.2f\n", SPPAT_LIVIANO_HASTA_1500);
	printf("• Livianos 1501-2500cc:        $%.2f\n", SPPAT_LIVIANO_1501_2500);
	printf("• Livianos mas de 2500cc:      $%.2f\n", SPPAT_LIVIANO_MAS_2500);
	printf("• Vehiculos Pesados:           $%.2f\n", SPPAT_PESADO);
	printf("• Vehiculos Comerciales:       $%.2f\n", SPPAT_COMERCIAL);
	printf("\n");
	
	printf("OTROS VALORES:\n");
	printf("• Adhesivo (Sticker):          $%.2f\n", VALOR_ADHESIVO);
	printf("• Recargo anual por mora:      %.1f%%\n", RECARGO_ANUAL_PORCENTAJE);
	printf("\n");
	
	printf("IMPUESTOS (Calculados segun avaluo):\n");
	printf("• Impuesto a la Propiedad (SRI)\n");
	printf("• Impuesto al Rodaje (AMT-Quito)\n");
	printf("  - Ambos varian segun el avaluo del vehiculo\n");
	printf("  - Use la opcion 'Calcular matricula' para detalles\n");
	printf("\n");
}

// FUNCIÓN MAIN PRINCIPAL
int main() {
	setlocale(LC_ALL, ""); // Configura la localización para manejo de caracteres especiales
	int opcion;
	
	printf("=== SISTEMA DE MATRICULACION VEHICULAR ===\n");
	printf("=========================================\n");
	printf("        Version %d - Ecuador\n", ANO_FISCAL);
	printf("=========================================\n\n");
	
	do {
		limpiar_pantalla();
		printf("=== Sistema de Matriculacion Vehicular ===\n");
		printf("1. Registrar nuevo usuario\n");
		printf("2. Iniciar sesion\n");
		printf("0. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		while (getchar() != '\n'); // O puedes llamar a limpiar_entrada() 
		
		switch (opcion) {
		case 1:
			registrar_usuario();
			break;
			
		case 2:
			if (iniciar_sesion()) {
				// Si el inicio de sesión es exitoso, entra al menú de vehículos
				menu_vehiculos();
			}
			break;
			
		case 0:
			limpiar_pantalla();
			printf("=== GRACIAS POR USAR EL SISTEMA ===\n");
			printf("Sistema de Matriculacion Vehicular %d\n", ANO_FISCAL);
			printf("Desarrollado por: Mathias, Jhostin, Christian\n");
			printf("¡Hasta luego!\n");
			printf("==================================\n");
			break;
			
		default:
			printf("Opcion invalida.\n");
			pausar();
		}
	} while (opcion != 0);
	
	return 0;
}
