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
 * gcc -o main src/main.c $(pkg-config --libs --cflags libmongoc-1.0) && ./main 
 * or
 * gcc -o mongodb_test src/main.c -I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -lmongoc-1.0 -lbson-1.0
 * 
 * TODO:
 * Makefile is broken, it doesn't make a working binary.
 * Add note about IP access list (Your external IP has to be listed in the MongoDB cloud, else you wont be able to connect.)
 * 
 * 
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
//#include "main.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include "../settings"

#define MAX_DBLIST_LENGTH 10

void create_new_doc(mongoc_collection_t *collection);
void print_count (mongoc_collection_t *collection, bson_t *filter);
void list_posts(mongoc_collection_t *collection, char **posts_array);

int main()
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;

   mongoc_init ();
   client = mongoc_client_new(CON_STRING);
   collection = mongoc_client_get_collection (client, COL_DB_NAME, COL_NAME);

   //create_new_doc(collection);

   char *posts_array;
   posts_array = NULL;
   int length = 100;
   posts_array = malloc(length * sizeof(char));

   list_posts(collection, &posts_array);

   for (int i = 0 ; i < length ; i++)
      printf("%d ", posts_array[i]);
   printf("\n");

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

void list_posts(mongoc_collection_t *collection, char **posts_array)
{
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_t *query;
   char *str;
   query = bson_new ();
   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

   int c=0;
   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_canonical_extended_json (doc, NULL);
      printf ("%s\n", str);
      posts_array[c] = str;
      bson_free (str);
      c++;
   }
   printf("Nr of posts: %d\n", c);
   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
}