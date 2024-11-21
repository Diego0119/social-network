/**
 * @file users.h
 * @author Iván Mansilla, Franco Aguilar, Diego Sanhueza, Duvan Figueroa, Nicolás Álvarez, Miguel Maripillan
 * @brief Cabecera para users.c
 */

#ifndef USERS_H
#define USERS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hash.h>

/*usuarios individuales*/
typedef struct _user User;
typedef User *PtrToUser;

typedef struct _postNode PostNode;
typedef PostNode* PtrToPostNode;
typedef PtrToPostNode PostsPosition;
typedef PtrToPostNode UserPosts;

#define MAX_POST_TABLE 100

/**
 * @struct _user
 * @brief Estructura que almacena los datos de un usuario
 */
struct _user{
    int id; /*!< id (hash) único del usuario*/
    char *username; /*!< nombre del usuario*/
    char *password; /*!< contraseña del usuario*/
    char *name; /*!< nombre del usuario*/
    UserPosts posts; /*!< puntero a la lista de posts*/
};

/**
 *  @struct _postNode
 *  @brief Estructura que define un post (publicación)
 *  @note Es el nodo de una lista enlazada simple
 */
struct _postNode {
    int id; /*!< id (hash) del post*/
    struct tm date; /*!< fecha del post*/
    char* post; /*!< contenido del post*/
    PtrToPostNode next; /*!< Puntero al post siguiente*/
};

// Funciones para gestionar usuarios
User create_new_user(char* username, char* password, char* name);
void delete_user(User user);
void print_user(User user);

// Funciones para gestionar publicaciones (lista enlazada simple + hash)
UserPosts create_empty_userPosts();
PtrToPostNode insert_post(UserPosts posts, char* content);
PtrToPostNode search_post(UserPosts posts, int postId);
void delete_post(UserPosts posts, int postId);
void delete_userPosts(UserPosts posts);
void print_userPosts(UserPosts posts);

#endif