#include <mongoc/mongoc.h>
#include <bson/bson.h>

int list_all_posts(mongoc_collection_t *collection, char **posts_array, int length)
{
   printf("In list_posts\n");

   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_t *query;
   char *str;
   query = bson_new ();
   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

   #define NUM_STRINGS 10

   free(*posts_array);
   posts_array = malloc(length * sizeof(char));
   if (*posts_array == NULL)
   {
      return 1;
   }

   int c=0;
   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_canonical_extended_json (doc, NULL);
      printf ("%d - %s\n", c, str);
      bson_free (str);
      c++;
   }
   return c;
}