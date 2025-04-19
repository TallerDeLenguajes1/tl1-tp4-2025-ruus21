#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Producto {
    int ProductoID;
    int Cantidad;
    char *TipoProducto;
    float PrecioUnitario;
};

struct Cliente {
    int ClienteID;
    char *NombreCliente;
    int CantidadProductosAPedir;
    struct Producto *Productos;
};

struct Tarea{
    int TareaID; // Numérico autoincremental comenzando en 1000
    char *Descripcion;
    int Duracion; // entre 10– 100
};

struct Nodo{
    struct Tarea T;
    struct Nodo *Siguiente;
};

struct Nodo *tareasPendientes = NULL;
struct Nodo *tareasRealizadas = NULL;

void cargarClientes(struct Cliente *clientes, int cantidadClientes);
void generarProductos(struct Producto *productos, int cantidad);
float calcularCostoTotal(struct Producto producto);
void mostrarInformacion(struct Cliente *clientes, int cantidadClientes);
void agregarTarea(struct Nodo **lista, struct Tarea tarea); 
void moverTarea(struct Nodo **listaOrigen, struct Nodo **listaDestino, int tareaID);
void elegirTareas(struct Nodo **pendientes, struct Nodo **realizadas);
void listarTareas(struct Nodo *lista, const char *nombre);
void cargarTareas(struct Nodo **lista);
void consultarTareas(struct Nodo *pendientes, struct Nodo *realizadas, int id);

int tareaID = 1000;

int main() {
    srand(time(NULL));

    int cantidadClientes;
    printf("Ingrese la cantidad de clientes: ");
    scanf("%d", &cantidadClientes);
    getchar(); 

    struct Cliente *clientes = (struct Cliente*) malloc(cantidadClientes * sizeof(struct Cliente));

    cargarClientes(clientes, cantidadClientes);
    mostrarInformacion(clientes, cantidadClientes);

    // Cargar tareas pendientes
    printf("\nCarga de tareas pendientes:\n");
    cargarTareas(&tareasPendientes);

    // Elegir tareas para marcar como realizadas
    printf("\nEleccion de tareas para marcar como realizadas:\n");
    elegirTareas(&tareasPendientes, &tareasRealizadas);

    // Listar todas las tareas
    printf("\nListado de todas las tareas:\n");
    listarTareas(tareasPendientes, "Tareas pendientes");
    listarTareas(tareasRealizadas, "Tareas realizadas");

    // Consultar tareas por ID
    printf("\nConsulta de tareas por ID:\n");
    printf("Ingrese el ID de la tarea que desea consultar: ");
    int id;
    scanf("%d", &id);
    getchar();
    consultarTareas(tareasPendientes, tareasRealizadas, id);

    // Liberar memoria
    for (int i = 0; i < cantidadClientes; i++) {
        free(clientes[i].NombreCliente);
        free(clientes[i].Productos);
    }
    free(clientes);

    return 0;
}


void cargarClientes(struct Cliente *clientes, int cantidadClientes) {
    for (int i = 0; i < cantidadClientes; i++) {
        printf("Ingrese el nombre del cliente %d: ", i + 1);
        char nombre[100];
        gets(nombre);
        clientes[i].NombreCliente = strdup(nombre);

        clientes[i].ClienteID = i + 1;
        clientes[i].CantidadProductosAPedir = rand() % 5 + 1;
        clientes[i].Productos = (struct Producto*) malloc(clientes[i].CantidadProductosAPedir * sizeof(struct Producto));

        generarProductos(clientes[i].Productos, clientes[i].CantidadProductosAPedir);
    }
}

void generarProductos(struct Producto *productos, int cantidad) {
    char *TiposProductos[] = {"Galletas", "Snack", "Cigarrillos", "Caramelos", "Bebidas"};

    for (int i = 0; i < cantidad; i++) {
        productos[i].ProductoID = i + 1;
        productos[i].Cantidad = rand() % 10 + 1;            
        productos[i].TipoProducto = TiposProductos[rand() % 5]; 
        productos[i].PrecioUnitario = (rand() % 91) + 10; 
    }
}

float calcularCostoTotal(struct Producto producto) {
    return producto.Cantidad * producto.PrecioUnitario;
}

void mostrarInformacion(struct Cliente *clientes, int cantidadClientes) {
    for (int i = 0; i < cantidadClientes; i++) {
        printf("Cliente %d - Nombre: %s\n", clientes[i].ClienteID, clientes[i].NombreCliente);
        float totalPagar = 0.0;
        for (int j = 0; j < clientes[i].CantidadProductosAPedir; j++) {
            printf("  Producto %d:\n", j + 1);
            printf("    ProductoID: %d\n", clientes[i].Productos[j].ProductoID);
            printf("    Cantidad: %d\n", clientes[i].Productos[j].Cantidad);
            printf("    TipoProducto: %s\n", clientes[i].Productos[j].TipoProducto);
            printf("    PrecioUnitario: %.2f\n", clientes[i].Productos[j].PrecioUnitario);
            float costoTotal = calcularCostoTotal(clientes[i].Productos[j]);
            printf("    Costo total: %.2f\n", costoTotal);
            totalPagar += costoTotal;
        }
        printf("  Total a pagar: %.2f\n", totalPagar);
    }
}

void agregarTarea(struct Nodo **lista, struct Tarea tarea) {
    struct Nodo *nuevoNodo = (struct Nodo*) malloc(sizeof(struct Nodo));
    nuevoNodo->T = tarea;
    nuevoNodo->Siguiente = *lista;
    *lista = nuevoNodo;
}

void moverTarea(struct Nodo **listaOrigen, struct Nodo **listaDestino, int tareaID) {
    struct Nodo *nodoActual = *listaOrigen;
    struct Nodo *nodoAnterior = NULL;

    while (nodoActual != NULL && nodoActual->T.TareaID != tareaID) {
        nodoAnterior = nodoActual;
        nodoActual = nodoActual->Siguiente;
    }

    if (nodoActual == NULL) {
        printf("No se encontro la tarea con ID %d.\n", tareaID);
        return;
    }

    if (nodoAnterior != NULL) {
        nodoAnterior->Siguiente = nodoActual->Siguiente;
    } else {
        *listaOrigen = nodoActual->Siguiente;
    }

    nodoActual->Siguiente = *listaDestino;
    *listaDestino = nodoActual;
}


void cargarTareas(struct Nodo **lista) {
    while (1) {
        struct Tarea tarea;
        tarea.TareaID = tareaID++;
        printf("Ingrese la descripción de la tarea: ");
        char descripcion[100];
        gets(descripcion);
        tarea.Descripcion = strdup(descripcion);
        printf("Ingrese la duracion de la tarea: ");
        scanf("%d", &tarea.Duracion);
        getchar();
        agregarTarea(lista, tarea);

        printf("¿Desea ingresar una nueva tarea? (s/n): ");
        char respuesta;
        scanf("%c", &respuesta);
        getchar();
        if (respuesta != 's' && respuesta != 'S') {
            break;
        }
    }
}

void elegirTareas(struct Nodo **pendientes, struct Nodo **realizadas) {
    while (*pendientes != NULL) {
        printf("Tareas pendientes:\n");
        struct Nodo *nodo = *pendientes;
        while (nodo != NULL) {
            printf("  ID: %d, Descripcion: %s, Duracion: %d\n", nodo->T.TareaID, nodo->T.Descripcion, nodo->T.Duracion);
            nodo = nodo->Siguiente;
        }

        printf("Ingrese el ID de la tarea que desea marcar como realizada: ");
        int id;
        scanf("%d", &id);
        getchar();
        moverTarea(pendientes, realizadas, id);
    }
}

void listarTareas(struct Nodo *lista, const char *nombre) {
    printf("%s:\n", nombre);
    struct Nodo *nodo = lista;
    while (nodo != NULL) {
        printf("  ID: %d, Descripcion: %s, Duracion: %d\n", nodo->T.TareaID, nodo->T.Descripcion, nodo->T.Duracion);
        nodo = nodo->Siguiente;
    }
}

void consultarTareas(struct Nodo *pendientes, struct Nodo *realizadas, int id) {
    struct Nodo *nodo = pendientes;
    while (nodo != NULL && nodo->T.TareaID != id) {
        nodo = nodo->Siguiente;
    }
    if (nodo != NULL) {
        printf("Tarea pendiente - ID: %d, Descripcion: %s, Duracion: %d\n", nodo->T.TareaID, nodo->T.Descripcion, nodo->T.Duracion);
        return;
    }

    nodo = realizadas;
    while (nodo != NULL && nodo->T.TareaID != id) {
        nodo = nodo->Siguiente;
    }
    if (nodo != NULL) {
        printf("Tarea realizada - ID: %d, Descripcion: %s, Duracion: %d\n", nodo->T.TareaID, nodo->T.Descripcion, nodo->T.Duracion);
    }
}
