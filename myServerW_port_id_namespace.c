
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


//define a global variable to provide dummy data to temperature
UA_Double Temperature=	20.0;


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
  	////////////////////////////////////////////////////////////////////////////////////
	/* add a new namespace to the server */
	UA_Int16 ns_room1 = UA_Server_addNamespace(server,"Room1");
    	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "New Namespace wass added with Num. %d", ns_room1);


	/* add a new oobject 'temperature sensor' */ // see open62451 documentation: "Working with Objects and object types
	 UA_NodeId r1_tempsenss_Id; //get the nodeid assigned by the server 
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2,"R1_TemperatureSensor"),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(2, "Temperature Sensor"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL,&r1_tempsenss_Id); // 2 is the namespace id
	

    	/* Add the variable Vendor name to server */
    UA_VariableAttributes vnAttr = UA_VariableAttributes_default; // vnAttr(vendor name  attirbutes, vmAttr(machine name)
    UA_String vendorName = UA_STRING("Temp Sensor King Ltd.");
    UA_Variant_setScalar(&vnAttr.value, &vendorName, &UA_TYPES[UA_TYPES_STRING]);
   // mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
   // addVariableNode works just like addObjectsNode
   // UA_NODEID_NULL assigned the next avaiblae nodeID not random NodeID
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2,"R1_TS1_VendorName"), r1_tempsenss_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "VendorName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vnAttr, NULL, NULL);


	/* Add the variable Serial to the the server */

    UA_VariableAttributes snAttr = UA_VariableAttributes_default; // vnAttr(vendor name  attirbutes, vmAttr(machine name)
	UA_Int32 serialNumber = 12345678;
    //   UA_String vendorName = UA_STRING("Temp Sensor King Ltd.");
    UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
   // mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
   // addVariableNode works just like addObjectsNode
   // UA_NODEID_NULL assigned the next avaiblae nodeID not random NodeID
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2,"R1_TS1_SerialNumber"), r1_tempsenss_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "SerialNumber"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), snAttr, NULL, NULL);


    	/* Add the variable Temparature to server */
    UA_VariableAttributes tpAttr = UA_VariableAttributes_default; // vnAttr(vendor name  attirbutes, vmAttr(machine name)
    
    // UA_String vendorName = UA_STRING("Temp Sensor King Ltd.");
    UA_Variant_setScalar(&tpAttr.value, &Temperature, &UA_TYPES[UA_TYPES_DOUBLE]); // change the type
   // mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
   // addVariableNode works just like addObjectsNode
   // UA_NODEID_NULL assigned the next avaiblae nodeID not random NodeID
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2,"R1_TS1_VendorName"), r1_tempsenss_Id,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "Temperature"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), tpAttr, NULL, NULL);





    ///////////////////////////////////////////////////////////////////////////////////////


    UA_StatusCode retval = UA_Server_run(server, &running); // start the server

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Server was shut down");
    running = false;
    UA_Server_delete(server); // free server memory because we allocated mem. for the server
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}

