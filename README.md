# Proyecto2_SISTOS

## Descripción

Este proyecto implementa un simulador visual interactivo que permite estudiar y analizar diferentes algoritmos de planificación de procesos y mecanismos de sincronización. La aplicación está desarrollada en C++ utilizando la biblioteca wxWidgets para la interfaz gráfica.

## Características Principales

### Simulador de Algoritmos de Calendarización
- **First In First Out (FIFO):** Ejecuta procesos en orden de llegada
- **Shortest Job First (SJF):** Prioriza procesos con menor tiempo de ejecución
- **Shortest Remaining Time (SRT):** Versión preemptiva de SJF
- **Round Robin (RR):** Asigna quantum de tiempo configurable a cada proceso
- **Priority Scheduling:** Ejecuta procesos según su prioridad

### Simulador de Mecanismos de Sincronización
- **Mutex Locks:** Control de acceso mutuamente exclusivo
- **Semáforos:** Manejo de recursos con contadores configurables

### Visualización Dinámica
- Diagrama de Gantt en tiempo real
- Diferenciación visual por colores para cada proceso
- Scroll horizontal automático para simulaciones largas
- Indicador de ciclo actual
- Métricas de eficiencia (tiempo de espera promedio)

## Requisitos del Sistema

### Dependencias
- **Compilador:** C++17 o superior (g++, clang++, MSVC)
- **Biblioteca gráfica:** wxWidgets 3.1 o superior
- **Sistema operativo:** Windows, Linux, o macOS

### Instalación de wxWidgets

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libwxgtk3.0-gtk3-dev
```

#### Windows (vcpkg)
```bash
vcpkg install wxwidgets
```

#### macOS (Homebrew)
```bash
brew install wxwidgets
```

## Compilación

### Usando CMake (Recomendado)
```bash
mkdir build
cd build
cmake ..
make
```

### Compilación Manual
```bash
g++ -std=c++17 $(wx-config --cxxflags) -Iinclude \
    src/**/*.cpp main.cpp \
    $(wx-config --libs) -o simulador
```

## Ejecución

```bash
./simulador
```

## Formatos de Archivos

### Archivo de Procesos
**Formato:** `<PID>, <BurstTime>, <ArrivalTime>, <Priority>`

**Ejemplo (procesos.txt):**
```
P1, 8, 0, 1
P2, 4, 1, 2
P3, 2, 3, 1
P4, 6, 2, 3
```

### Archivo de Recursos (Solo Sincronización)
**Formato:** `<NombreRecurso>, <Contador>`

**Ejemplo (recursos.txt):**
```
R1, 1
R2, 2
R3, 1
```

### Archivo de Acciones (Solo Sincronización)
**Formato:** `<PID>, <ACCION>, <RECURSO>, <CICLO>`

**Ejemplo (acciones.txt):**
```
P1, READ, R1, 0
P2, WRITE, R1, 1
P1, READ, R2, 2
P3, WRITE, R2, 3
```

## Instrucciones de Uso

### Simulación de Calendarización

1. **Seleccionar Algoritmo:** Escoge uno de los cinco algoritmos disponibles
2. **Configurar Quantum:** Solo para Round Robin, establece el valor del quantum
3. **Cargar Procesos:** Utiliza el botón "Cargar Procesos" para seleccionar el archivo
4. **Iniciar Simulación:** Presiona "Iniciar" para comenzar la visualización
5. **Controlar Simulación:** Usa "Detener" para pausar y "Reiniciar" para empezar de nuevo

### Simulación de Sincronización

1. **Seleccionar Mecanismo:** Elige entre Mutex Locks o Semáforos
2. **Cargar Archivos:** 
   - Carga procesos, recursos y acciones en ese orden
   - Todos los archivos son requeridos para iniciar
3. **Ejecutar Simulación:** Inicia la simulación una vez cargados todos los archivos
4. **Observar Estados:** 
   - Verde: Acceso exitoso al recurso
   - Rojo: Proceso en espera

## Estructura del Proyecto

```
simulador/
├── include/
│   ├── Core/              # Clases fundamentales
│   │   ├── Action.h
│   │   ├── Process.h
│   │   ├── Resource.h
│   │   └── SimulationClock.h
│   ├── GUI/               # Interfaz gráfica
│   │   ├── GanttChartPanel.h
│   │   ├── MainFrame.h
│   │   ├── SchedulerPanel.h
│   │   ├── SimulationPanel.h
│   │   └── SynchronizationPanel.h
│   ├── Scheduler/         # Algoritmos de calendarización
│   │   ├── BaseScheduler.h
│   │   ├── FIFOScheduler.h
│   │   ├── SJFScheduler.h
│   │   ├── SRTScheduler.h
│   │   ├── RoundRobinScheduler.h
│   │   └── PriorityScheduler.h
│   ├── Synchronization/   # Mecanismos de sincronización
│   │   ├── BaseSynchronization.h
│   │   ├── MutexLock.h
│   │   └── Semaphore.h
│   └── Utils/             # Utilidades
│       ├── ColorManager.h
│       └── FileParser.h
├── src/                   # Implementaciones
├── main.cpp              # Punto de entrada
└── README.md
```

## Características Implementadas


**Simulación de Calendarización:**
- Implementación de 5 algoritmos de scheduling
- Carga dinámica desde archivos de texto
- Visualización en tiempo real con Diagrama de Gantt
- Quantum configurable para Round Robin
- Cálculo de métricas de eficiencia
- Diferenciación visual por colores

**Simulación de Sincronización:**
- Implementación de Mutex y Semáforos
- Carga de procesos, recursos y acciones
- Visualización de estados (ACCESSED/WAITING)
- Manejo de colas de espera
- Control de acceso a recursos

**Interfaz Gráfica:**
- Interfaz intuitiva con pestañas
- Controles de simulación (Iniciar/Detener/Reiniciar)
- Scroll horizontal dinámico
- Visualización de información cargada
- Guardado de resultados

## Solución de Problemas

### Error: "wx-config not found"
```bash
# Ubuntu/Debian
export PATH=$PATH:/usr/local/bin
sudo ldconfig

# Verificar instalación
wx-config --version
```

### Error de Compilación C++17
```bash
# Verificar versión del compilador
g++ --version

# Compilar con flag explícito
g++ -std=c++17 ...
```

### Archivos de Entrada No Válidos
- Verificar que no haya espacios extra al inicio/final de líneas
- Comprobar que todos los valores sean numéricos donde corresponde
- Asegurar que el formato de comas sea consistente

## Métricas y Análisis

El simulador calcula automáticamente:
- **Tiempo de Espera Promedio:** Para todos los algoritmos de scheduling
- **Tiempo de Finalización:** Cuándo termina cada proceso
- **Eficiencia del Algoritmo:** Comparación entre diferentes estrategias

## Limitaciones Conocidas

- Los archivos de entrada deben seguir exactamente el formato especificado
- La simulación se ejecuta paso a paso (no en tiempo real del sistema)
- Los valores de entrada deben ser enteros positivos

### Arquitectura del Código
- **Patrón Observer:** Para actualización de la interfaz
- **Factory Pattern:** Para creación de schedulers
- **Strategy Pattern:** Para intercambio de algoritmos
- **Programación Defensiva:** Validación de entrada y manejo de errores

