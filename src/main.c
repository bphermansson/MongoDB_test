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
 * or
 * gcc -g -O0 -Wall -Wextra -Wshadow -pedantic -o main src/main.c $(pkg-config --libs --cflags libmongoc-1.0) && ./main 
 * 
 * TODO:
 * Makefile is broken, it doesn't make a working binary. Or?
 * Code Runner settings?
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
//#include <ping.h>
#include <create_new_doc.h>
#include "../settings"

#define MAX_DBLIST_LENGTH 10
#define BUFFSIZE 100

typedef struct 
{
   uint8_t key;
   uint8_t data;
} struct_data;

static struct_data data_struct[BUFFSIZE];

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

   printf("Enter data to add to db. Just press enter if you don't want to add anything.\n ");

   char *key;  
   key = (char *) malloc(BUFFSIZE);

   printf("Enter key: ");
   fgets(key, BUFFSIZE, stdin);
   key[strcspn(key, "\n")] = 0;    // Remove trailing newline

   if(strlen(key) > 0)     // A key is entered, go ahead with adding data.
   {
      printf("Enter data: ");
      char *data;
      data = (char *) malloc(BUFFSIZE);
      fgets(data, BUFFSIZE, stdin);
      data[strcspn(data, "\n")] = 0;
      //printf("%s\n", data);
      create_new_doc(collection, key, data);
      free(data);
   }
   else 
   {
      printf("List current posts\n");
      /*
      char *posts_array;
      posts_array = NULL;
      int length = 100;
      posts_array = malloc(length * sizeof(char));

      uint8_t no_of_posts = list_posts(collection, &posts_array);
      */
      data_struct[3].data = 1;
      struct_data dbdata[BUFFSIZE];
      struct_data *ptr_dbdata = dbdata;

      list_posts(collection, ptr_dbdata);

      printf("Done listing\n");

      //for (int i = 0 ; i < sizeof(posts_array) ; i++)
      for (int i = 0 ; i < 3 ; i++)
        // printf("%d - %d ", i, posts_array[i]);
      printf("---\n");
   }

   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0; 
}


int list_posts(mongoc_collection_t *collection, char **posts_array)
{
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_t *query;
   char *str;
   query = bson_new ();
   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);


   //char *key;  
   //key = (char *) malloc(BUFFSIZE);
int *b = (int*)malloc(sizeof(int)*4);

   b[2] = 6;
   b[0] = 6;

   int c=0;
   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_canonical_extended_json (doc, NULL);
      //printf ("%d - %s\n", c, str);
     // posts_array[c] = str;
  // memcpy(b, str, sizeof(int)*4);

      //key[c] = str;
     // strcpy(key[c], str);

      bson_free (str);
      c++;
   }
   printf("P: %c\n", b[2]);
   
   printf("Nr of posts: %d\n", c);
   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
   return c;
}