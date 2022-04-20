
typedef struct 
{
    uint8_t state;
    uint8_t pin_number;
    uint8_t nr_of_con_reads;
    bool initialized;
    uint8_t level;
    uint8_t prev_level;
    uint8_t edge;
} btn_struct;
static btn_struct debouncer_btns[MAX_NO_OF_BTNS];


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