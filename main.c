/**
 * @file main.cpp
 * @author Patrik Hermansson (hermansson.patrik@gmail.com)
 * @brief 
 * Test connection to cloud MongoDB instance.
 * 
 * $ gcc -o main main.c $(pkg-config --libs --cflags libmongoc-1.0) && ./main 
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

void print_count (mongoc_collection_t *collection, bson_t *filter);


int main()
{
   mongoc_database_t *database;
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   int64_t count;
   bson_error_t error;
   bson_t *filter;
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_t *query;
   char *str;
   
   mongoc_init ();

   client = mongoc_client_new(
      "mongodb+srv://mongo_user_2:BagIGj34yrYplohv@cluster0.y7oro.mongodb.net/myFirstDatabase?retryWrites=true&w=majority"
   );
   database = mongoc_client_get_database (client, "test");
   collection = mongoc_client_get_collection (client, "NEW_DATABASE_NAME", "NEW_COLLECTION_NAME");

   query = bson_new ();
   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_canonical_extended_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);


   // Call this to create a new post in the db
   //create_new_doc(collection);

   mongoc_database_destroy (database);
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
