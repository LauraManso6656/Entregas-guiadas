#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	
	//inicialicaciones
	//abrimos socket
	if ((sock_listen =socket(AF_INET, SOCK_STREAM, 0))<0)
		printf("Error creando socket");
	// Hacemos el bind al puerto
	memset(&serv_adr, 0, sizeof(serv_adr)); // Inicializa a 0 el serv_adr
	serv_adr.sin_family=AF_INET;
	//asocia el socket a cualquiera de las IP de la maquina
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	//escuchamos en el puerto 9050
	serv_adr.sin_port=htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr))<0)
		printf("error al bind");
	if (listen(sock_listen, 3)<0)
		printf("error al listen");
	int i;
	
	//atendemos solo 5 peticiones
	for (;;){
		printf("escuchando\n");
		
		
		sock_conn=accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		//blucle de atencion al cliente
		int terminar=0;
		while (terminar==0){
			
		
		//ahora recibimos su nombre, que dejamos en buff
		ret=read(sock_conn, peticion,sizeof(peticion));
		printf("recibido\n");
		
		//tenemos que añadirle la marca de fin de string para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		//escribimos el nombre en la consola
		printf("peticion: %s\n", peticion);
		
		//vamos a ver que quieren
		
		char *p=strtok(peticion, "/");
		int codigo= atoi(p);
		char nombre[20];
		if (codigo!=0){
			
		
		p=strtok(NULL,"/");
	
		strcpy(nombre,p);
		printf("codigo: %d, Nombre: %s\n", codigo, nombre);
		}
		if(codigo==0)
			terminar=1;
		
		
		else if (codigo==1) //piden la longitud del nombre
			sprintf(respuesta, "%d", strlen(nombre));
		else if(codigo==2)
			//quieren saber si el nombre es bonito
			if((nombre[0]=='M') || (nombre[0]=='s'))
			strcpy (respuesta, "SI");
			else 
				strcpy(respuesta, "NO");
		else{//decir si es alto
			p=strtok(NULL,"/");
			float altura=atof(p);
			if(altura>1.70)
				sprintf (respuesta, "%s: eres alto", nombre);
			else
				
				sprintf (respuesta, "%s: eres bajo", nombre);
			
		
			
		}
		
		if (codigo!=0)
		{
		printf("respuesta:%s\n", respuesta);
		//y lo enviamos
		write(sock_conn, respuesta, strlen(respuesta));
		}
		
		//se acabo el servicio para este cliente
		close(sock_conn);
		
		
	}
	}
	

}

