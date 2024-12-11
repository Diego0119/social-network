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

int main(int argc, char *argv[])
{
    PtrToHashTable table;
    Graph graph;
    GlobalInterests globalInterestsTable;
    User currentUser;
    // obtener parámetros ingresados por el terminal
    int option=get_option(argc, argv);
    
    // si no se llama a ayuda o hay error, se inician las estructuras de datos
    if(option > 0 && option != 2){
        table = create_hash_table();
        graph = initialize_graph();
        globalInterestsTable = init_global_interests();
        // cargar base de datos si es que existe
        if (database_exists_and_not_empty()) {
            load_all_users(table, graph, globalInterestsTable);
        }
        else {
            if(option==3){
                int quantity;
                if(sscanf(argv[2], "%d", &quantity)!=1){
                    printf("ERROR: No se pudo leer la cantidad de usuarios a generar\n");
                    free_graph(graph);
                    free_global_interests(globalInterestsTable);
                    exit(EXIT_FAILURE);
                }
                generate_users(quantity, table, graph, globalInterestsTable);
                generate_random_connections(graph, globalInterestsTable);
                save_all_users(graph, globalInterestsTable);
            }
            else{
                printf("No se ha encontrado una base de datos. Ejecute './devgraph -g <cantidad de usuarios>' para generar una.\n");
                free_graph(graph);
                free_global_interests(globalInterestsTable);
                exit(EXIT_FAILURE);
            }
        }
    }

    // verificación para comandos que requieren una sesión iniciada
    if(option==5||option==6||option==9||option==11){
        currentUser = current_session(table);
        if(!currentUser){
            printf("ERROR: No se ha iniciado sesión. Ejecute './devgraph -l' para iniciar sesión.\n");
            free_graph(graph);
            free_global_interests(globalInterestsTable);
            exit(EXIT_FAILURE);
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

    case 7: /* VER USUARIO */
        User user = search_user(argv[2], table);
        if(!user){
            printf("ERROR: Usuario no encontrado\n");
            return 0;
        }
        print_user(user, globalInterestsTable);
        break;
    case 8: /* VER TODOS LOS USUARIOS */
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
        clear_database();
        break;
    
    case 13: /* EDITAR INFORMACIÓN DEL USUARIO */
        /* POR HACER */
        break;
    
    case 14: /* MOSTRAR POSTS */
        /* POR HACER*/
        break;
    
    case 15: /* MOSTRAR USUARIOS RECOMENDADOS*/
        /* POR HACER*/
        break;

    default:
        return 0;
        break;
    }

    /*
    free_all_users(table, graph, globalInterestsTable);
    free_graph(graph);
    free_global_interests(globalInterestsTable);
    */

    return 0;
}
