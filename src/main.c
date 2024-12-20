/**
 * @file main.c
 * @brief Flujo principal del programa
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"
#include "heaps.h"
#include "graph.h"
#include "hash_table.h"
#include "utilities.h"
#include "database.h" 

/**
 * @brief Función que ejecuta el flujo principal del programa
 * 
 * @param argc Número de argumentos
 * @param argv Argumentos ingresados al programa
 * @return int 
 */
int main(int argc, char *argv[]){
    srand(time(0)^clock());

    PtrToHashTable table = NULL;
    Graph graph = NULL;
    GlobalInterests globalInterestsTable;
    User currentUser = NULL;
    heap feed;

    // obtener parámetros ingresados por el terminal
    int option=get_option(argc, argv);
    
    // si no se llama a ayuda o hay error, se inician las estructuras de datos
    if(option > 0 && option != 2){
        table = create_hash_table();
        graph = initialize_graph();
        globalInterestsTable = init_global_interests();
        // cargar base de datos si es que existe
        if (database_exists_and_not_empty() && option != 3) {
            load_database(table, graph, globalInterestsTable);
        }
        else {
            if(option==3){
                int quantity;
                if(sscanf(argv[2], "%d", &quantity)!=1 || quantity<1){
                    printf("ERROR: cantidad de usuarios inválida\n");
                    free_structures_and_exit(table, graph, globalInterestsTable);
                }
                if(quantity>50000){
                    printf(COLOR_RED "ERROR: La cantidad de usuarios generados es demasiado alta y puede tener problemas de rendimiento. Intente generar menos de 50.000 usuarios.\n");
                    free_structures_and_exit(table, graph, globalInterestsTable);
                }
                generate_database(quantity, table, graph, globalInterestsTable);
                free_all_users(table, graph);
                printf(COLOR_BLUE "Se han generado %d usuarios exitosamente\n", quantity);
            }
            else{
                printf("No se ha encontrado una base de datos. Ejecute './devgraph -g <cantidad de usuarios>' para generar una.\n");
                free_structures_and_exit(table, graph, globalInterestsTable);
            }
        }
    }

    // verificación para comandos que requieren una sesión iniciada
    if(option==5||option==6||option==9||option==10||option==11||option==13||option==14||option==15||option==17||option==18){
        currentUser = current_session(table);
        if(!currentUser){
            printf("ERROR: No se ha iniciado sesión. Ejecute './devgraph -l' para iniciar sesión.\n");
            free_structures_and_exit(table, graph, globalInterestsTable);
        }
    }

    /* COMANDOS DEL PROGRAMA */
    switch (option){

    case 1: /* INICIO DE SESIÓN */
        login(table);
        break;

    case 2: /* CIERRA SESIÓN */
        logout();
        break;
    
    case 4: /* REGISTRAR USUARIO */
        register_user(table, graph, globalInterestsTable);
        break;

    case 5: /* PUBLICAR PUBLICACIÓN */
        write_post(currentUser, globalInterestsTable);
        break;

    case 6: /* VER PERFIL DEL USUARIO */
        print_user(currentUser, globalInterestsTable);
        break;

    case 7: {/* VER USUARIO */
        User user = search_user(argv[2], table);
        if(!user){
            printf("ERROR: Usuario no encontrado\n");
            return 0;
        }
        print_logo();
        print_user(user, globalInterestsTable);
        break;
    }
    case 8: /* VER TODOS LOS USUARIOS */
        print_logo();
        print_all_users(graph);
        break;
    
    case 9: /* SEGUIR A UN USUARIO */
        follow(currentUser, argv[2], globalInterestsTable, table);
        break;
    
    case 10: /* DEJAR DE SEGUIR A UN USUARIO */
        unfollow(currentUser, argv[2], globalInterestsTable, table);
        break;
    
    case 11: /* BORRAR CUENTA */
        delete_account(currentUser);
        break;

    case 12: /* BORRAR BASE DE DATOS */
        printf("Preparando, por favor espere...\n");
        remove("current.dat");
        clear_database(graph);
        break;
    
    case 13: /* EDITAR INFORMACIÓN DEL USUARIO */
        print_logo();
        edit_account(currentUser, globalInterestsTable, table);
        break;
    
    case 14: /* MOSTRAR POSTS */
        search_posts_in_my_follows(&feed, currentUser);
        search_posts_by_interests(&feed, table, globalInterestsTable, currentUser);
        watch_posts(&feed);
        free_heap(&feed);
        break;
    
    case 15: /* MOSTRAR USUARIOS RECOMENDADOS*/
        print_logo();
        dijkstra(&feed, graph, currentUser);
        watch_suggestions_friends_of_friends(&feed);
        search_new_possible_friends(&feed, table, globalInterestsTable, currentUser);
        watch_suggestions_by_interests(&feed);
        free_heap(&feed);
        break;
    
    case 16: /* MOSTRAR TEMAS */
        printf("Tópicos de DevGraph:\n");
        print_global_interests(globalInterestsTable);
        break;
    
    case 17: /* MOSTRAR SEGUIDORES DEL USUARIO */
        print_followers(currentUser);
        break;
    
    case 18: /* MOSTRAR SEGUIDOS DEL USUARIO */
        print_following(currentUser);
        break;

    default:
        return 0;
        break;
    }

    // LIBERAR MEMORIA
    if(option > 0 && option != 2){
        free_all_users(table, graph);
        free_graph(graph);
        free_global_interests(globalInterestsTable);
        free_hash_table(table);
    }

    return 0;
}
