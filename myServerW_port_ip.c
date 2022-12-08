
/*In this example we will include our own IP address and Port */
// run with gcc myServer.c -o myServer -lopen62541


#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>
#include <string.h>
static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(int argc, char **argv) {
    signal(SIGINT, stopHandler); // ctrl-c stops the server
    signal(SIGTERM, stopHandler);
//create a new server
UA_Server * server = UA_Server_new();


/*handling the input parameters for ip and port  */
	if (argc >2) {
		UA_Int16 port_number = atoi(argv[2]);
			UA_ServerConfig_setMinimal(UA_Server_getConfig(server), port_number,0);

	}
	else	
		UA_ServerConfig_setDefault(UA_Server_getConfig(server));

	if (argc >1) { // if you have more than one argument passed
	// hostname or ip address at input
	// /copy the host name from  char * to an open62541 variable
	UA_String hostname;
	UA_String_init(&hostname);
	hostname.length = strlen(argv[1]);
	hostname.data =(UA_Byte *)argv[1];
	//change the configuration
	UA_ServerConfig_setCustomHostname(UA_Server_getConfig(server), hostname);
    }



    UA_StatusCode retval = UA_Server_run(server, &running); // start the server

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Server was shut down");
    running = false;
    UA_Server_delete(server); // free server memory because we allocated mem. for the server
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}

