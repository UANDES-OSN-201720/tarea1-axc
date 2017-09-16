#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
typedef struct trans{
	char origen[6];
	char destino[6];
	int monto;
	char tipo;
}trans;
typedef struct cuenta{
	int numero;
	char key[6];
	int monto;
}cuenta;
typedef struct sucursal{
	int id; //numero de creacion de la sucursal
	char key[6];
	int nro;
	int tado;
	cuenta* ctas;
	trans* tr;
	int plh[2]; //pipe lecturra hijo
 	int plp[2]; //pipe lectura padre
}sucursal;
int banco;
int ccs; // numero total de cuentas en el banco
int main(int argc, char** argv){
	//hacer la funcion que crea las sucursales al leer init
	sucursal* sucursales;
	banco = getpid();
	char* linea;
	size_t sz= 512;
	linea = malloc(sizeof(linea)*sz);
	int ssales;
	printf("Bienvenido al banco: %i\n", banco);
	while(1){
		getline(&linea, &sz, stdin);
		if(!strncmp("init", linea, 4)){//---------------------------linea del init-----------------------
			//crear sucursal con pipe, primero leer cuantas cuenta debe crear 
			//ahora a leer las cuentas
			ssales++;
			sucursal s;
			s.id = ssales;
			s.tado = 1;
			int d = (int)realloc(sucursales, sizeof(sucursal)*(ssales));
			if(d == NULL){
				free(sucursales);
				printf("error, fallo realloc.\n");
				return -1;}
			sucursales[ssales-1] = s;
			int iden = fork();
			pipe(s.plp);
			pipe(s.plh);
			close(s.plp[1]);//cerrar qrite del padre
			close(s.plh[0]);//cerrar lectura padre
			if (iden < 0 ){
				printf ("fallo la creacion de la sucursal");
				ssales--;
				d = (int)realloc(sucursales, sizeof(sucursal)*(ssales));
				if (d == NULL){
					printf("realloc fallo\n");
				}
			}
			else if(iden==0){
				//codigo del hijo
				sucursales[ssales-1].nro = getpid();
				ccs += 1;//numero sacaddo del stream
				//iniciar threads y las transacciones 
				printf("Se ha creado la sucursal con numero %d.\n", getpid());
				close(sucursales[ssales].plp[0]); //cierra lectura del padre
				close(sucursales[ssales].plh[1]); //cierra write del hijo
				while(1){
					//aca el hijo realiza su pega--------------pega del hijo---------------------------
					//hacer threads, con semaforo, que solo una transaccion escriba en el pipe, al escribir, espera respuesta
					//notar el inicio de cada mensaje del pipe, el primer mensaje es para indicar si las transacciones estan permitidas
					//si no, hace wait a las transacciones hasta que llegue un mensaje, se puede hacer wait haciendo que si recibe 
					//stop en las transacciones lea el pipe esperando indicaciones, si esta en espera de la respuesta, ese movimiento
					//queda fuera de la funcion, hasta que se levante la indicacion
					char msj[512];
					read(sucursales[ssales-1].plh[0], &msj, 512);
					if(!strncmp("k", msj, 1)){
						//free() las cuentas y movimientos
					}
				}
			}
			else{
				sucursales[ssales].nro = iden;
			}
		}
		else if(!strncmp("dump", linea, 4)){//--------------codigo dump, ojo hay que bloquear las transacciones--------
			//comando dump
		}
		else if(!strncmp("list", linea, 4)){//--------------codigo list, ojo hay que bloquear las transacciones--------
			//comando list
		}
		else if(!strncmp("dump_accs", linea, 9)){//--------------codigo dump_accs, ojo hay que bloquear las transacciones--------
			//comando dump_accs
		}
		else if(!strncmp("dump_errs", linea, 9)){//--------------codigo dump_errs, ojo hay que bloquear las transacciones--------
			//comando dump_errs
		}
		else if(!strncmp("kill", linea, 4)){//--------------codigo kill, ojo hay que bloquear las transacciones--------
			//comando kill
			//obtener numero n de sucursal desde la linea
			int i;//numero de creacion de la sucursal
			//busqueda del pid n en sucursales y obtener su id, luego i = id
			char k = 'k';
			sucursales[i-1].tado = 0;
			write(sucursales[i].plh[1], &k, 1);
		}
		else if(!strncmp("quit", linea, 4)){//--------------codigo quit, ojo hay que bloquear las transacciones, FREE THE MALLOCS--------
			for(int i = 0;  &sucursales[i].id != NULL;i++){
				//a liberar
				//enviar pipe de kill en ciclo, con el siguente codigo
				char k = 'k';
				write(sucursales[i].plh[1], &k, 1);
				//free(sucursales[i].ctas);
				//free(sucursales[i].tr);
				//------luego------
				free(sucursales);
				printf("Se ha eliminado el banco, se siente algo en el aire, como si miles de threads gritaran en terror y hayan sido callados...\n");
				return(1);
			}
		}
		else{
			printf("Comando no valido, ingrese nuevamente, verifique que no existan espacios de mas.\n");
		}
		//aca la grandisima sucursal matriz hace de las suyas mandando y recibiendo mensajes, esperemos que no sea brutal 
		//la cantidaad de threads corriendo
		//esrcibe a las sucursales el nuevo numero de cuentas 
	}
}
