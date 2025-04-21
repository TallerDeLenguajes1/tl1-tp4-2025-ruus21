#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // para usar isdigit en la búsqueda por id

#define ID_INICIAL 1000

// variable global para ir generando los ids automáticamente
int id_actual = ID_INICIAL;

// estructura para representar una tarea
typedef struct tarea {
    int id;             // id único
    char *descripcion;  // puntero dinámico para la descripción
    int duracion;       // duración entre 10 y 100
} Tarea;

// estructura del nodo de la lista
typedef struct nodo {
    Tarea t;            // cada nodo tiene una tarea
    struct nodo *siguiente;  // y un puntero al siguiente nodo
} Nodo;

// puntero a Nodo (más cómodo para manejar listas)
typedef Nodo *Lista;

// declaraciones de las funciones que vamos a usar
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

    // menú principal
    do {
        printf("\nmenu:\n");
        printf("1. cargar tareas\n");
        printf("2. marcar tarea como realizada\n");
        printf("3. mostrar tareas\n");
        printf("4. buscar tarea\n");
        printf("0. salir\n");
        printf("opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpia el enter que queda en el buffer

        switch(opcion) {
            case 1:
                // cargar tareas en la lista de pendientes
                do {
                    printf("descripcion: ");
                    gets(desc);  // se podría usar fgets, pero uso gets por simplicidad
                    do {
                        printf("duracion (10-100): ");
                        scanf("%d", &duracion);
                        getchar(); // limpiar el enter
                    } while (duracion < 10 || duracion > 100);
                    n = crear_nodo(desc, duracion);
                    pendientes = agregar_tarea(pendientes, n);
                    printf("cargar otra? (s/n): ");
                    scanf("%c", &seguir);
                    getchar();
                } while (seguir == 's' || seguir == 'S');
                break;
            case 2:
                // pasar una tarea de pendientes a realizadas
                mostrar_lista(pendientes, "pendientes");
                printf("id de la tarea a mover: ");
                scanf("%d", &id);
                getchar();
                n = mover_tarea(&pendientes, id);
                if (n != NULL) {
                    realizadas = agregar_tarea(realizadas, n);
                    printf("tarea movida\n");
                } else {
                    printf("id no encontrado\n");
                }
                break;
            case 3:
                // mostrar ambas listas
                mostrar_lista(pendientes, "pendientes");
                mostrar_lista(realizadas, "realizadas");
                break;
            case 4:
                // búsqueda por palabra o id
                printf("buscar en pendientes o realizadas? (p/r): ");
                char lista_elegida;
                scanf("%c", &lista_elegida);
                getchar();

                printf("buscar por id o palabra: ");
                gets(entrada);
                if (isdigit(entrada[0])) {  // si arranca con número, interpreto como id
                    id = 0;
                    for (int i = 0; entrada[i] != '\0'; i++) {
                        id = id * 10 + (entrada[i] - '0');
                    }
                    if (lista_elegida == 'p' || lista_elegida == 'P') {
                        buscar_por_id(pendientes, id);
                    } else if (lista_elegida == 'r' || lista_elegida == 'R') {
                        buscar_por_id(realizadas, id);
                    }
                } else {
                    if (lista_elegida == 'p' || lista_elegida == 'P') {
                        buscar_por_palabra(pendientes, entrada);
                    } else if (lista_elegida == 'r' || lista_elegida == 'R') {
                        buscar_por_palabra(realizadas, entrada);
                    } else {
                        printf("opcion invalida\n");
                    }
                }
                break;
            case 0:
                printf("saliendo del programa...\n");
                break;
            default:
                printf("opcion invalida\n");
        }

    } while (opcion != 0);

    // liberar memoria antes de terminar
    pendientes = liberar_lista(pendientes);
    realizadas = liberar_lista(realizadas);
    return 0;
}

// crea una lista vacía
Lista crear_lista() {
    return NULL;
}

// genera un id autoincremental
int generar_id() {
    return id_actual++;
}

// crea un nodo nuevo con la descripción y duración pasadas
Lista crear_nodo(char *desc, int duracion) {
    Lista n = (Lista)malloc(sizeof(Nodo));
    n->t.id = generar_id();
    n->t.descripcion = (char *)malloc(strlen(desc) + 1);  // memoria dinámica para la descripción
    strcpy(n->t.descripcion, desc);
    n->t.duracion = duracion;
    n->siguiente = NULL;
    return n;
}

// agrega una tarea al principio de la lista (como pila)
Lista agregar_tarea(Lista lista, Lista nuevo) {
    nuevo->siguiente = lista;
    return nuevo;
}

// muestra los datos de una sola tarea
void mostrar_tarea(Tarea t) {
    printf("id: %d | duracion: %d | desc: %s\n", t.id, t.duracion, t.descripcion);
}

// muestra todas las tareas de la lista
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

// busca una tarea por id y la saca de la lista original
Lista mover_tarea(Lista *origen, int id) {
    Lista act = *origen, ant = NULL;
    while (act != NULL) {
        if (act->t.id == id) {
            if (ant == NULL) {
                *origen = act->siguiente;
            } else {
                ant->siguiente = act->siguiente;
            }
            act->siguiente = NULL;  // desconecto el nodo
            return act;
        }
        ant = act;
        act = act->siguiente;
    }
    return NULL;
}

// busca una tarea por id exacto
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

// busca tareas que contengan una palabra en la descripción
void buscar_por_palabra(Lista lista, char *clave) {
    int enc = 0;
    while (lista != NULL) {
        if (strstr(lista->t.descripcion, clave)) {  // strstr devuelve puntero si encuentra la clave
            if (!enc) printf("resultados:\n");
            mostrar_tarea(lista->t);
            enc = 1;
        }
        lista = lista->siguiente;
    }
    if (!enc) printf("no se encontro nada\n");
}

// libera toda la memoria usada por una lista
Lista liberar_lista(Lista lista) {
    Lista aux;
    while (lista != NULL) {
        aux = lista;
        lista = lista->siguiente;
        free(aux->t.descripcion);  // liberar la descripción dinámica
        free(aux);  // liberar el nodo
    }
    return NULL;
}
