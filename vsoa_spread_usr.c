#include "sp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ---------------------------------------------------------------------------------------------------------
// SPREAD
#define ERROR_MESS		0
#define ERROR_MESS_STR	"error"
#define MAX_MSG_LEN		1024
#define GRUPOS_MAX		16

int cont_msg = 0;
int error = 0;

mailbox buzon;
char nombre_privado[MAX_PRIVATE_NAME] = "vsoa-usr-";
char* nombre_spread = NULL;
char grupo_privado[MAX_GROUP_NAME];
int recibir_msgs_membresia = 1;

void unirse_a_grupo(char* nombre_grupo);
void salir_de_grupo(char* nombre_grupo);
void enviar_mensaje(char* msg, int16 tipo_servicio, char* nombre_grupo);
void recibir_mensaje();
void recibir_n_mensajes();

// ---------------------------------------------------------------------------------------------------------
// Menu
#define OPC_UNIRSE_GRUPO 'j'
#define OPC_SALIR_GRUPO  'l'
#define OPC_ENVIAR_MSG   's'
#define OPC_RECIBIR_MSG  'r'
#define OPC_SALIR		 'q'

#define UNRELIABLE_MESS_STR "unrel"
#define RELIABLE_MESS_STR	"rel"
#define FIFO_MESS_STR		"fifo"
#define CAUSAL_MESS_STR		"causal"
#define AGREED_MESS_STR		"atom"
#define SAFE_MESS_STR		"safe"

void imprimir_menu();
void gestionar_opciones();
int16 mapear_tipo_servicio_str(char* tipo_servicio_str);
char* mapear_tipo_servicio(int16 tipo_servicio);
void limpiar_ch();
void str_replace(char* str, char old_ch, char new_ch);

// ---------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Llamar a programa con numero de container
	strcat(nombre_privado, argv[1]);
	error = SP_connect(nombre_spread, nombre_privado, 0, recibir_msgs_membresia, &buzon, grupo_privado);
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

// ---------------------------------------------------------------------------------------------------------
void imprimir_menu()
{
	printf("------------------------------------------------------------------------\n");
	printf("|                            VSOA SPREAD USR                           |\n");
	printf("------------------------------------------------------------------------\n\n");
	printf("                          %c - unirse a un grupo\n", OPC_UNIRSE_GRUPO);
	printf("                          %c - salir de un grupo\n", OPC_SALIR_GRUPO);
	printf("                          %c - enviar mensaje\n", OPC_ENVIAR_MSG);
	printf("                          %c - recibir mensaje\n\n", OPC_RECIBIR_MSG);
	printf("                          %c - salir\n\n", OPC_SALIR);
}

void gestionar_opciones()
{
	char msg[MAX_MSG_LEN] = "";
	int16 tipo_servicio = ERROR_MESS;
	char nombre_grupo[MAX_GROUP_NAME] = "";
	
	char opc = OPC_SALIR;
	char tipo_servicio_str[8] = ERROR_MESS_STR;

	do
	{
		printf("------------------------------------------------------------------------\n");
		printf("Opcion: ");
		scanf(" %c", &opc);
		switch (opc)
		{
		case OPC_UNIRSE_GRUPO:
			printf("Grupo: ");
			scanf(" %s", nombre_grupo); // Los grupos no pueden tener espacios

			unirse_a_grupo(nombre_grupo);
			break;
		case OPC_SALIR_GRUPO:
			printf("Grupo: ");
			scanf(" %s", nombre_grupo);

			salir_de_grupo(nombre_grupo);
			break;
		case OPC_ENVIAR_MSG:
			printf("Mensaje: ");
			getchar(); // Ignorar '\n' de la opcion
			fgets(msg, MAX_MSG_LEN, stdin);
			str_replace(msg, '\n', '\0'); // Sacar el salto de linea

			do
			{
				printf("Tipo de servicio (%s, %s, %s, %s, %s, %s): ", UNRELIABLE_MESS_STR, RELIABLE_MESS_STR, FIFO_MESS_STR, CAUSAL_MESS_STR, AGREED_MESS_STR, SAFE_MESS_STR);
				scanf(" %s", tipo_servicio_str);
				tipo_servicio = mapear_tipo_servicio_str(tipo_servicio_str);
				if (tipo_servicio == ERROR_MESS)
					printf("Error: el tipo de servicio especificado no es valido\n");
			} 
			while (tipo_servicio == ERROR_MESS);

			limpiar_ch();
			printf("Grupo: ");
			scanf(" %s", nombre_grupo);
			
			enviar_mensaje(msg, tipo_servicio, nombre_grupo);
			break;
		case OPC_RECIBIR_MSG:
			recibir_n_mensajes();
			break;
		case OPC_SALIR:
			SP_disconnect(buzon);
			printf("Saliendo...\n\n");
			break;
		default:
			printf("Error: la opcion especificada no es valida\n\n");
			break;
		}

		limpiar_ch();
	} 
	while (opc != OPC_SALIR);
}

void limpiar_ch()
{
	char descartar;
	do { descartar = getchar(); } 
	while (descartar != '\n' && descartar != EOF);
}

void str_replace(char* str, char old_ch, char new_ch)
{
	for (int i = 0; i < strlen(str); i++)
		if (str[i] == old_ch)
			str[i] = new_ch;
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
	if (Is_unreliable_mess(tipo_servicio))
		return UNRELIABLE_MESS_STR;
	else if (Is_reliable_mess(tipo_servicio))
		return RELIABLE_MESS_STR;
	else if (Is_fifo_mess(tipo_servicio))
		return FIFO_MESS_STR;
	else if (Is_causal_mess(tipo_servicio))
		return CAUSAL_MESS_STR;
	else if (Is_agreed_mess(tipo_servicio))
		return AGREED_MESS_STR;
	else if (Is_safe_mess(tipo_servicio))
		return SAFE_MESS_STR;
	else
		return ERROR_MESS_STR;
}

// ---------------------------------------------------------------------------------------------------------

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
	// Nota: el significado de lo que guardan las variables cambia de acuerdo al tipo de mensaje
	char msg[MAX_MSG_LEN] = "";
	char remitente[MAX_GROUP_NAME] = "";
	char grupos[GRUPOS_MAX][MAX_GROUP_NAME];
	int	num_grupos = -1;
	membership_info info_membresia;
	int	tipo_servicio = ERROR_MESS;
	int16 tipo_mensaje = -1;
	int	endian_mismatch;
	
	error = SP_receive(buzon, &tipo_servicio, remitente, GRUPOS_MAX, &num_grupos, grupos, &tipo_mensaje, &endian_mismatch, MAX_MSG_LEN, msg);
	if (error < 0)
	{
		printf("Error: no se pudo recibir el mensaje\n");
		SP_error(error);
	}
	else
	{
		if (Is_regular_mess(tipo_servicio))
		{
			printf("#%i - Mensaje regular\n", cont_msg);
			printf("Tipo de servicio: %s\n", mapear_tipo_servicio(tipo_servicio));
			printf("Remitente: %s\n", remitente);
			printf("Grupo: %s\n", &grupos[0][0]);
			printf("Mensaje: %s\n", msg);
			printf("Tamanio: %li bytes\n\n", strlen(msg) + 1);
		}
		else if (Is_membership_mess(tipo_servicio))
		{
			error = SP_get_memb_info(msg, tipo_servicio, &info_membresia);
			if (error < 0)
			{
				printf("Error: el mensaje de membresia no tiene un cuerpo valido\n\n");
				SP_error(error);
			}
			else
			{
				if (Is_reg_memb_mess(tipo_servicio))
				{
					printf("#%i - Mensaje de membresia regular\n", cont_msg);

					if (Is_caused_join_mess(tipo_servicio))
						printf("Motivo: el miembro %s se unio al grupo\n", info_membresia.changed_member);
					else if (Is_caused_leave_mess(tipo_servicio))
						printf("Motivo: el miembro %s salio del grupo\n", info_membresia.changed_member);
					else if (Is_caused_disconnect_mess(tipo_servicio))
						printf("Motivo: el miembro %s se desconecto del grupo (abrupta o correctamente)\n", info_membresia.changed_member);
					else if (Is_caused_network_mess(tipo_servicio))
						printf("Motivo: se cayo un miembro, un spread daemon murio o hay una particion en la red\n");

					printf("Grupo: %s\n", remitente);
					printf("Miembros:\n");
					for (int i = 0; i < num_grupos; i++)
						printf("\t%i - %s\n", i + 1, &grupos[i][0]);
					printf("\n");
				}
				else if (Is_transition_mess(tipo_servicio))
				{
					printf("#%i - Mensaje de membresia transicional\n", cont_msg);
					printf("Grupo: %s\n\n", remitente);
				}
				else if (Is_caused_leave_mess(tipo_servicio))
					printf("#%i - Mensaje de membresia de que el presente miembro salio del grupo\n\n", cont_msg);
				else
					printf("#%i - Error: mensaje de membresia invalido\n\n", cont_msg);
			}
		}
	}
}

void recibir_n_mensajes()
{
	if (SP_poll(buzon) > 0)
		while (SP_poll(buzon) > 0)
		{
			cont_msg++;
			recibir_mensaje();
		}
	else
		printf("Error: no hay mensajes para recibir\n");
}









