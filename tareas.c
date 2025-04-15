#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ID_INICIAL 1000

typedef struct tarea {
    int TareaID;
    char *Descripcion;
    int Duracion;
} Tarea;

typedef struct nodo {
    Tarea T;
    struct nodo *Siguiente;
} Nodo;

typedef Nodo *Lista;

Lista crearListaVacia();
int generarID();
Lista crearNodo(char *desc, int duracion);
void agregarTarea(Lista *lista, Lista nodo);
void mostrarTarea(Tarea t);
void listarTareas(Lista lista, const char *titulo);
Lista buscarYRemoverTarea(Lista *lista, int id);
void buscarTareaPorID(Lista pendientes, Lista realizadas, int id);
void buscarTareaPorPalabra(Lista pendientes, Lista realizadas, char *palabra);
void liberarLista(Lista *lista);

int idGlobal = ID_INICIAL;

int main() {
    Lista pendientes = crearListaVacia();
    Lista realizadas = crearListaVacia();
    int opcion;

    do {
        printf("\n=== MENU ===\n");
        printf("1. Cargar tareas pendientes\n");
        printf("2. Marcar tareas como realizadas\n");
        printf("3. Listar tareas\n");
        printf("4. Buscar tarea por ID o palabra clave\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1) {
            char descripcion[256];
            int duracion;
            char seguir;
            do {
                printf("\nDescripcion: ");
                gets(descripcion);
                do {
                    printf("Duracion (10-100): ");
                    scanf("%d", &duracion);
                    getchar();
                } while (duracion < 10 || duracion > 100);

                Lista nuevo = crearNodo(descripcion, duracion);
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
            Lista movida = buscarYRemoverTarea(&pendientes, id);
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
            gets(entrada);
            if (strspn(entrada, "0123456789") == strlen(entrada)) {
                buscarTareaPorID(pendientes, realizadas, atoi(entrada));
            } else {
                buscarTareaPorPalabra(pendientes, realizadas, entrada);
            }

        } else if (opcion == 0) {
            printf("Saliendo...\n");

        } else {
            printf("Opcion invalida.\n");
        }

    } while (opcion != 0);

    liberarLista(&pendientes);
    liberarLista(&realizadas);
    return 0;
}

Lista crearListaVacia() {
    return NULL;
}

int generarID() {
    return idGlobal++;
}

Lista crearNodo(char *desc, int duracion) {
    Lista nuevo = (Lista)malloc(sizeof(Nodo));
    nuevo->T.TareaID = generarID();
    nuevo->T.Descripcion = strdup(desc);
    nuevo->T.Duracion = duracion;
    nuevo->Siguiente = NULL;
    return nuevo;
}

void agregarTarea(Lista *lista, Lista nodo) {
    nodo->Siguiente = *lista;
    *lista = nodo;
}

void mostrarTarea(Tarea t) {
    printf("ID: %d | Duracion: %d | Descripcion: %s\n", t.TareaID, t.Duracion, t.Descripcion);
}

void listarTareas(Lista lista, const char *titulo) {
    printf("\n--- %s ---\n", titulo);
    if (!lista) {
        printf("No hay tareas.\n");
        return;
    }
    Lista actual = lista;
    while (actual) {
        mostrarTarea(actual->T);
        actual = actual->Siguiente;
    }
}

Lista buscarYRemoverTarea(Lista *lista, int id) {
    Lista actual = *lista, *anterior = NULL;
    while (actual) {
        if (actual->T.TareaID == id) {
            if (!anterior)
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
    Lista aux = pendientes;
    while (aux) {
        if (aux->T.TareaID == id) {
            printf("Tarea en PENDIENTES:\n");
            mostrarTarea(aux->T);
            return;
        }
        aux = aux->Siguiente;
    }
    aux = realizadas;
    while (aux) {
        if (aux->T.TareaID == id) {
            printf("Tarea en REALIZADAS:\n");
            mostrarTarea(aux->T);
            return;
        }
        aux = aux->Siguiente;
    }
    printf("No se encontro ninguna tarea con ID %d.\n", id);
}

void buscarTareaPorPalabra(Lista pendientes, Lista realizadas, char *palabra) {
    int encontrada = 0;
    Lista aux = pendientes;
    while (aux) {
        if (strstr(aux->T.Descripcion, palabra)) {
            if (!encontrada) printf("Coincidencias con '%s':\n", palabra);
            printf("Pendiente: ");
            mostrarTarea(aux->T);
            encontrada = 1;
        }
        aux = aux->Siguiente;
    }
    aux = realizadas;
    while (aux) {
        if (strstr(aux->T.Descripcion, palabra)) {
            if (!encontrada) printf("Coincidencias con '%s':\n", palabra);
            printf("Realizada: ");
            mostrarTarea(aux->T);
            encontrada = 1;
        }
        aux = aux->Siguiente;
    }
    if (!encontrada)
        printf("No se encontro ninguna tarea con esa palabra.\n");
}

void liberarLista(Lista *lista) {
    Lista aux;
    while (*lista) {
        aux = *lista;
        *lista = (*lista)->Siguiente;
        free(aux->T.Descripcion);
        free(aux);
    }
}