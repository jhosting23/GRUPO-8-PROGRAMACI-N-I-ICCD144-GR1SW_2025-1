# Sistema de Matriculación Vehicular - Ecuador

[![Lenguaje](https://img.shields.io/badge/Lenguaje-C-blue.svg)](https://gcc.gnu.org/)
[![Plataforma](https://img.shields.io/badge/Plataforma-Windows-green.svg)](https://www.microsoft.com/windows)
[![Universidad](https://img.shields.io/badge/Universidad-EPN-red.svg)](https://www.epn.edu.ec/)

---

## Descripción

Sistema completo de matriculación vehicular para Ecuador, desarrollado en C. Permite gestionar el proceso integral de matriculación: registro de vehículos, cálculo de costos, generación de comprobantes, pagos, reportes y autenticación de usuarios.

---

## Autores

| Nombre      | Rol         | Universidad                  |
|-------------|-------------|-----------------------------|
| Mathias     | Desarrollador | Escuela Politécnica Nacional |
| Jhostin     | Desarrollador | Escuela Politécnica Nacional |
| Christian   | Desarrollador | Escuela Politécnica Nacional |

**Materia:** Programación I - ICCD144  
**Año:** 2025

---

## Funcionalidades Principales

| Funcionalidad                  | Descripción                                                                 |
|--------------------------------|-----------------------------------------------------------------------------|
| Registro de Vehículos          | Alta y validación de datos de vehículos                                     |
| Cálculo de Matrícula           | Cálculo automático de tasas, impuestos y costos                             |
| Generación de Comprobantes     | Comprobantes detallados con número único                                    |
| Sistema de Pagos               | Procesamiento de pagos con múltiples métodos                                |
| Listado de Vehículos           | Consulta de vehículos matriculados                                          |
| Reportes Detallados            | Estadísticas y reportes del sistema                                         |
| Autenticación de Usuarios      | Login seguro con contraseña oculta                                          |
| Backup Automático              | Copias de seguridad diarias y por cada push                                 |

---

## Estructura del Proyecto

```
MiProyecto/
├── main.c                # Programa principal y menús
├── vehiculos.c/h         # Gestión y validación de vehículos
├── matricula.c/h         # Cálculo de matrícula y comprobantes
├── pagos.c/h             # Sistema de pagos y recibos
├── usuarios.txt          # Base de datos de usuarios
├── vehiculos.txt         # Base de datos de vehículos
├── comprobantes/         # Carpeta de comprobantes
│   ├── comprobantes.txt
│   └── comprobante_*.txt
└── pagos/                # Carpeta de pagos
    ├── pagos.txt
    └── recibo_*.txt
```

---

## Compilación y Ejecución

**Requisitos:**
- Compilador GCC (recomendado: MinGW para Windows)
- Sistema operativo Windows
- Biblioteca estándar de C

**Compilar el proyecto:**
```bash
cd MiProyecto
gcc -o MiProyecto.exe main.c matricula.c vehiculos.c pagos.c
```

**Ejecutar el programa:**
```bash
./MiProyecto.exe
```


https://github.com/user-attachments/assets/7cfbb74f-3de7-446b-b985-4c0b0a661dc8


---

## Manual de Usuario

1. Inicie el programa y acceda con su usuario y contraseña.
2. Use el menú principal para registrar vehículos, calcular matrícula, procesar pagos y consultar listados o reportes.
3. Siga las instrucciones en pantalla para cada opción. El sistema valida los datos y guía el flujo paso a paso.
4. Para registrar un nuevo usuario, seleccione la opción correspondiente en el inicio de sesión.

---

## Reportes y Consultas

- **Listado de Vehículos Matriculados:**
  Muestra todos los vehículos con comprobantes generados, incluyendo placa, propietario, tipo, monto y estado de pago.

- **Reporte Detallado:**
  Estadísticas completas: total de vehículos, comprobantes pagados, pendientes, vencidos y total recaudado.

---

## Seguridad y Backup

- Contraseñas ocultas al ingresar
- Validación de datos (placa, cédula, etc.)
- Backup automático diario y por cada push (ver carpeta `.github/workflows/backup.yml`)
- Script local de backup: `backup.bat`

---

## Ramas del Proyecto

- `main`: Rama principal (código estable)
- `desarrollo`: Rama de desarrollo activo
- `backup/*`: Ramas de respaldo automático

---

## Herramientas para Desarrolladores

- `dev.bat`: Script interactivo para compilar, ejecutar, hacer backup y más.
- `.gitignore`: Ignora archivos temporales, ejecutables y carpetas de build.
- `README.md`: Documentación profesional y actualizada.

---

## Preguntas Frecuentes

**¿Por qué no veo vehículos en el listado?**  
Asegúrese de haber generado y pagado un comprobante correctamente. El listado solo muestra vehículos con comprobante generado.

**¿Cómo restauro un backup?**  
Copia los archivos desde la carpeta `backups/` a la raíz del proyecto o usa la rama de backup en GitHub.

**¿Puedo usar este sistema en Linux?**  
El sistema está optimizado para Windows, pero el código es portable con pequeños ajustes.

---

## Contacto

¿Dudas o sugerencias?  
Contacta a los autores a través de los canales oficiales de la universidad o abre un issue en el repositorio.

---

**Versión:** 1.0  
**Última actualización:** Julio 2025  
**Estado:** Activo en desarrollo
