void create_new_doc(mongoc_collection_t *collection, char *key, char *data)
{
   printf("Create new item %s %s", key, data);

   bson_error_t error;
   bson_oid_t oid;
   bson_t *doc;

   doc = bson_new ();
   bson_oid_init (&oid, NULL);
   BSON_APPEND_OID (doc, "_id", &oid);
   BSON_APPEND_UTF8 (doc, key, data);

   if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) 
   {
      fprintf (stderr, "%s\n", error.message);
   }
}
