#include "sp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ---------------------------------
// SPREAD
#define ERROR_MESS 0
#define ERROR_MESS_STR "error"
#define MAX_MSG_LEN 1024
#define GRUPOS_MAX 16

int error = 0;
char usr[80];
char nombre_spread[80];
char nombre_grupo[MAX_GROUP_NAME];
mailbox buzon;

void unirse_a_grupo(char* nombre_grupo);
void salir_de_grupo(char* nombre_grupo);
void enviar_mensaje(char* msg, int16 tipo_servicio, char* nombre_grupo);
void recibir_mensaje();

// ---------------------------------
// Menu
#define OPC_UNIRSE_GRUPO 'j'
#define OPC_SALIR_GRUPO  'l'
#define OPC_ENVIAR_MSG   's'
#define OPC_RECIBIR_MSG  'r'
#define OPC_SALIR		 'q'

#define UNRELIABLE_MESS_STR "conf"
#define RELIABLE_MESS_STR "no conf"
#define FIFO_MESS_STR "fifo"
#define CAUSAL_MESS_STR "causal"
#define AGREED_MESS_STR "atom"
#define SAFE_MESS_STR "safe"

void imprimir_menu();
void gestionar_opciones();
int16 mapear_tipo_servicio_str(char* tipo_servicio_str);

// ---------------------------------
int main(int argc, char* argv[])
{
	error = SP_connect(nombre_spread, usr, 0, 1, &buzon, nombre_grupo);
	if (error < 0)
	{
		printf("Error. No se pudo conectar a SPREAD. Saliendo...\n");
		SP_error(error);
		exit(EXIT_FAILURE);
	}
	else
	{
		imprimir_menu();
		gestionar_opciones();
	}
	
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
	printf("|                                              |\n");
	printf("------------------------------------------------\n");
}

void gestionar_opciones()
{
	char msg[MAX_MSG_LEN];
	int16 tipo_servicio;
	
	char opc;
	char tipo_servicio_str[8];

	do
	{
		printf("\nOpcion: ");
		scanf(" %c", &opc);
		switch (opc)
		{
		case OPC_UNIRSE_GRUPO:
			printf("Grupo: ");
			scanf("%s", nombre_grupo);
			
			unirse_a_grupo(nombre_grupo);
			break;
		case OPC_SALIR_GRUPO:
			printf("Grupo: ");
			scanf("%s", nombre_grupo);
			
			salir_de_grupo(nombre_grupo);
			break;
		case OPC_ENVIAR_MSG:
			printf("Mensaje: ");
			scanf("%s", msg);
			
			printf("Tipo de servicio (%s, %s, %s, %s, %s, %s): ", UNRELIABLE_MESS_STR, RELIABLE_MESS_STR, FIFO_MESS_STR, CAUSAL_MESS_STR, AGREED_MESS_STR, SAFE_MESS_STR);
			do
			{
				tipo_servicio = mapear_tipo_servicio_str(tipo_servicio_str);
				scanf("%s", tipo_servicio_str);
				printf("Error: el tipo de servicio especificado no es valido\n");

			} while (tipo_servicio == ERROR_MESS);

			printf("Grupo: ");
			scanf("%s", nombre_grupo);
			
			enviar_mensaje(msg, tipo_servicio, nombre_grupo);
			break;
		case OPC_RECIBIR_MSG:
			recibir_mensaje();
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
}

int16 mapear_tipo_servicio_str(char* tipo_servicio_str)
{
	if (strcmp(tipo_servicio_str, UNRELIABLE_MESS_STR) == 0)
		return UNRELIABLE_MESS;
	else if (strcmp(tipo_servicio_str, RELIABLE_MESS_STR) == 0)
		return RELIABLE_MESS;
	else if (strcmp(tipo_servicio_str, FIFO_MESS_STR) == 0)
		return FIFO_MESS;
	else if (strcmp(tipo_servicio_str, CAUSAL_MESS_STR) == 0)
		return CAUSAL_MESS;
	else if (strcmp(tipo_servicio_str, AGREED_MESS_STR) == 0)
		return AGREED_MESS;
	else if (strcmp(tipo_servicio_str, SAFE_MESS_STR) == 0)
		return SAFE_MESS;
	else
		return ERROR_MESS;
}

char* mapear_tipo_servicio(int16 tipo_servicio)
{
	if (tipo_servicio == UNRELIABLE_MESS)
		return UNRELIABLE_MESS_STR;
	else if (tipo_servicio == RELIABLE_MESS)
		return RELIABLE_MESS_STR;
	else if (tipo_servicio == FIFO_MESS)
		return FIFO_MESS_STR;
	else if (tipo_servicio == CAUSAL_MESS)
		return CAUSAL_MESS_STR;
	else if (tipo_servicio == AGREED_MESS)
		return AGREED_MESS_STR;
	else if (tipo_servicio == SAFE_MESS)
		return SAFE_MESS_STR;
	else
		return ERROR_MESS_STR;
}

void unirse_a_grupo(char* nombre_grupo)
{
	error = SP_join(buzon, nombre_grupo);
	if (error < 0)
	{
		printf("Error: no se pudo unir al grupo\n");
		SP_error(error);
	}
	else
		printf("Unido al grupo exitosamente\n");
}

void salir_de_grupo(char* nombre_grupo)
{
	error = SP_leave(buzon, nombre_grupo);
	if (error < 0)
	{
		printf("Error: no se pudo salir del grupo\n");
		SP_error(error);
	}
	else
		printf("Se salio del grupo exitosamente\n");
}

void enviar_mensaje(char* msg, int16 tipo_servicio, char* nombre_grupo)
{
	error = SP_multicast(buzon, tipo_servicio, nombre_grupo, 1, strlen(msg), msg);
	if (error < 0)
	{
		printf("Error: no pudo enviarse el mensaje\n");
		SP_error(error);
	}
	else
		printf("Mensaje enviado exitosamente\n");
}

void recibir_mensaje()
{
	char msg[MAX_MSG_LEN];
	char remitente[MAX_GROUP_NAME];
	char grupos_receptores[GRUPOS_MAX][MAX_GROUP_NAME];
	int	cant_grupos_receptores;
	membership_info info_membresia;
	int	tipo_servicio = 0;
	int16 tipo_mensaje;
	int	endian_mismatch;
	
	error = SP_receive(buzon, &tipo_servicio, remitente, GRUPOS_MAX, &cant_grupos_receptores, grupos_receptores, &tipo_mensaje, &endian_mismatch, sizeof(msg), msg);
	if (error < 0)
	{
		printf("Error: no se pudo recibir el mensaje\n");
		SP_error(error);
	}
	else
	{
		// TO DO: mostrar info mensaje
	}
}









