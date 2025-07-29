/*
 * pagos.c - Implementacion del modulo de pagos de matricula vehicular
 * 
 * Descripcion: Este archivo contiene la implementacion de todas las
 *              funciones para el procesamiento de pagos de matricula,
 *              incluyendo validacion de comprobantes, registro de pagos
 *              y generacion de comprobantes de pago.
 * 
 * Autores: Mathias, Jhostin, Christian
 * Fecha: 2025
 * Materia: Programacion I - ICCD144
 * Universidad: Escuela Politecnica Nacional
 */

#include "pagos.h"
#include "vehiculos.h"
#include <ctype.h>
#include <direct.h>  // Para _mkdir en Windows
#include <sys/stat.h>  // Para verificar si existe la carpeta

// ===================================================================
// FUNCIONES PRINCIPALES DEL MODULO DE PAGOS
// ===================================================================

/*
 * Funcion: menu_pagos
 * Descripcion: Muestra el menu principal del modulo de pagos
 * Parametros: ninguno
 * Retorno: void
 */
void menu_pagos() {
    int opcion;
    char buffer[10];
    
    // Crear carpetas del sistema si no existen
    crear_carpetas_sistema();
    
    do {
        // Limpiar pantalla
        system("cls");
        
        printf("\n");
        printf("=======================================================\n");
        printf("           MODULO DE PAGOS DE MATRICULA\n");
        printf("           AGENCIA NACIONAL DE TRANSITO\n");
        printf("=======================================================\n");
        printf("\n");
        printf("1. Pagar matricula por placa\n");
        printf("2. Consultar estado de pago por placa\n");
        printf("3. Volver al menu principal\n");
        printf("\n");
        printf("Ingrese su opcion: ");
        
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &opcion) != 1) {
                opcion = 0;
            }
        }
        
        switch (opcion) {
            case 1:
                procesar_pago_por_placa();
                break;
            case 2:
                consultar_estado_por_placa();
                break;
            case 3:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
                pausar_sistema();
        }
    } while (opcion != 3);
}

// ===================================================================
// FUNCIONES DE VALIDACION
// ===================================================================

/*
 * Funcion: comprobante_vigente
 * Descripcion: Verifica si un comprobante esta vigente
 * Parametros: fecha_vencimiento - Fecha de vencimiento del comprobante
 * Retorno: 1 si esta vigente, 0 si ha vencido
 */
int comprobante_vigente(const char* fecha_vencimiento) {
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm* fecha_actual = localtime(&t);
    
    // Extraer fecha de vencimiento (formato: DD/MM/YYYY)
    int dia_venc, mes_venc, ano_venc;
    if (sscanf(fecha_vencimiento, "%d/%d/%d", &dia_venc, &mes_venc, &ano_venc) != 3) {
        return 0;
    }
    
    // Comparar fechas
    int ano_actual = fecha_actual->tm_year + 1900;
    int mes_actual = fecha_actual->tm_mon + 1;
    int dia_actual = fecha_actual->tm_mday;
    
    if (ano_venc > ano_actual) return 1;
    if (ano_venc < ano_actual) return 0;
    
    if (mes_venc > mes_actual) return 1;
    if (mes_venc < mes_actual) return 0;
    
    return dia_venc >= dia_actual;
}

// ===================================================================
// FUNCIONES DE ARCHIVOS
// ===================================================================

/*
 * Funcion: guardar_comprobante_sistema
 * Descripcion: Guarda un comprobante en el archivo del sistema
 * Parametros: placa, resultado, vehiculo, numero_comprobante
 * Retorno: 1 si fue exitoso, 0 si hubo error
 */
int guardar_comprobante_sistema(const char* placa, ResultadoMatricula resultado, 
                               DatosVehiculo vehiculo, const char* numero_comprobante) {
    FILE* archivo = fopen(ARCHIVO_COMPROBANTES, "a");
    if (!archivo) {
        return 0;
    }
    
    // El numero de comprobante ya se paso como parametro, no generar uno nuevo
    
    // Obtener fechas
    char fecha_emision[20], fecha_vencimiento[20];
    obtener_fecha_actual(fecha_emision);
    calcular_fecha_vencimiento(fecha_vencimiento, DIAS_VALIDEZ_COMPROBANTE);
    
    // Guardar en formato: placa|numero_comprobante|propietario|tipo|subtipo|fecha_emision|fecha_vencimiento|total|estado
    char cedula_propietario[15], nombre_propietario[100];
    if (obtener_datos_propietario(placa, cedula_propietario, nombre_propietario)) {
        fprintf(archivo, "%s|%s|%s|%s|%s|%s|%s|%.2f|%d\n", 
                placa, numero_comprobante, nombre_propietario, vehiculo.tipo, vehiculo.subtipo,
                fecha_emision, fecha_vencimiento, resultado.total_matricula, ESTADO_PENDIENTE);
    } else {
        // Si no se encuentran los datos del propietario, usar valores por defecto
        fprintf(archivo, "%s|%s|%s|%s|%s|%s|%s|%.2f|%d\n", 
                placa, numero_comprobante, "N/A", vehiculo.tipo, vehiculo.subtipo,
                fecha_emision, fecha_vencimiento, resultado.total_matricula, ESTADO_PENDIENTE);
    }
    
    fclose(archivo);
    return 1;
}

/*
 * Funcion: guardar_registro_pago
 * Descripcion: Guarda un registro de pago en el archivo
 * Parametros: pago - Estructura con datos del pago
 * Retorno: 1 si fue exitoso, 0 si hubo error
 */
int guardar_registro_pago(RegistroPago pago) {
    FILE* archivo = fopen(ARCHIVO_PAGOS, "a");
    if (!archivo) {
        return 0;
    }
    
    // Formato: numero_comprobante|placa|fecha_pago|monto|tipo|referencia|cedula|nombre
    fprintf(archivo, "%s|%s|%s|%.2f|%d|%s|%s|%s\n",
            pago.numero_comprobante, pago.placa, pago.fecha_pago, 
            pago.monto_pagado, pago.tipo_pago, pago.referencia_pago,
            pago.cedula_pagador, pago.nombre_pagador);
    
    fclose(archivo);
    return 1;
}

/*
 * Funcion: actualizar_estado_comprobante
 * Descripcion: Actualiza el estado de un comprobante
 * Parametros: numero_comprobante, nuevo_estado
 * Retorno: 1 si fue exitoso, 0 si hubo error
 */
int actualizar_estado_comprobante(const char* numero_comprobante, int nuevo_estado) {
    FILE* archivo = fopen(ARCHIVO_COMPROBANTES, "r");
    FILE* temp = fopen("temp_comprobantes.txt", "w");
    
    if (!archivo || !temp) {
        if (archivo) fclose(archivo);
        if (temp) fclose(temp);
        return 0;
    }
    
    char linea[500];
    while (fgets(linea, sizeof(linea), archivo)) {
        char linea_copia[500];
        strcpy(linea_copia, linea);
        
        // Formato: placa|numero_comprobante|propietario|tipo|subtipo|fecha_emision|fecha_vencimiento|total|estado
        char placa_temp[20], numero_temp[50], propietario_temp[100], tipo_temp[50], subtipo_temp[50];
        char fecha_emision_temp[20], fecha_vencimiento_temp[20];
        float total_temp;
        int estado_temp;
        
        if (sscanf(linea_copia, "%19[^|]|%49[^|]|%99[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%f|%d",
                   placa_temp, numero_temp, propietario_temp, tipo_temp, subtipo_temp,
                   fecha_emision_temp, fecha_vencimiento_temp, &total_temp, &estado_temp) == 9) {
            
            if (strcmp(numero_temp, numero_comprobante) == 0) {
                // Actualizar estado - mantener el mismo formato
                fprintf(temp, "%s|%s|%s|%s|%s|%s|%s|%.2f|%d\n", 
                        placa_temp, numero_temp, propietario_temp, tipo_temp, subtipo_temp,
                        fecha_emision_temp, fecha_vencimiento_temp, total_temp, nuevo_estado);
            } else {
                fprintf(temp, "%s", linea);
            }
        } else {
            fprintf(temp, "%s", linea);
        }
    }
    
    fclose(archivo);
    fclose(temp);
    
    // Reemplazar archivo original
    remove(ARCHIVO_COMPROBANTES);
    rename("temp_comprobantes.txt", ARCHIVO_COMPROBANTES);
    
    return 1;
}

/*
 * Funcion: obtener_datos_propietario
 * Descripcion: Obtiene los datos del propietario de un vehiculo por placa
 * Parametros: placa - Placa del vehiculo, cedula - Buffer para cedula, nombre - Buffer para nombre
 * Retorno: 1 si encontro los datos, 0 si no los encontro
 */
int obtener_datos_propietario(const char* placa, char* cedula, char* nombre) {
    FILE* archivo = fopen("vehiculos.txt", "r");
    if (!archivo) {
        return 0;
    }
    
    char linea[500];
    while (fgets(linea, sizeof(linea), archivo)) {
        // Formato: placa,cedula,nombre,tipo,subtipo,anio,valor,cilindraje
        char placa_temp[10], cedula_temp[15], nombre_temp[100];
        char tipo_temp[20], subtipo_temp[20];
        int anio_temp, cilindraje_temp;
        float valor_temp;
        
        if (sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f,%d", 
                   placa_temp, cedula_temp, nombre_temp, tipo_temp, 
                   subtipo_temp, &anio_temp, &valor_temp, &cilindraje_temp) == 8) {
            
            if (strcmp(placa_temp, placa) == 0) {
                strcpy(cedula, cedula_temp);
                strcpy(nombre, nombre_temp);
                fclose(archivo);
                return 1;
            }
        }
    }
    
    fclose(archivo);
    return 0;
}

// ===================================================================
// FUNCIONES DE CONSULTA
// ===================================================================

/*
 * Funcion: consultar_estado_por_placa
 * Descripcion: Consulta el estado de un comprobante buscando por placa del vehiculo
 * Parametros: ninguno
 * Retorno: 1 si encontro el comprobante, 0 si no
 */
int consultar_estado_por_placa() {
    char placa[20];
    char buffer[100];
    ComprobanteMatricula comprobante;
    
    system("cls");
    printf("\n");
    printf("=======================================================\n");
    printf("           CONSULTA DE ESTADO POR PLACA\n");
    printf("           AGENCIA NACIONAL DE TRANSITO\n");
    printf("=======================================================\n");
    printf("\n");
    
    printf("Ingrese la placa del vehiculo: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error al leer la placa del vehiculo.\n");
        pausar_sistema();
        return 0;
    }
    
    // Limpiar salto de linea y convertir a mayusculas
    buffer[strcspn(buffer, "\n")] = 0;
    strcpy(placa, buffer);
    for (int i = 0; placa[i]; i++) {
        placa[i] = toupper(placa[i]);
    }
    
    // Buscar comprobante para esta placa
    FILE* archivo = fopen(ARCHIVO_COMPROBANTES, "r");
    if (!archivo) {
        printf("No se encontraron comprobantes en el sistema.\n");
        pausar_sistema();
        return 0;
    }
    
    char linea[500];
    int comprobante_encontrado = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        char placa_temp[20], numero_temp[50], propietario_temp[100], tipo_temp[50], subtipo_temp[50];
        char fecha_emision_temp[20], fecha_vencimiento_temp[20];
        float total_temp;
        int estado_temp;
        
        if (sscanf(linea, "%19[^|]|%49[^|]|%99[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%f|%d",
                   placa_temp, numero_temp, propietario_temp, tipo_temp, subtipo_temp,
                   fecha_emision_temp, fecha_vencimiento_temp, &total_temp, &estado_temp) == 9) {
            
            if (strcmp(placa_temp, placa) == 0) {
                // Encontramos el comprobante
                strcpy(comprobante.numero_comprobante, numero_temp);
                strcpy(comprobante.placa, placa_temp);
                strcpy(comprobante.fecha_emision, fecha_emision_temp);
                strcpy(comprobante.fecha_vencimiento, fecha_vencimiento_temp);
                comprobante.monto_total = total_temp;
                comprobante.estado = estado_temp;
                comprobante_encontrado = 1;
                break;
            }
        }
    }
    fclose(archivo);
    
    if (!comprobante_encontrado) {
        printf("No se encontro comprobante para la placa '%s'.\n", placa);
        printf("\nOpciones:\n");
        printf("- Verifique que la placa este correcta\n");
        printf("- Calcule la matricula primero si no lo ha hecho\n");
        pausar_sistema();
        return 0;
    }
    
    // Mostrar informacion del comprobante
    printf("INFORMACION DEL COMPROBANTE:\n");
    printf("=======================================================\n");
    printf("Placa: %s\n", comprobante.placa);
    printf("Numero de comprobante: %s\n", comprobante.numero_comprobante);
    printf("Fecha de emision: %s\n", comprobante.fecha_emision);
    printf("Fecha de vencimiento: %s\n", comprobante.fecha_vencimiento);
    printf("Monto total: $%.2f\n", comprobante.monto_total);
    printf("Estado: ");
    mostrar_estado_comprobante(comprobante.estado);
    printf("\n");
    printf("=======================================================\n");
    
    // Mostrar informacion adicional segun el estado
    if (comprobante.estado == ESTADO_PENDIENTE) {
        printf("\nACCIONES DISPONIBLES:\n");
        printf("- Puede proceder al pago en la opcion 1 del menu\n");
        if (!comprobante_vigente(comprobante.fecha_vencimiento)) {
            printf("- ATENCION: El comprobante esta vencido\n");
            printf("- Debe generar un nuevo comprobante calculando la matricula\n");
        }
    } else if (comprobante.estado == ESTADO_PAGADO) {
        printf("\nESTADO: MATRICULA PAGADA\n");
        printf("- Su matricula ya esta pagada\n");
        printf("- Puede proceder con la revision tecnica\n");
        printf("- Luego puede completar la matriculacion final\n");
    }
    
    pausar_sistema();
    return 1;
}

// ===================================================================
// FUNCIONES AUXILIARES
// ===================================================================

/*
 * Funcion: obtener_fecha_actual
 * Descripcion: Obtiene la fecha actual en formato DD/MM/YYYY HH:MM
 * Parametros: fecha - Buffer donde guardar la fecha
 * Retorno: void
 */
void obtener_fecha_actual(char* fecha) {
    time_t t = time(NULL);
    struct tm* fecha_actual = localtime(&t);
    
    sprintf(fecha, "%02d/%02d/%d %02d:%02d", 
            fecha_actual->tm_mday, 
            fecha_actual->tm_mon + 1, 
            fecha_actual->tm_year + 1900,
            fecha_actual->tm_hour,
            fecha_actual->tm_min);
}

/*
 * Funcion: calcular_fecha_vencimiento
 * Descripcion: Calcula la fecha de vencimiento agregando dias a la fecha actual
 * Parametros: fecha_vencimiento - Buffer donde guardar la fecha, dias - Dias a agregar
 * Retorno: void
 */
void calcular_fecha_vencimiento(char* fecha_vencimiento, int dias) {
    time_t t = time(NULL);
    t += dias * 24 * 60 * 60; // Agregar dias en segundos
    struct tm* fecha_venc = localtime(&t);
    
    sprintf(fecha_vencimiento, "%02d/%02d/%d", 
            fecha_venc->tm_mday, 
            fecha_venc->tm_mon + 1, 
            fecha_venc->tm_year + 1900);
}

/*
 * Funcion: generar_numero_comprobante
 * Descripcion: Genera un numero unico de comprobante
 * Parametros: numero - Buffer donde guardar el numero, placa - Placa del vehiculo
 * Retorno: void
 */
void generar_numero_comprobante(char* numero, const char* placa) {
    time_t t = time(NULL);
    struct tm* fecha = localtime(&t);
    
    sprintf(numero, "MAT-%s-%04d%02d%02d-%03d", 
            placa, 
            fecha->tm_year + 1900,
            fecha->tm_mon + 1,
            fecha->tm_mday,
            rand() % 1000);
}

// ===================================================================
// FUNCIONES DE INTERFAZ
// ===================================================================

/*
 * Funcion: mostrar_estado_comprobante
 * Descripcion: Muestra el estado de un comprobante
 * Parametros: estado - Estado del comprobante
 * Retorno: void
 */
void mostrar_estado_comprobante(int estado) {
    switch (estado) {
        case ESTADO_PENDIENTE:
            printf("PENDIENTE DE PAGO");
            break;
        case ESTADO_PAGADO:
            printf("PAGADO");
            break;
        case ESTADO_VENCIDO:
            printf("VENCIDO");
            break;
        default:
            printf("ESTADO DESCONOCIDO");
    }
}

/*
 * Funcion: pausar_sistema
 * Descripcion: Pausa el sistema esperando que el usuario presione Enter
 * Parametros: ninguno
 * Retorno: void
 */
void pausar_sistema() {
    printf("\nPresione Enter para continuar...");
    getchar();
}

/*
 * Funcion: crear_carpetas_sistema
 * Descripcion: Crea las carpetas necesarias para el sistema de pagos
 * Parametros: ninguno
 * Retorno: void
 */
void crear_carpetas_sistema() {
    struct stat st = {0};
    
    // Crear carpeta para comprobantes
    if (stat(CARPETA_COMPROBANTES, &st) == -1) {
        if (_mkdir(CARPETA_COMPROBANTES) == 0) {
            printf("Carpeta '%s' creada exitosamente.\n", CARPETA_COMPROBANTES);
        } else {
            printf("Error al crear carpeta '%s'.\n", CARPETA_COMPROBANTES);
        }
    }
    
    // Crear carpeta para pagos
    if (stat(CARPETA_PAGOS, &st) == -1) {
        if (_mkdir(CARPETA_PAGOS) == 0) {
            printf("Carpeta '%s' creada exitosamente.\n", CARPETA_PAGOS);
        } else {
            printf("Error al crear carpeta '%s'.\n", CARPETA_PAGOS);
        }
    }
}

/*
 * Funcion: procesar_pago_por_placa
 * Descripcion: Procesa el pago de matricula buscando por placa (version simplificada)
 * Parametros: ninguno
 * Retorno: 1 si el pago fue exitoso, 0 si hubo error
 */
int procesar_pago_por_placa() {
    char placa[20];
    char buffer[100];
    ComprobanteMatricula comprobante;
    RegistroPago pago;
    
    system("cls");
    printf("\n");
    printf("=======================================================\n");
    printf("              PAGAR MATRICULA POR PLACA\n");
    printf("           AGENCIA NACIONAL DE TRANSITO\n");
    printf("=======================================================\n");
    printf("\n");
    
    // Solicitar placa del vehiculo
    printf("Ingrese la placa del vehiculo a pagar: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error al leer la placa del vehiculo.\n");
        pausar_sistema();
        return 0;
    }
    
    // Limpiar salto de linea y convertir a mayusculas
    buffer[strcspn(buffer, "\n")] = 0;
    strcpy(placa, buffer);
    for (int i = 0; placa[i]; i++) {
        placa[i] = toupper(placa[i]);
    }
    
    // Buscar comprobante pendiente para esta placa
    FILE* archivo = fopen(ARCHIVO_COMPROBANTES, "r");
    if (!archivo) {
        printf("Error: No se encontraron comprobantes en el sistema.\n");
        printf("Debe calcular la matricula primero.\n");
        pausar_sistema();
        return 0;
    }
    
    char linea[500];
    int comprobante_encontrado = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        char placa_temp[20], numero_temp[50], propietario_temp[100], tipo_temp[50], subtipo_temp[50];
        char fecha_emision_temp[20], fecha_vencimiento_temp[20];
        float total_temp;
        int estado_temp;
        
        if (sscanf(linea, "%19[^|]|%49[^|]|%99[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%f|%d",
                   placa_temp, numero_temp, propietario_temp, tipo_temp, subtipo_temp,
                   fecha_emision_temp, fecha_vencimiento_temp, &total_temp, &estado_temp) == 9) {
            
            if (strcmp(placa_temp, placa) == 0 && estado_temp == ESTADO_PENDIENTE) {
                // Encontramos el comprobante pendiente
                strcpy(comprobante.numero_comprobante, numero_temp);
                strcpy(comprobante.placa, placa_temp);
                strcpy(comprobante.fecha_emision, fecha_emision_temp);
                strcpy(comprobante.fecha_vencimiento, fecha_vencimiento_temp);
                comprobante.monto_total = total_temp;
                comprobante.estado = estado_temp;
                comprobante_encontrado = 1;
                break;
            }
        }
    }
    fclose(archivo);
    
    if (!comprobante_encontrado) {
        printf("No se encontro comprobante pendiente para la placa '%s'.\n", placa);
        printf("Opciones:\n");
        printf("1. Verifique que la placa este correcta\n");
        printf("2. Calcule la matricula primero si no lo ha hecho\n");
        printf("3. Verifique si ya pago esta matricula\n");
        pausar_sistema();
        return 0;
    }
    
    // Verificar si el comprobante esta vigente
    if (!comprobante_vigente(comprobante.fecha_vencimiento)) {
        printf("Error: El comprobante ha vencido.\n");
        printf("Fecha de vencimiento: %s\n", comprobante.fecha_vencimiento);
        printf("Debe generar un nuevo comprobante calculando la matricula nuevamente.\n");
        pausar_sistema();
        return 0;
    }
    
    // Mostrar informacion del comprobante encontrado
    printf("COMPROBANTE ENCONTRADO:\n");
    printf("=======================================================\n");
    printf("Placa: %s\n", comprobante.placa);
    printf("Numero de comprobante: %s\n", comprobante.numero_comprobante);
    printf("Fecha de emision: %s\n", comprobante.fecha_emision);
    printf("Fecha de vencimiento: %s\n", comprobante.fecha_vencimiento);
    printf("Monto a pagar: $%.2f\n", comprobante.monto_total);
    printf("=======================================================\n");
    printf("\n");
    
    // Obtener datos del propietario
    char cedula_propietario[15], nombre_propietario[100];
    if (obtener_datos_propietario(placa, cedula_propietario, nombre_propietario)) {
        printf("DATOS DEL PROPIETARIO:\n");
        printf("-------------------------------------------------------\n");
        printf("Cedula: %s\n", cedula_propietario);
        printf("Nombre: %s\n", nombre_propietario);
        printf("-------------------------------------------------------\n");
        printf("\n");
        
        strcpy(pago.cedula_pagador, cedula_propietario);
        strcpy(pago.nombre_pagador, nombre_propietario);
    } else {
        printf("Ingrese los datos del pagador:\n");
        printf("Cedula: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error al leer la cedula.\n");
            pausar_sistema();
            return 0;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(pago.cedula_pagador, buffer);
        
        printf("Nombre completo: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error al leer el nombre.\n");
            pausar_sistema();
            return 0;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(pago.nombre_pagador, buffer);
    }
    
    // Confirmar pago
    printf("CONFIRMACION DE PAGO:\n");
    printf("=======================================================\n");
    printf("Esta seguro de que desea pagar $%.2f para el vehiculo %s?\n", 
           comprobante.monto_total, placa);
    printf("=======================================================\n");
    printf("Confirmar pago? (S/N): ");
    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error al leer la confirmacion.\n");
        pausar_sistema();
        return 0;
    }
    
    if (buffer[0] != 'S' && buffer[0] != 's') {
        printf("Pago cancelado.\n");
        pausar_sistema();
        return 0;
    }
    
    // Procesar pago (simplificado - solo efectivo por ahora)
    strcpy(pago.numero_comprobante, comprobante.numero_comprobante);
    strcpy(pago.placa, comprobante.placa);
    pago.monto_pagado = comprobante.monto_total;
    pago.tipo_pago = TIPO_EFECTIVO;
    strcpy(pago.referencia_pago, "EFECTIVO");
    obtener_fecha_actual(pago.fecha_pago);
    
    // Guardar registro de pago
    if (!guardar_registro_pago(pago)) {
        printf("Error: No se pudo guardar el registro de pago.\n");
        pausar_sistema();
        return 0;
    }
    
    // Actualizar estado del comprobante
    if (!actualizar_estado_comprobante(comprobante.numero_comprobante, ESTADO_PAGADO)) {
        printf("Error: No se pudo actualizar el estado del comprobante.\n");
        pausar_sistema();
        return 0;
    }
    
    // Guardar en archivo simple de pagos realizados
    FILE* archivo_pagados = fopen("matriculas_pagadas.txt", "a");
    if (archivo_pagados) {
        fprintf(archivo_pagados, "%s|%s|%s|%.2f|PAGADO\n",
                comprobante.numero_comprobante, placa, pago.fecha_pago, pago.monto_pagado);
        fclose(archivo_pagados);
    }
    
    // Mostrar comprobante de pago exitoso
    printf("\n");
    printf("=======================================================\n");
    printf("           PAGO PROCESADO EXITOSAMENTE\n");
    printf("           AGENCIA NACIONAL DE TRANSITO\n");
    printf("=======================================================\n");
    printf("\n");
    printf("COMPROBANTE DE PAGO:\n");
    printf("-------------------------------------------------------\n");
    printf("Numero de comprobante: %s\n", pago.numero_comprobante);
    printf("Placa: %s\n", pago.placa);
    printf("Fecha de pago: %s\n", pago.fecha_pago);
    printf("Monto pagado: $%.2f\n", pago.monto_pagado);
    printf("Pagador: %s (%s)\n", pago.nombre_pagador, pago.cedula_pagador);
    printf("-------------------------------------------------------\n");
    printf("\n");
    printf("IMPORTANTE:\n");
    printf("- Su matricula ha sido pagada exitosamente\n");
    printf("- Ahora puede proceder con la revision tecnica\n");
    printf("- Conserve este comprobante como respaldo\n");
    printf("\n");
    printf("=======================================================\n");
    printf("      GRACIAS POR UTILIZAR NUESTROS SERVICIOS\n");
    printf("=======================================================\n");
    
    pausar_sistema();
    return 1;
}