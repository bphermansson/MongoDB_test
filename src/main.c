/**
 * @file main.cpp
 * @author Patrik Hermansson (hermansson.patrik@gmail.com)
 * @brief 
 * Test connection to cloud MongoDB instance.
 * 
 * To compile on Fedora, install mongo-c-driver-devel;
 *  
 * sudo dnf install mongo-c-driver-devel
 * 
 * Then compile;
 * 
 * gcc -g -O0 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o main src/main.c $(pkg-config --libs --cflags libmongoc-1.0) && ./main 
 * 
 * 
 * 
 * gcc -Wall src/main.c lib/ping/ping.c -o main  -Ilib -Ilib/ping -I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -lmongoc-1.0 -lbson-1.0
 * 
 * TODO:
 * Makefile is broken, it doesn't make a working binary.
 * 
 * 
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include "main.h"
#include <ping.h>
#include "../settings"

#define MAX_DBLIST_LENGTH 10

//void create_new_doc(mongoc_collection_t *collection);
void print_count (mongoc_collection_t *collection, bson_t *filter);
//void list_posts(mongoc_collection_t *collection, char **posts_array);

int main()
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;

   mongoc_init ();
   client = mongoc_client_new(CON_STRING);
   collection = mongoc_client_get_collection (client, COL_DB_NAME, COL_NAME);
   mongoc_client_set_appname (client, "connect-example");

   //uint8_t pingres = ping();
   if (ping() != 1)
   {
      printf("Ping dataase failed, check your settings!\n");
      exit(1);
   }



   //database = mongoc_client_get_database (client, "test_database_1");

   //int p = ping();
   //create_new_doc(collection);

   char *posts_array;
   posts_array = NULL;
   int length = 100;
   posts_array = malloc(length * sizeof(char));

   uint8_t no_of_posts = list_all_posts(collection, &posts_array, length);

   printf("Done listing\n");
   printf("Found %d items.\n", no_of_posts);

   //for (int i = 0 ; i < sizeof(posts_array) ; i++)
   for (int i = 0 ; i < 3 ; i++)
      printf("%d - %d ", i, posts_array[i]);
   printf("---\n");

   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0; 
}

void create_new_doc(mongoc_collection_t *collection)
{
   bson_error_t error;
   bson_oid_t oid;
   bson_t *doc;

   doc = bson_new ();
   bson_oid_init (&oid, NULL);
   BSON_APPEND_OID (doc, "_id", &oid);
   BSON_APPEND_UTF8 (doc, "Hello", "world!");

   if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) 
   {
      fprintf (stderr, "%s\n", error.message);
   }
}