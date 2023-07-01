#include "sp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static	int	Read_message();
static	void	Usage(int argc, char* argv[]);

// ---------------------------------
// SPREAD
char usr[80];
char nombre_spread[80];
char nombre_grupo[MAX_GROUP_NAME];
mailbox buzon;

// ---------------------------------
// Menu
void imprimir_menu();
#define OPC_UNIRSE_GRUPO 'j'
#define OPC_SALIR_GRUPO  'l'
#define OPC_ENVIAR_MSG   's'
#define OPC_RECIBIR_MSG  'r'
#define OPC_SALIR		 'q'

bool validar_tipo_msg(char* tipo_msg);
#define UNREL_MSG_STR "rel"
#define REL_MSG_STR "unrel"
#define FIFO_MSG_STR "fifo"
#define CAUSAL_MSG_STR "causal"
#define AGREED_MSG_STR "atom"
#define SAFE_MSG_STR "safe"

// ---------------------------------
int main(int argc, char* argv[])
{
	char opc;
	char tipo_msg[8];

	imprimir_menu();
	do
	{
		printf("\nOpcion: ");
		scanf(" %c", &opc);
		switch (opc)
		{
			case OPC_UNIRSE_GRUPO:
				printf("Nombre del grupo: ");
				scanf("%s", nombre_grupo);
				// TO DO: unirse a grupo con spread
				break;
			case OPC_SALIR_GRUPO:
				printf("Nombre del grupo: ");
				scanf("%s", nombre_grupo);
				// TO DO: salir de grupo con spread
				break;
			case OPC_ENVIAR_MSG:
				printf("Tipo de mensaje (%s, %s, %s, %s, %s, %s): ", UNREL_MSG_STR, REL_MSG_STR, FIFO_MSG_STR, CAUSAL_MSG_STR, AGREED_MSG_STR, SAFE_MSG_STR);
				scanf("%s", tipo_msg);
				if (validar_tipo_msg(tipo_msg))
				{
					// TO DO: enviar mensaje con spread
				}
				else
					printf("Error: el tipo de mensaje especificado no es valido\n");
				break;
			case OPC_RECIBIR_MSG:
				// TO DO: recibir mensaje con spread
				break;
			case OPC_SALIR:
				printf("Saliendo...\n");
				break;
			default:
				//printf("%c\n", opc);
				printf("Error: la opcion especificada no es valida\n\n");
				break;
		}

	} while (opc != OPC_SALIR);

	return EXIT_SUCCESS;
}

void imprimir_menu()
{
	printf("------------------------------------------------\n");
	printf("|               VSOA SPREAD USR                |\n");
	printf("------------------------------------------------\n");
	printf("|                                              |\n");
	printf("|            %c - unirse a un grupo             |\n", OPC_UNIRSE_GRUPO);
	printf("|            %c - salir de un grupo             |\n", OPC_SALIR_GRUPO);
	printf("|            %c - enviar mensaje                |\n", OPC_ENVIAR_MSG);
	printf("|            %c - recibir mensaje               |\n", OPC_RECIBIR_MSG);
	printf("|                                              |\n");
	printf("|            %c - salir                         |\n", OPC_SALIR);
	printf("------------------------------------------------\n");
}

bool validar_tipo_msg(char* tipo_msg)
{
	if (strcmp(tipo_msg, UNREL_MSG_STR) == 0)
		return true;
	else if (strcmp(tipo_msg, REL_MSG_STR) == 0)
		return true;
	else if (strcmp(tipo_msg, FIFO_MSG_STR) == 0)
		return true;
	else if (strcmp(tipo_msg, CAUSAL_MSG_STR) == 0)
		return true;
	else if (strcmp(tipo_msg, AGREED_MSG_STR) == 0)
		return true;
	else if (strcmp(tipo_msg, SAFE_MSG_STR) == 0)
		return true;
	else
		return false;
}




