# Configuración del entorno de desarrollo
 
---
 
## ¿Qué vamos a instalar?
 
| Herramienta | ¿Para qué sirve? |
|---|---|
| **iTerm2** | Terminal (más cómoda que la de Mac) |
| **clang / clang++** | Compilador de C++ |
| **Visual Studio Code** | Editor de código |
| **C/C++ extension** | Autocompletado y detección de errores |
| **Code Runner** | Ejecutar código con un click |
 
> **Glosario rápido**
> - **Terminal** → interfaz para escribir comandos de texto
> - **Compilador** → convierte tu código C++ en un programa ejecutable
> - **Ejecutable** → el programa que podés correr
> - **Compilar** → el proceso de esa conversión
 
---
 
## Paso 1 — iTerm2 (opcional pero recomendado)
 
iTerm2 es una terminal más cómoda que la que viene con Mac.
 
👉 Descargar desde: https://iterm2.com
 
Instalá y abrí iTerm2. A partir de acá usamos esta terminal.
 
> **Importante:** iTerm2 es solo la interfaz. No instala herramientas de programación — eso lo hacemos en el siguiente paso.
 
---
 
## Paso 2 — Command Line Tools (compilador)
 
Escribí esto en la terminal y presioná Enter:
 
```bash
xcode-select --install
```
 
Va a aparecer una ventana pidiendo confirmación. Aceptá e instalá.
 
Esto instala:
- `clang` / `clang++` — el compilador de C++
- Herramientas básicas de desarrollo
 
### Verificar que se instaló bien
 
```bash
clang --version
```
 
Deberías ver algo como:
```
Apple clang version 15.0.0
Target: arm64-apple-darwin
```
 
Si ves eso, el compilador está listo. ✅
 
---
 
## Paso 3 — Visual Studio Code
 
👉 Descargar desde: https://code.visualstudio.com
 
Instalá y abrí VS Code.
 
### Instalar extensiones necesarias
 
Dentro de VS Code, ir a Extensions (ícono de cuadraditos en la barra lateral) e instalar:
 
1. **C/C++** (de Microsoft) → autocompletado y detección de errores en tiempo real
2. **Code Runner** → ejecutar código con un click sin escribir comandos
 
---
 
## Paso 4 — Primer programa en C++
 
### Crear el proyecto
 
En la terminal:
 
```bash
mkdir pas2
cd pas2
touch main.cpp
```
 
Esto crea una carpeta `pas2` y un archivo vacío `main.cpp` adentro.
 
### Abrir en VS Code
 
```bash
code .
```
 
Esto abre VS Code en la carpeta actual.
 
### Escribir el código
 
Abrí `main.cpp` y escribí:
 
```cpp
#include <iostream>
 
int main() {
    std::cout << "Hola PAS2 🚀" << std::endl;
    return 0;
}
```
 
> **¿Qué significa cada línea?**
> - `#include <iostream>` → permite imprimir texto en la terminal
> - `int main()` → punto de entrada del programa, por acá empieza todo
> - `std::cout` → imprime texto
> - `std::endl` → salto de línea
> - `return 0` → el programa terminó correctamente
 
### Compilar manualmente
 
En la terminal:
 
```bash
clang++ main.cpp -o programa
```
 
> **¿Qué está pasando?**
> - `clang++` → el compilador
> - `main.cpp` → tu código fuente
> - `-o programa` → el nombre del ejecutable que se va a generar
 
### Ejecutar
 
```bash
./programa
```
 
Resultado:
```
Hola PAS2 🚀
```
 
---
 
## Paso 5 — Ejecutar desde VS Code con Code Runner
 
Una vez instalada la extensión Code Runner, podés ejecutar directamente desde el editor:
 
- Click en el botón **▶ Run Code** (arriba a la derecha)
- O usar el atajo: `Ctrl + Option + N`
 
> **¿Qué hace Code Runner internamente?**  
> Básicamente ejecuta los mismos comandos que hicimos a mano:
> ```bash
> clang++ main.cpp -o programa
> ./programa
> ```
> Solo que automáticamente y sin que tengas que escribirlos.
 
---
 
