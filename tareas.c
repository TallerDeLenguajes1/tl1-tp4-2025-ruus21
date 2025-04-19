#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_INICIAL 1000

// definiciones de tipos
typedef struct tarea {
    int id;
    char *descripcion;
    int duracion;
} Tarea;

typedef struct nodo {
    Tarea t;
    struct nodo *sig;
} Nodo;

typedef Nodo *Lista;

// funciones declaradas
Lista crear_lista();
int generar_id();
Nodo *crear_nodo(char *desc, int duracion);
void agregar_tarea(Lista *lista, Nodo *nuevo);
void mostrar_tarea(Tarea t);
void mostrar_lista(Lista lista, char *nombre);
Nodo *mover_tarea(Lista *origen, int id);
void buscar_por_id(Lista pend, Lista real, int id);
void buscar_por_palabra(Lista pend, Lista real, char *clave);
void liberar_lista(Lista *lista);

int main() {
    Lista pendientes = crear_lista();
    Lista realizadas = crear_lista();
    int opcion;
    int id;
    char desc[256], seguir, entrada[256];
    int duracion;
    Nodo *n;

    do {
        printf("\nmenu:\n");
        printf("1. cargar tareas\n");
        printf("2. marcar tarea como realizada\n");
        printf("3. mostrar tareas\n");
        printf("4. buscar tarea\n");
        printf("0. salir\n");
        printf("opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpia el buffer

        if (opcion == 1) {
            do {
                printf("descripcion: ");
                gets(desc); // como pediste
                do {
                    printf("duracion (10-100): ");
                    scanf("%d", &duracion);
                    getchar();
                } while (duracion < 10 || duracion > 100);
                n = crear_nodo(desc, duracion);
                agregar_tarea(&pendientes, n);
                printf("cargar otra? (s/n): ");
                scanf("%c", &seguir);
                getchar();
            } while (seguir == 's' || seguir == 'S');
        } else if (opcion == 2) {
            mostrar_lista(pendientes, "pendientes");
            printf("id de la tarea a mover: ");
            scanf("%d", &id);
            getchar();
            n = mover_tarea(&pendientes, id);
            if (n != NULL) {
                agregar_tarea(&realizadas, n);
                printf("tarea movida\n");
            } else {
                printf("id no encontrado\n");
            }
        } else if (opcion == 3) {
            mostrar_lista(pendientes, "pendientes");
            mostrar_lista(realizadas, "realizadas");
        } else if (opcion == 4) {
            printf("buscar por id o palabra: ");
            gets(entrada);
            if (strspn(entrada, "0123456789") == strlen(entrada)) {
                buscar_por_id(pendientes, realizadas, atoi(entrada));
            } else {
                buscar_por_palabra(pendientes, realizadas, entrada);
            }
        }

    } while (opcion != 0);

    liberar_lista(&pendientes);
    liberar_lista(&realizadas);
    return 0;
}

// funciones definidas abajo

Lista crear_lista() {
    return NULL;
}

int generar_id() {
    static int id = ID_INICIAL;
    return id++;
}

Nodo *crear_nodo(char *desc, int duracion) {
    Nodo *n = (Nodo *)malloc(sizeof(Nodo));
    n->t.id = generar_id();
    n->t.descripcion = strdup(desc);
    n->t.duracion = duracion;
    n->sig = NULL;
    return n;
}

void agregar_tarea(Lista *lista, Nodo *nuevo) {
    nuevo->sig = *lista;
    *lista = nuevo;
}

void mostrar_tarea(Tarea t) {
    printf("id: %d | duracion: %d | desc: %s\n", t.id, t.duracion, t.descripcion);
}

void mostrar_lista(Lista lista, char *nombre) {
    printf("\n--- %s ---\n", nombre);
    if (lista == NULL) {
        printf("no hay tareas\n");
        return;
    }
    while (lista != NULL) {
        mostrar_tarea(lista->t);
        lista = lista->sig;
    }
}

Nodo *mover_tarea(Lista *origen, int id) {
    Nodo *act = *origen, *ant = NULL;
    while (act != NULL) {
        if (act->t.id == id) {
            if (ant == NULL)
                *origen = act->sig;
            else
                ant->sig = act->sig;
            act->sig = NULL;
            return act;
        }
        ant = act;
        act = act->sig;
    }
    return NULL;
}

void buscar_por_id(Lista pend, Lista real, int id) {
    while (pend != NULL) {
        if (pend->t.id == id) {
            printf("en pendientes:\n");
            mostrar_tarea(pend->t);
            return;
        }
        pend = pend->sig;
    }
    while (real != NULL) {
        if (real->t.id == id) {
            printf("en realizadas:\n");
            mostrar_tarea(real->t);
            return;
        }
        real = real->sig;
    }
    printf("no se encontro la tarea\n");
}

void buscar_por_palabra(Lista pend, Lista real, char *clave) {
    int enc = 0;
    while (pend != NULL) {
        if (strstr(pend->t.descripcion, clave)) {
            if (!enc) printf("resultados:\n");
            printf("pendiente: ");
            mostrar_tarea(pend->t);
            enc = 1;
        }
        pend = pend->sig;
    }
    while (real != NULL) {
        if (strstr(real->t.descripcion, clave)) {
            if (!enc) printf("resultados:\n");
            printf("realizada: ");
            mostrar_tarea(real->t);
            enc = 1;
        }
        real = real->sig;
    }
    if (!enc) printf("no se encontro nada\n");
}

void liberar_lista(Lista *lista) {
    Nodo *aux;
    while (*lista != NULL) {
        aux = *lista;
        *lista = (*lista)->sig;
        free(aux->t.descripcion);
        free(aux);
    }
}
