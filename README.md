# Pipex & Pipex_Bonus

Un proyecto diseñado para profundizar en los mecanismos de redirección de UNIX, la gestión de procesos (`fork`), descriptores de archivo (`dup2`) y tuberías (`pipe`).
Este repositorio contiene tanto la implementación de la **Parte Obligatoria** como las extensiones avanzadas de la **Parte Bonus**.

---

## Conceptos Generales (Comunes a ambos)

Tanto la parte obligatoria como el bonus comparten la misma lógica estructural interna:
* **Gestión de Procesos:** Uso de `fork` para crear procesos hijos independientes que ejecutan los comandos en paralelo.
* **Redirecciones:** Uso de `dup2` para clonar descriptores de archivo y redirigir la entrada estándar (`STDIN`) y salida estándar (`STDOUT`).
* **Tuberías (`pipe`):** Creación de canales de comunicación unidireccionales para transferir la salida de un comando hacia la entrada del siguiente.
* **Búsqueda de Binarios:** Extracción y parsing de la variable de entorno `PATH` para verificar la existencia y permisos de los ejecutables mediante `access`.

---

## Estructura del Proyecto

El código está organizado de manera modular, compartiendo utilidades esenciales pero aislando los puntos de entrada de cada binario:

```text
.
├── includes
│   ├── pipex.h             # Cabecera de la parte obligatoria
│   └── pipex_bonus.h       # Cabecera de la parte bonus
├── Libft                   # Biblioteca Libft + GNL personalizado
├── Pipex                   # Fuentes base compartidos y parte obligatoria
│   ├── files.c             # Gestión de errores y apertura de archivos
│   ├── main.c              # Punto de entrada de la parte obligatoria
│   ├── pipex.c             # Lógica de forks para 2 comandos
│   ├── process.c           # Búsqueda de rutas (PATH) y ejecución (execve)
│   └── utils.c             # Funciones de liberación de memoria (free)
├── pipex_bonus             # Fuentes específicos de la lógica extendida
│   ├── main_bonus.c        # Punto de entrada de la parte bonus
│   └── pipex_bonus.c       # pipeline secuencial (N pipes) y here_doc
└── Makefile

```

---

## Parte Obligatoria: Dos Comandos

La parte obligatoria replica el comportamiento de un pipe simple de la shell: `< archivo_origen comando1 | comando2 > archivo_destino`.

### Compilación y Uso

```bash
make
./pipex archivo_origen "comando1" "comando2" archivo_destino

```

* Abre `archivo_origen` en modo lectura.
* Crea un pipe, ejecuta ambos comandos en procesos hijos comunicados entre sí.
* Escribe el resultado final truncando (`O_TRUNC`) el `archivo_destino`.

---

## Parte Bonus: Singularidades Extendidas

El bonus introduce dos características dinámicas que rompen la restricción de usar solo dos procesos hijos:

### 1. Múltiples Pipes Secuenciales ($N$ comandos)

Permite encadenar un número indeterminado de comandos de forma consecutiva mediante un bucle dinámico de pipes.

```bash
make bonus
./pipex infile.txt "cat" "grep a" "wc -l" outfile.txt

```

*(Equivalente exacto en la shell a: `< infile.txt cat | grep a | wc -l > outfile.txt`)*

### 2. Simulación de `here_doc` (Modo Append)

Sustituye el archivo de entrada por un flujo de lectura en vivo desde la terminal (`stdin`) que simula el delimitador de Bash.

```bash
make bonus
./pipex here_doc LIMITADOR "cat" "wc -l" outfile.txt

```

* **Prompt interactivo:** Lee continuamente mostrando `heredoc> ` hasta que introduces la palabra exacta `LIMITADOR` en una sola línea.
* **Modo Append (`O_APPEND`):** A diferencia de la parte obligatoria, si el `outfile.txt` ya existe, el resultado del heredoc se añade al final del archivo en lugar de borrar lo anterior.

---

## Reglas de Limpieza

Eliminar archivos objeto (`.o`):

```bash
make clean

```

Eliminar objetos, binario ejecutable y la librería `libft.a`:

```bash
make fclean

```

Recompilar todo desde cero:

```bash
make re

```