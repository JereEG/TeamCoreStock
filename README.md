# TeamCoreStock

Sistema de gestión de inventario y stock desarrollado en lenguaje **C** como Proyecto Integrador para la materia **Algoritmos y Estructuras de Datos I** (listas simplemente enlazadas, archivos binarios y corte de control).

> **Fe de erratas:** En septiembre de 2021 se subió por error el archivo de práctica `pilaProductosVideo.c` en lugar del código real del proyecto. Este repositorio fue corregido para incluir el proyecto integrador completo, su archivo de cabecera `listasEnlazadas.h`, el archivo binario de datos de prueba `Productos.dat` y esta guía de uso.

---

## Estructura del proyecto

```text
TeamCoreStock/
├── .gitignore
├── README.md
└── Proyecto Integrador/
    ├── teamCoreStock.c      # Programa principal (menú, archivos y corte de control)
    ├── listasEnlazadas.h    # TAD Lista simplemente enlazada y tipo de registro tDato
    └── Productos.dat        # Archivo binario precargado con 17 productos de prueba
```

> **Importante:** Los tres archivos dentro de `Proyecto Integrador/` deben permanecer juntos en la misma carpeta para que el programa compile y encuentre `Productos.dat` al ejecutarse.

---

## Cómo probarlo en Dev-C++ (Guía para alumnos)

Si usás **Dev-C++** (por ejemplo, Dev-C++ 5.11 con TDM-GCC) en Windows, seguí estos pasos:

1. **Descargar el repositorio:**
   - Hacé clic en el botón verde **`<> Code`** en GitHub y luego en **`Download ZIP`**.
   - Descomprimí el archivo `.zip` en una carpeta accesible (por ejemplo, `Documentos\TeamCoreStock`).
   - *(Opcional, si usás Git)*:
     ```bash
     git clone https://github.com/JereEG/TeamCoreStock.git
     ```
2. **Abrir el código fuente en Dev-C++:**
   - Abrí **Dev-C++**.
   - Andá a **`Archivo` → `Abrir...`** (o presioná `Ctrl + O`).
   - Navegá hasta la carpeta `Proyecto Integrador` y seleccioná **`teamCoreStock.c`**.
   - *Nota:* No hace falta crear un proyecto `.dev` nuevo; `teamCoreStock.c` incluye directamente a `"listasEnlazadas.h"` desde la misma carpeta.
3. **Compilar y ejecutar:**
   - Presioná **`F11`** (o hacé clic en **`Ejecutar` → `Compilar y Ejecutar`**).
   - Dev-C++ compilará `teamCoreStock.exe` dentro de `Proyecto Integrador/` y abrirá la consola automáticamente.
4. **Probar con los datos precargados:**
   - Ingresá la opción **`7`** para listar los 17 productos ya guardados en `Productos.dat`.
   - Ingresá la opción **`8`** para ver el informe completo con **Corte de Control** agrupado por categoría.
   - También podés cargar productos nuevos en memoria (`1`), buscarlos (`2`), eliminarlos (`3`), listarlos (`4`) y guardarlos en el archivo (`5` para agregar o `6` para sobrescribir).
   - Para salir correctamente, elegí la opción **`0`**.

### Solución a problemas comunes en Dev-C++

- **`listasEnlazadas.h: No such file or directory`**
  - Ocurre si moviste `teamCoreStock.c` a otra carpeta sin copiar `listasEnlazadas.h` junto a él. Mantené ambos archivos en la misma carpeta.
- **`Error al abrir el archivo 'Productos.dat'!` al elegir la opción `7` u `8`**
  - Ocurre si `Productos.dat` no está en la misma carpeta donde se ejecuta el programa (o si ejecutaste el `.exe` desde otro directorio en consola). Asegurate de trabajar dentro de `Proyecto Integrador/`.
  - *Nota:* Si el archivo todavía no existe y cargás productos con la opción `1`, al elegir la opción `5` o `6` el programa creará un nuevo `Productos.dat`.
- **El antivirus (Windows Defender) bloquea o demora el `.exe` al presionar `F11`**
  - Es habitual con ejecutables recién compilados por MinGW/TDM-GCC. Si pasa, cerrá la consola, agregá la carpeta de estudio a exclusiones o volvé a presionar `F11`.
- **La ventana se cierra inmediatamente al abrir `teamCoreStock.exe` con doble clic**
  - Ejecutalo desde **Dev-C++** (`F11` o `F10`), que usa `ConsolePauser` para pausar al finalizar, o desde una terminal (`CMD` / `PowerShell`).

---

## Cómo probarlo desde terminal (GCC / MinGW / Linux / macOS)

Si preferís compilar desde línea de comandos, es importante ubicarse primero dentro de `Proyecto Integrador` para que el programa encuentre `Productos.dat` por ruta relativa:

### Windows (PowerShell / CMD con MinGW o GCC)

```powershell
cd "Proyecto Integrador"
gcc -Wall -Wextra teamCoreStock.c -o teamCoreStock.exe
.\teamCoreStock.exe
```

### Linux / macOS

```bash
cd "Proyecto Integrador"
gcc -Wall -Wextra teamCoreStock.c -o teamCoreStock
./teamCoreStock
```

---

## Funcionalidades del sistema

El menú principal ofrece las siguientes operaciones:

| Opción | Descripción |
| :---: | :--- |
| `1` | **Ingresar un producto:** carga un producto en la lista enlazada en memoria (código, descripción de hasta 24 caracteres, cantidad, categoría, costo y precio). |
| `2` | **Buscar producto por código:** busca e imprime los datos del producto dentro de la lista en memoria. |
| `3` | **Eliminar producto por código:** elimina de la lista en memoria las coincidencias con el código ingresado. |
| `4` | **Ver productos en lista:** muestra todos los productos actualmente cargados en memoria. |
| `5` | **Añadir productos al archivo:** agrega los productos de la lista al final de `Productos.dat` (modo `"ab"`). |
| `6` | **Sobrescribir archivo:** reemplaza todo el contenido de `Productos.dat` con los productos de la lista actual (modo `"wb"`). *Usar con cuidado si no se quiere perder el stock precargado.* |
| `7` | **Ver productos guardados:** lee y muestra todos los registros almacenados en `Productos.dat`. |
| `8` | **Ver informe de productos (Corte de Control):** agrupa los registros consecutivos por categoría, informa el producto con menor cantidad dentro de cada categoría, valor de stock, costo y ganancia estimada por categoría y totales generales. |
| `0` | **Finalizar programa:** libera la memoria de la lista enlazada y cierra el programa. |

### Categorías disponibles

1. `Comestibles`
2. `Tecnologia`
3. `Productos del Hogar`
4. `Farmacia`
5. `Ropa`
6. `Otros`

---

## Datos de prueba y notas técnicas

- **Archivo `Productos.dat`:** viene precargado con **17 productos** distribuidos en las 6 categorías, ya agrupados por `codCategoria` para poder probar directamente el listado (`7`) y el informe por corte de control (`8`).
- **Comportamiento del Corte de Control (opción `8`):** el algoritmo procesa el archivo secuencialmente asumiendo que los registros están agrupados por `codCategoria`. Si agregás productos nuevos con la opción `5` (al final del archivo) cuya categoría ya aparecía antes, volverá a abrir un bloque de corte al leerlos al final del archivo.
- **Formato binario de registros:** `Productos.dat` almacena estructuras `tDato` de **48 bytes** (alineación estándar `x86/x64` little-endian en Windows/GCC). Si compilás en una arquitectura distinta o querés iniciar un inventario limpio desde cero, podés borrar o renombrar `Productos.dat`, cargar tus productos con la opción `1` y guardarlos con la opción `6`.
