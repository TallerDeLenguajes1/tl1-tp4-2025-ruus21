#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Incluir ctype.h para isdigit

#define ID_INICIAL 1000

// variable global para los ids
int id_actual = ID_INICIAL;

// definiciones de tipos
typedef struct tarea {
    int id;
    char *descripcion;
    int duracion;
} Tarea;

typedef struct nodo {
    Tarea t;
    struct nodo *siguiente;
} Nodo;

typedef Nodo *Lista;

// funciones declaradas
Lista crear_lista();
int generar_id();
Lista crear_nodo(char *desc, int duracion);
Lista agregar_tarea(Lista lista, Lista nuevo);
void mostrar_tarea(Tarea t);
void mostrar_lista(Lista lista, char *nombre);
Lista mover_tarea(Lista *origen, int id);
void buscar_por_id(Lista lista, int id);
void buscar_por_palabra(Lista lista, char *clave);
Lista liberar_lista(Lista lista);

int main() {
    Lista pendientes = crear_lista();
    Lista realizadas = crear_lista();
    int opcion;
    int id;
    char desc[256], seguir, entrada[256];
    int duracion;
    Lista n;

    do {
        printf("\nmenu:\n");
        printf("1. cargar tareas\n");
        printf("2. marcar tarea como realizada\n");
        printf("3. mostrar tareas\n");
        printf("4. buscar tarea\n");
        printf("0. salir\n");
        printf("opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer de entrada

        switch(opcion) {
            case 1:
                do {
                    printf("descripcion: ");
                    gets(desc);
                    do {
                        printf("duracion (10-100): ");
                        scanf("%d", &duracion);
                        getchar(); // Limpiar el buffer de entrada
                    } while (duracion < 10 || duracion > 100);
                    n = crear_nodo(desc, duracion);
                    pendientes = agregar_tarea(pendientes, n);
                    printf("cargar otra? (s/n): ");
                    scanf("%c", &seguir);
                    getchar(); // Limpiar el buffer de entrada
                } while (seguir == 's' || seguir == 'S');
                break;
            case 2:
                mostrar_lista(pendientes, "pendientes");
                printf("id de la tarea a mover: ");
                scanf("%d", &id);
                getchar(); // Limpiar el buffer de entrada
                n = mover_tarea(&pendientes, id);
                if (n != NULL) {
                    realizadas = agregar_tarea(realizadas, n);
                    printf("tarea movida\n");
                } else {
                    printf("id no encontrado\n");
                }
                break;
            case 3:
                mostrar_lista(pendientes, "pendientes");
                mostrar_lista(realizadas, "realizadas");
                break;
            case 4:
                printf("buscar en pendientes o realizadas? (p/r): ");
                char lista_elegida;
                scanf("%c", &lista_elegida);
                getchar(); // Limpiar el buffer de entrada

                if (lista_elegida == 'p' || lista_elegida == 'P') {
                    printf("buscar por id o palabra: ");
                    gets(entrada);
                    if (isdigit(entrada[0])) {  // Usando isdigit de ctype.h
                        id = 0;
                        for (int i = 0; entrada[i] != '\0'; i++) {
                            id = id * 10 + (entrada[i] - '0');
                        }
                        buscar_por_id(pendientes, id);
                    } else {
                        buscar_por_palabra(pendientes, entrada);
                    }
                } else if (lista_elegida == 'r' || lista_elegida == 'R') {
                    printf("buscar por id o palabra: ");
                    gets(entrada);
                    if (isdigit(entrada[0])) {  // Usando isdigit de ctype.h
                        id = 0;
                        for (int i = 0; entrada[i] != '\0'; i++) {
                            id = id * 10 + (entrada[i] - '0');
                        }
                        buscar_por_id(realizadas, id);
                    } else {
                        buscar_por_palabra(realizadas, entrada);
                    }
                } else {
                    printf("opcion invalida\n");
                }
                break;
            case 0:
                printf("saliendo del programa...\n");
                break;
            default:
                printf("opcion invalida\n");
        }

    } while (opcion != 0);

    pendientes = liberar_lista(pendientes);
    realizadas = liberar_lista(realizadas);
    return 0;
}

Lista crear_lista() {
    return NULL;
}

int generar_id() {
    return id_actual++;
}

Lista crear_nodo(char *desc, int duracion) {
    Lista n = (Lista)malloc(sizeof(Nodo));
    n->t.id = generar_id();
    n->t.descripcion = (char *)malloc(strlen(desc) + 1);
    strcpy(n->t.descripcion, desc);
    n->t.duracion = duracion;
    n->siguiente = NULL;
    return n;
}

Lista agregar_tarea(Lista lista, Lista nuevo) {
    nuevo->siguiente = lista;
    return nuevo;
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
        lista = lista->siguiente;
    }
}

Lista mover_tarea(Lista *origen, int id) {
    Lista act = *origen, ant = NULL;
    while (act != NULL) {
        if (act->t.id == id) {
            if (ant == NULL) {
                *origen = act->siguiente;
            } else {
                ant->siguiente = act->siguiente;
            }
            act->siguiente = NULL;
            return act;
        }
        ant = act;
        act = act->siguiente;
    }
    return NULL;
}

void buscar_por_id(Lista lista, int id) {
    while (lista != NULL) {
        if (lista->t.id == id) {
            printf("tarea encontrada:\n");
            mostrar_tarea(lista->t);
            return;
        }
        lista = lista->siguiente;
    }
    printf("no se encontro la tarea\n");
}

void buscar_por_palabra(Lista lista, char *clave) {
    int enc = 0;
    while (lista != NULL) {
        if (strstr(lista->t.descripcion, clave)) {
            if (!enc) printf("resultados:\n");
            mostrar_tarea(lista->t);
            enc = 1;
        }
        lista = lista->siguiente;
    }
    if (!enc) printf("no se encontro nada\n");
}

Lista liberar_lista(Lista lista) {
    Lista aux;
    while (lista != NULL) {
        aux = lista;
        lista = lista->siguiente;
        free(aux->t.descripcion);
        free(aux);
    }
    return NULL;
}
