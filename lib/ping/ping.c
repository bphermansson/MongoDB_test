#include <stdio.h>
#include <string.h>
#include <jansson.h>
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

    json_error_t jerror;
    json_t *root;
    size_t i=0;


    if (!retval) {
      fprintf (stderr, "%s\n", error.message);
      return EXIT_FAILURE;
    }

    str = bson_as_json (&reply, NULL);
    printf ("Ping: %s\n", str);

    root = json_loads(str, 0, &jerror);   

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", jerror.line, jerror.text);
        return 1;
    }

    if(!json_is_object(root))
    {
        fprintf(stderr, "Error: commit data %ld is not an object\n", i + 1);
        json_decref(root);
    }
/*
       json_object_foreach(root, key, value)
    {
        printf("Found key '%s'\n", key);
    }
*/
    json_t *val_ok;
    val_ok = json_object_get(root, "ok");
//    printf("val_key '%d'\n", val_ok);

//    printf("%d\n", json_integer_value(val_ok));

    u_int8_t res = json_integer_value(val_ok);
    if(res == 1)
    {
        printf("Ping ok (value: %d)\n", res);
    }
    else 
    {
        printf("Ping NOT ok (value: %d)\n", res);
    }

    return res;
}