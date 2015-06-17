#include <public.h>
#include <Utils/mem_libs.h>
#include <GL Tools/interface.h>
#include "scene.h"
#include <Utils/utils.h>
#include <GL Tools/window.h>
#include <Network\network_client.h>

/*
interface_init.c

- Interface creation
*/

// Interface variables

char *address;
char *name;
int port;

char *message;

// End of interface variables

void send(void)
{
	string str;
	str.len = strlen(message);
	str.s = newString(message);
	CL_sendMessage(-1, str);
	mem_free(str.s);
	message[0] = '\0';
}

void connect(void)
{
	networkStatus_t status;
	bytestream clientInfo;
	bytestream_init(&clientInfo, strlen(name));
	bytestream_write(&clientInfo, name, clientInfo.len);

	CL_connectToServer(address, (unsigned short)port, clientInfo, &status);

	bytestream_destroy(&clientInfo);

	if (status.error == NETWORK_ERROR_NONE)
	{ // Success
		setMenu(1);
	}
	else
	{
		printError(status);
	}
}

void leave(void)
{
	CL_disconnectFromServer(NULL);
	setMenu(0);
}


void createInterface(int window)
{
	int temp;

	initInterface(window);

	address = (char*)mem_alloc(sizeof(char) * 16);
	strcpy(address, "127.0.0.1");
	address[9] = '\0';

	name = (char*)mem_alloc(sizeof(char) * 32);
	strcpy(name, "Client");
	name[6] = '\0';

	message = (char*)mem_alloc(sizeof(char) * 1024);
	message[0] = '\0';

	port = 5875;
	
	// Menu 0: Connection
	newMenu(0, 0, NULL, NULL);
	
	newLabel(newString("Connectez-vous a un serveur"), 0, 5, 18);

	newLabel(newString("Votre nom:"), 0, 5, 50);
	temp = newTextField(0, 100, 30, 150, 30, FIELDTYPE_TEXT);
	setTextFieldValue(0, temp, 0, 0, name, 1);

	newLabel(newString("Addresse du serveur:"), 0, 275, 50);
	temp = newTextField(0, 450, 30, 300, 30, FIELDTYPE_TEXT);
	setTextFieldValue(0, temp, 0, 0, address, 1);
	temp = newTextField(0, 775, 30, 100, 30, FIELDTYPE_INT);
	setTextFieldValue(0, temp, 0, 65535, &port, 1);

	newButton(newString("Connexion"), 0, 900, 20, 150, INTERFACE_HEIGHT - 40, connect);
	
	newButton(newString("Quitter"), 0, INTERFACE_WIDTH - 110, INTERFACE_HEIGHT - 30, 100, 20, closeProgram);

	// Menu 1: Chat room
	newMenu(INTERFACE_WIDTH, 0, NULL, NULL);

	temp = newTextField(1, 50, 30, INTERFACE_WIDTH - 150, 30, FIELDTYPE_TEXT);
	setTextFieldValue(1, temp, 0, 0, message, 1);

	newButton(newString("Envoyer"), 1, INTERFACE_WIDTH - 90, 30, 80, 30, send);

	newButton(newString("Se deconnecter"), 1, INTERFACE_WIDTH - 160, INTERFACE_HEIGHT - 30, 150, 20, leave);
}
