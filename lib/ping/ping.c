#include <stdio.h>
#include <ping.h>
#include "../../settings"

int ping()
{
    mongoc_client_t *client;

    mongoc_init ();
    client = mongoc_client_new(CON_STRING);

    bson_t *command, reply;
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

    // TODO: Decode JSON and return digit
    //return str;
}