# Sistema de Matriculación Vehicular - Ecuador

[![Lenguaje](https://img.shields.io/badge/Lenguaje-C-blue.svg)](https://gcc.gnu.org/)
[![Plataforma](https://img.shields.io/badge/Plataforma-Windows-green.svg)](https://www.microsoft.com/windows)
[![Universidad](https://img.shields.io/badge/Universidad-EPN-red.svg)](https://www.epn.edu.ec/)

##  Descripción

Sistema completo de matriculación vehicular para Ecuador, desarrollado en lenguaje C. Este sistema permite gestionar el proceso completo de matriculación vehicular, incluyendo registro de vehículos, cálculo de costos, generación de comprobantes y procesamiento de pagos.

## 👥 Autores

- **Mathias**
- **Jhostin** 
- **Christian**

**Materia:** Programación I - ICCD144  
**Universidad:** Escuela Politécnica Nacional  
**Año:** 2025

## Características

### Funcionalidades Principales
-  **Registro de Vehículos**: Registro completo de vehículos con validación de datos
-  **Cálculo de Matrícula**: Cálculo automático de tasas, impuestos y costos
-  **Generación de Comprobantes**: Comprobantes detallados con número único
-  **Sistema de Pagos**: Procesamiento de pagos con múltiples métodos
-  **Listado de Vehículos**: Consulta de vehículos matriculados
-  **Reportes Detallados**: Estadísticas y reportes del sistema
-  **Autenticación**: Sistema de login con usuarios y contraseñas

### Características Técnicas
-  **Autenticación segura** con contraseña oculta
-  **Organización automática** de archivos en carpetas
-  **Persistencia de datos** en archivos de texto
-  **Validación completa** de datos de entrada
-  **Reportes estadísticos** detallados

##  Estructura del Proyecto

```
MiProyecto/
├── main_new.c          # Programa principal y menús
├── vehiculos.c         # Gestión de vehículos
├── vehiculos.h         # Definiciones de vehículos
├── matricula.c         # Cálculo de matrícula
├── matricula.h         # Definiciones de matrícula
├── pagos.c             # Sistema de pagos
├── pagos.h             # Definiciones de pagos
├── usuarios.txt        # Base de datos de usuarios
├── vehiculos.txt       # Base de datos de vehículos
├── comprobantes/       # Carpeta de comprobantes
│   ├── comprobantes.txt
│   └── comprobante_*.txt
└── pagos/              # Carpeta de pagos
    ├── pagos.txt
    └── recibo_*.txt
```

##  Compilación y Ejecución

### Requisitos
- Compilador GCC
- Sistema operativo Windows
- Biblioteca estándar de C

### Compilar el proyecto
```bash
gcc -o MiProyecto.exe main_new.c matricula.c vehiculos.c pagos.c
```

### Ejecutar el programa
```bash
./MiProyecto.exe
```

##  Manual de Usuario

### 1. Inicio de Sesión
- Ejecutar el programa
- Ingresar usuario y contraseña
- Opción para registrar nuevo usuario (presionar '0')

### 2. Menú Principal
1. **[REGISTRO]** - Registrar nuevo vehículo
2. **[BUSCAR]** - Buscar vehículo por placa
3. **[CALCULO]** - Calcular matrícula vehicular
4. **[PAGOS]** - Procesar pago de matrícula
5. **[LISTADO]** - Ver vehículos matriculados
6. **[REPORTE]** - Reporte detallado de vehículos
7. **[TARIFAS]** - Mostrar tarifas vigentes

### 3. Proceso de Matriculación
1. Registrar vehículo (si no existe)
2. Calcular matrícula (genera comprobante)
3. Procesar pago con número de comprobante
4. Verificar en listado de vehículos matriculados

## 🔧 Funcionalidades Detalladas

### Sistema de Validación
- Validación de placas ecuatorianas (formato ABC-1234)
- Validación de cédulas ecuatorianas (algoritmo oficial)
- Validación de datos de vehículos
- Validación de números de comprobante

### Cálculo de Costos
- Impuesto a la propiedad vehicular
- Impuesto de rodaje
- Tasas ANT (Agencia Nacional de Tránsito)
- Tasas de prefectura
- Revisión técnica vehicular (RTV)
- Recargos por mora
- Multas pendientes

### Sistema de Pagos
- Pago en efectivo
- Pago con tarjeta de crédito/débito
- Transferencia bancaria
- Generación de recibos de pago
- Historial de pagos

## 📊 Reportes y Consultas

### Listado de Vehículos Matriculados
Muestra todos los vehículos con comprobantes generados, incluyendo:
- Placa del vehículo
- Número de comprobante
- Propietario
- Tipo de vehículo
- Monto total
- Estado del pago

### Reporte Detallado
Incluye estadísticas completas:
- Total de vehículos matriculados
- Comprobantes pagados, pendientes y vencidos
- Total recaudado
- Porcentaje de pagos efectivos

##  Desarrollo y Contribución

### Ramas del Proyecto
- `main`: Rama principal (código estable)
- `desarrollo`: Rama de desarrollo activo
- `feature/*`: Ramas para nuevas funcionalidades
- `backup/*`: Ramas de respaldo automático

### Estructura de Commits
```
tipo(alcance): descripción breve

- feat: nueva funcionalidad
- fix: corrección de errores
- docs: documentación
- refactor: refactorización de código
- test: pruebas
- backup: respaldo automático
```

##  Sistema de Backup

El proyecto incluye un sistema automático de backup que:
- Crea copias de seguridad diarias
- Mantiene versiones históricas
- Protege contra pérdida de datos
- Permite restauración rápida

##  Notas Técnicas

### Archivos de Datos
- **usuarios.txt**: Almacena credenciales de usuarios
- **vehiculos.txt**: Base de datos de vehículos registrados
- **comprobantes.txt**: Registro de comprobantes generados
- **pagos.txt**: Historial de pagos procesados

### Formatos de Archivo
- Separador de campos: coma (,) para vehículos, pipe (|) para comprobantes
- Codificación: UTF-8
- Formato de fecha: DD/MM/YYYY HH:MM

##  Problemas Conocidos

- El sistema está optimizado para Windows
- Requiere permisos de escritura en la carpeta de ejecución
- Las fechas se manejan en formato local del sistema

##  Licencia

Este proyecto es desarrollado con fines académicos para la Escuela Politécnica Nacional.

##  Contacto

Para soporte técnico o consultas sobre el proyecto, contactar a los autores a través de los canales oficiales de la universidad.

---

**Versión:** 1.0  
**Última actualización:** Julio 2025  
**Estado:** Activo en desarrollo
