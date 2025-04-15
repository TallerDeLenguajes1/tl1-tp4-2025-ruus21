/*) Considere la siguiente situación: En la misma distribuidora del práctico anterior ahora surgió
la necesidad de llevar un control de las tareas realizadas por sus empleados. Usted forma
parte del equipo de programación que se encargará de hacer el módulo en cuestión que a
partir de ahora se llamará módulo To-Do:
Tareas
Cada empleado tiene un listado de tareas a realizar y debe indicar en el sistema si fueron
realizadas o no. Para ello deberá crear dos listas enlazadas: una para las tareas pendientes y
otra para las tareas realizadas. Cada vez que se marque una tarea como realizada deberá
mover la tarea de la lista de tareas pendientes a la lista de tareas realizadas.
Las estructuras de datos necesarias son las siguientes:
struct Tarea{
int TareaID;//Numérico autoincremental comenzando en 1000
char *Descripcion; //
int Duracion; // entre 10 – 100
};
struct Nodo{
Tarea T;
Nodo *Siguiente;

1) Desarrolle una interfaz de carga de tareas para solicitar tareas pendientes, en la cual se
solicite descripción y duración de la misma (el id debe ser generado automáticamente por
el sistema, de manera autoincremental comenzando desde el número 1000). Al cabo de
cada tarea consulte al usuario si desea ingresar una nueva tarea o finalizar la carga.
2) Implemente una interfaz para elegir qué tareas de la lista de pendientes deben ser
transferidas a la lista de tareas realizadas.
3) Implemente una funcionalidad que permita listar todas las tareas pendientes y realizadas.
4) Implemente una funcionalidad que permita consultar tareas por id o palabra clave y
mostrarlas por pantalla, indicando si corresponde a una tarea pendiente o realizada.
};*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === ESTRUCTURAS ORIGINALES ===

typedef struct tarea {
    int TareaID;
    char *Descripcion;
    int Duracion;
} Tarea;

typedef struct nodo {
    Tarea T;
    struct nodo *Siguiente;
} Nodo;

typedef Nodo * Lista;

Lista crearListaVacia() {
    return NULL;
}

// === VARIABLES GLOBALES ===

int idGlobal = 1000;

// === FUNCIONES ===

int generarID() {
    return idGlobal++;
}

Nodo *crearNodo(char *desc, int duracion) {
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->T.TareaID = generarID();
    nuevo->T.Descripcion = strdup(desc);
    nuevo->T.Duracion = duracion;
    nuevo->Siguiente = NULL;
    return nuevo;
}

void agregarTarea(Lista *lista, Nodo *nodo) {
    nodo->Siguiente = *lista;
    *lista = nodo;
}

void mostrarTarea(Tarea t) {
    printf("ID: %d | Duración: %d | Descripción: %s\n", t.TareaID, t.Duracion, t.Descripcion);
}

void listarTareas(Lista lista, const char *titulo) {
    printf("\n--- %s ---\n", titulo);
    if (lista == NULL) {
        printf("No hay tareas.\n");
        return;
    }
    Nodo *actual = lista;
    while (actual != NULL) {
        mostrarTarea(actual->T);
        actual = actual->Siguiente;
    }
}

Nodo *buscarYRemoverTarea(Lista *lista, int id) {
    Nodo *actual = *lista, *anterior = NULL;
    while (actual != NULL) {
        if (actual->T.TareaID == id) {
            if (anterior == NULL)
                *lista = actual->Siguiente;
            else
                anterior->Siguiente = actual->Siguiente;
            actual->Siguiente = NULL;
            return actual;
        }
        anterior = actual;
        actual = actual->Siguiente;
    }
    return NULL;
}

void buscarTareaPorID(Lista pendientes, Lista realizadas, int id) {
    Nodo *aux = pendientes;
    while (aux != NULL) {
        if (aux->T.TareaID == id) {
            printf("Tarea encontrada en PENDIENTES:\n");
            mostrarTarea(aux->T);
            return;
        }
        aux = aux->Siguiente;
    }
    aux = realizadas;
    while (aux != NULL) {
        if (aux->T.TareaID == id) {
            printf("Tarea encontrada en REALIZADAS:\n");
            mostrarTarea(aux->T);
            return;
        }
        aux = aux->Siguiente;
    }
    printf("No se encontró ninguna tarea con ID %d.\n", id);
}

void buscarTareaPorPalabra(Lista pendientes, Lista realizadas, char *palabra) {
    int encontrada = 0;
    Nodo *aux = pendientes;
    while (aux != NULL) {
        if (strstr(aux->T.Descripcion, palabra)) {
            if (!encontrada) printf("Coincidencias con '%s':\n", palabra);
            printf("Pendiente: ");
            mostrarTarea(aux->T);
            encontrada = 1;
        }
        aux = aux->Siguiente;
    }
    aux = realizadas;
    while (aux != NULL) {
        if (strstr(aux->T.Descripcion, palabra)) {
            if (!encontrada) printf("Coincidencias con '%s':\n", palabra);
            printf("Realizada: ");
            mostrarTarea(aux->T);
            encontrada = 1;
        }
        aux = aux->Siguiente;
    }
    if (!encontrada)
        printf("No se encontró ninguna tarea con esa palabra.\n");
}

void liberarLista(Lista *lista) {
    Nodo *aux;
    while (*lista) {
        aux = *lista;
        *lista = (*lista)->Siguiente;
        free(aux->T.Descripcion);
        free(aux);
    }
}

// === MAIN ===

int main() {
    Lista pendientes = crearListaVacia();
    Lista realizadas = crearListaVacia();
    int opcion;

    do {
        printf("\n=== MENÚ ===\n");
        printf("1. Cargar tareas pendientes\n");
        printf("2. Marcar tareas como realizadas\n");
        printf("3. Listar tareas\n");
        printf("4. Buscar tarea por ID o palabra clave\n");
        printf("0. Salir\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        if (opcion == 1) {
            char descripcion[256];
            int duracion;
            char seguir;

            do {
                printf("\nDescripción: ");
                gets(descripcion); // ⚠️ Uso de gets

                do {
                    printf("Duración (10-100): ");
                    scanf("%d", &duracion);
                    getchar();
                } while (duracion < 10 || duracion > 100);

                Nodo *nuevo = crearNodo(descripcion, duracion);
                agregarTarea(&pendientes, nuevo);

                printf("¿Cargar otra tarea? (s/n): ");
                scanf("%c", &seguir);
                getchar();

            } while (seguir == 's' || seguir == 'S');

        } else if (opcion == 2) {
            listarTareas(pendientes, "TAREAS PENDIENTES");
            int id;
            printf("ID de la tarea a marcar como realizada: ");
            scanf("%d", &id);
            getchar();
            Nodo *movida = buscarYRemoverTarea(&pendientes, id);
            if (movida) {
                agregarTarea(&realizadas, movida);
                printf("Tarea marcada como realizada.\n");
            } else {
                printf("ID no encontrado.\n");
            }

        } else if (opcion == 3) {
            listarTareas(pendientes, "TAREAS PENDIENTES");
            listarTareas(realizadas, "TAREAS REALIZADAS");

        } else if (opcion == 4) {
            char entrada[256];
            printf("Buscar por ID o palabra clave: ");
            gets(entrada); // ⚠️ Uso de gets

            if (strspn(entrada, "0123456789") == strlen(entrada)) {
                int id = atoi(entrada);
                buscarTareaPorID(pendientes, realizadas, id);
            } else {
                buscarTareaPorPalabra(pendientes, realizadas, entrada);
            }
        }

    } while (opcion != 0);

    liberarLista(&pendientes);
    liberarLista(&realizadas);
    printf("Programa finalizado.\n");
    return 0;
}
