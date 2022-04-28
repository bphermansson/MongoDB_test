void create_new_doc(mongoc_collection_t *collection);
int list_posts(mongoc_collection_t *collection, char **posts_array, int length);
void print_count (mongoc_collection_t *collection, bson_t *filter);
