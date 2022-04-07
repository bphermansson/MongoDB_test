
int ping()
{
    mongoc_database_t *database;
    bson_t *command, reply, *insert;
    bool retval;
    bson_error_t error;
    char *str;
    command = BCON_NEW ("ping", BCON_INT32 (1));

    retval = mongoc_client_command_simple (
      client, "admin", command, NULL, &reply, &error);

    if (!retval) {
      fprintf (stderr, "%s\n", error.message);
      return EXIT_FAILURE;
    }

    str = bson_as_json (&reply, NULL);
    printf ("%s\n", str);
}