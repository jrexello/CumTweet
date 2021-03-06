/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#define _XOPEN_SOURCE //Usamos esto para la encriptación
#include "CumTwit.h" 
#include <unistd.h> //Librería básica para muchas operaciones
#include <crypt.h> //Se usa para la encriptación
#include <time.h> //Se usa para obtener la fecha y hora actual
#include <string.h> //Se usa para modificar los strings
#include <errno.h> //Para la gestión de errores.
extern int error; //El error de la union (Función de búsqueda)

/*A continuación escribimos las cabeceras de las funciones para que el compilador las conozca*/
credenciales *pideDatos();
usuario creaUsuario();
char * dameFecha();
int esBisiesto(int a);
int existeUsuario(char *host, CLIENT *clnt, cadena * cad_usu);

void
cumtwit_2(char *host, int sel) {
    /*Función base. A partir de esta se realizarán las llamadas al servidor según lo que seleccionemos en el menú.
     *host = La dirección del servidor con el que se debe comunicar.
     *sel = Selección del menú. Esta variable es la que decide a qué función se llama en el servidor.
     */
    CLIENT *clnt; //Cliente. En esta dirección de memoria guardaremos toda la información del cliente.
    void *result_1; //Lo que devuelve la función de registro/eliminación.
    int *result_2; //La variable que se devuelve la función "Enviar".
    paquete *result_3; //El paquete que devuelve el servidor en la función "Leer".
    int *result_4; //Valor devuelto por la función que comprueba si existe un usuario.
    int *result_5; //Valor devuelto por el servidor en la función "Eliminar".
    int *result_6; //Valor devuelto por el servidor en la función "logearse", que comprueba las credenciales.
    res_busqueda *result_7; //Valor devuelto por el servidor en la función de búsqueda.
    usuario registrar_2_arg; //La variable que se le envía a la función registrar en el servidor.
    paquete enviar_2_arg; //El paquete (Remitente, destino y mensaje) que se le envía al servidor en la función "enviar".    
    credenciales leer_2_arg; //Las credenciales (login+pass) que se envían al servidor en la función "leer".    
    usuario existeuser_2_arg; //Usuario enviado al servidor en la función "existeUsuario".    
    credenciales eliminar_2_arg; //Credenciales enviadas al servidor en la función "eliminar".
    credenciales logearse_2_arg; //Credenciales enviadas al servidor en la función "logearse".
    credenciales cr_aux; //Credenciales auxiliares. Usada para la copia de datos.
    const char *key; //Donde guardaremos la contraseña para ser encriptada.
    const char *salt = "13"; //El código para encriptar la contraseña
    int ultimo = 0; //Valdrá 1 cuando se llege al último mensaje en la parte de leer mensajes
    int select = -1; //Para la selección de parámetros de búsqueda
    char term; //Para comprobar que el valor insertado en la selección de parámetros de búsqueda es correcto.
    lista nodo_lista; //Alamecenamos los punteros a nodos.
    int continua = 0; //Usaremos esta variable para controlar que no se introducen cadenas incorrectas
    int tam; //Lo usaremos para verificar que no se introducen cadenas demasiado largas.



#ifndef DEBUG
    /*Creamos el cliente*/
    clnt = clnt_create(host, CUMTWIT, CT_VERSION, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    /*Ahora modificamos el tiempo de timeout*/
    struct timeval timeinit; //Aquí guardaremos los datos del nuevo tiempo de espera.
    timeinit.tv_sec = 2; //Pondremos 2 segundos
    timeinit.tv_usec = 0; // Y 0 nanosegundos.
    if (clnt_control(clnt, CLSET_TIMEOUT, &timeinit) == FALSE) { //Le decimos que el tiempo de timeout sea de 2 segundos.
        printf("Error al estipular el timeout\n");
        exit(1);
    }
#endif /* DEBUG */

    switch (sel) {
        case 1: //=======================================================Registrar/Borrar
            registrar_2_arg = creaUsuario(host, clnt); //Ejecutamos el proceso de pedir los datos y guardaremos en una variable de tipo usuario.
            if ((registrar_2_arg.passwd != NULL) && (registrar_2_arg.passwd != "")) { //Enviamos la infomación recogida al servidor para que cree el usuario
                result_1 = registrar_2(&registrar_2_arg, clnt); //TODO: Hacer que devuelva un int e informar del resultado satisfactorio/fallo.
            }
            break;
        case 2: //=======================================================Enviar Mensaje
            cr_aux = *pideDatos(); //Pedimos el usuario y contraseña del usuario que va a enviar el mensaje y lo guardamos en una variable de tipo credenciales.
            result_6 = *logearse_2(&cr_aux, clnt); //Le pedimos al servidor que compruebe si el usuario existe.
            if (result_6 != 1) { //Si NO existe o hay algún fallo...
                printf("Error en la autentificación [PULSE ENTER]\n");
                fflush(stdin); // limpiamos el stdin.
                getchar(); // Esperamos a que se pulse enter.
            } else {//Si el usuario se ha identificado correctamente.
                char in[MAX_CAD]; //Variable auxiliar que usaremos para almacenar, modificar y recoger strings [Caben mensajes].
                char to[50]; //Cadena en la que almacenaremos el usuario del destinatario [No caben mensajes].
                cadena cad_to; //Cadena auxiliar para compatibilizar el usuario destino.
                paquete pkt; //Paquete en el que almacenaremos la información del mensaje. Este será el paquete que le enviemos al servidor.
                fflush(stdin); //Limpiamos el buffer de entrada.
                continua = 0;
                while (continua == 0) { //continua será 0 mientras no se meta un input válido.
                    printf("Introduzca el destinatario: ");
                    fgets(to, 50, stdin);
                    //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
                    if ((to[0] != '\n') && (to[0] != ' ') && to[strlen(to) - 2] != ' ') continua = 1;
                    tam = strlen(to);
                    if (tam == 49) {
                        while (getchar() != '\n');
                        system("clear");
                        continua = 0;
                        printf("El destinatario introducido es demasiado largo.\n");
                        fflush(stdin);
                        getchar();
                    }
                }
                strtok(to, "\n"); //Evitamos que añada un \n al final
                cad_to = strdup(to);
                int *result_4 = 0;
                result_4 = existeuser_2((cadena*) & cad_to, clnt); //Comprobamos si el usuario existe.
                if (result_4 == (int *) NULL) {
                    clnt_perror(clnt, "call failed");
                } else {
                    //printf("Resultado: %d\n", *result_4);
                    if (*result_4 == 0) {
                        printf("El usuario destino NO existe [PULSE ENTER]");
                        fflush(stdin); // limpiamos el stdin
                        getchar(); // Esperamos a que se pulse enter
                    } else if (*result_4 == 1) {
                        continua = 0;
                        while (continua == 0) { //continua será 0 mientras no se meta un input válido.
                            fflush(stdin); // limpiamos el stdin
                            printf("\nIntroduzca el mensaje [Recuerde que no puede tener una logitud superior a %d caracteres]:\n ", MAX_CAD);
                            fgets(in, MAX_CAD - 1, stdin);
                            //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
                            if ((in[0] != '\n') && (in[0] != ' ') && in[strlen(in) - 2] != ' ') continua = 1;
                            tam = strlen(in);
                            if (tam >= MAX_CAD-1) {
                                while (getchar() != '\n');
                                system("clear");
                                continua = 0;
                                printf("El mensaje introducido es demasiado largo.\n");
                                fflush(stdin);
                                getchar();
                            }
                        }
                        strtok(in, "\n"); //Evitamos que añada un \n al final
                        /*Introducimos los datos en el paquete*/
                        pkt.mess = strdup(in);
                        pkt.dest = strdup(to);
                        pkt.remite = strdup(cr_aux.login);
                        pkt.fecha = strdup("");
                        pkt.hora = strdup("");
                        //printf("Se va a enviar un mensaje de %s para %s cuyo contenido es %s\n",pkt.remite, pkt.dest, pkt.mess);
                        result_2 = enviar_2(&pkt, clnt); //Se envía el paquete al servidor.
                        if (result_2 == (int *) NULL) {
                            clnt_perror(clnt, "call failed");
                        } else { //El servidor responde correctamente.
                            if (*result_2 == 1) {
                                printf("Mensaje enviado correctamente [PULSE ENTER]");
                                fflush(stdin); // limpiamos el stdin
                                getchar(); // Esperamos a que se pulse enter
                            }
                        }
                    }
                }
            }
            break;
        case 3: //=======================================================Leer Mensajes

            cr_aux = *pideDatos(); //Verificamos la identidad del usuario
            result_6 = *logearse_2(&cr_aux, clnt);
            if (result_6 != 1) {
                printf("Error en la autentificación [PULSE ENTER]\n");
                fflush(stdin); // limpiamos el stdin
                getchar(); // Esperamos a que se pulse enter
            } else {//El usuario se ha identificado correctamente
                while (ultimo != 1) { //Mientras siga habiendo mensajes, los mostramos.
                    result_3 = leer_2((credenciales *) & cr_aux, clnt);
                    //printf("Leido\n");
                    if (result_3 == (paquete *) NULL) {
                        clnt_perror(clnt, "call failed");
                    } else {
                        if (strcmp("", result_3->mess) != 0) { //Cuando no queden más mensajes si leer el servidor devuelve un paquete con el mensaje "".
                            //Si se ejecuta correctamente, recibimos un paquete.
                            printf("Mensaje recibido.\nRemitente: %s, Fecha: %s, Hora: %s\n\n%s\n\n\n", result_3->remite, result_3->fecha, result_3->hora, result_3->mess);
                            //printf("Mensaje enviado correctamente [PULSE ENTER]");
                            fflush(stdin); // limpiamos el stdin
                            getchar(); // Esperamos a que se pulse enter
                        } else {
                            ultimo = 1;
                            printf("No tiene más mensajes\n");
                            fflush(stdin); // limpiamos el stdin
                            getchar(); // Esperamos a que se pulse enter
                        }
                    }
                }
            }
            break;
        case 4:
            cr_aux = *pideDatos(); //Verificamos la identidad del usuario
            result_6 = *logearse_2(&cr_aux, clnt);
            if (result_6 != 1) {
                printf("Error en la autentificación [PULSE ENTER]\n");
                fflush(stdin); // limpiamos el stdin
                getchar(); // Esperamos a que se pulse enter
            } else {//El usuario se ha identificado correctamente
                //Aquí ya estamos identificados
                while (select == -1) {
                    system("clear");
                    printf("Seleccione un parámetro de búsqueda:\n\t1- Mensajes recibidos\n\t2- Mensajes enviados\n\t0- Volver a menú principal\n\t\t>> ");
                    if (scanf("%d%c", &select, &term) != 2 || term != '\n') { //Si no se introduce un número y se pulsa enter
                        while (getchar() != '\n'); //Para evitar que entre en un bucle
                        select = -1;
                    } else if (select > 2 || select < 0) select = -1;
                }
                //Selección correcta
                /*====================Búsqueda en mensajes====================*/
                char * p_fecha = dameFecha();
                char s_fecha[50];
                char busca_remite[50];
                int continua = 0;
                buscador pkt_bsq; //El paquete que enviaremos con los parámetros de búsqueda.
                strcpy(s_fecha, p_fecha);
                while (continua == 0) {
                    printf("Escriba el usuario del remitente [Deje en blanco para no filtrar por remitente]: \n");
                    fgets(busca_remite, 50, stdin);
                    //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
                    if ((busca_remite[0] == '\n')) {
                        busca_remite[0] = ' ';
                        busca_remite[1] = '\0';
                        continua = 1;
                    } else {
                        if ((busca_remite[0] != ' ') && busca_remite[strlen(busca_remite) - 2] != ' ') continua = 1;
                        tam = strlen(busca_remite);
                        if (tam >= 49) {
                            while (getchar() != '\n');
                            continua = 0;
                            printf("El usuario introducido es demasiado largo.\n");
                            fflush(stdin);
                            getchar();
                        }
                    }
                }
                strtok(busca_remite, "\n"); //Evitamos que añada un \n al final
                pkt_bsq.login = strdup(cr_aux.login);
                pkt_bsq.login_buscado = strdup(busca_remite);
                //printf("Fecha: %s\n", s_fecha);
                pkt_bsq.fecha = strdup(s_fecha);
                pkt_bsq.tipo = select;
                //printf("Se va a buscar: %s, %s, %s\n",pkt_bsq.login, pkt_bsq.login_buscado, pkt_bsq.fecha);
                result_7 = busca_2(&pkt_bsq, clnt);
                nodo_lista = result_7->res_busqueda_u.list;
                int nencontrados = 0;
                char *deA = "De"; //Estético. Según miremos mensajes recibidos o enviados
                if (select == 2) deA = "A";
                while (nodo_lista != NULL && nencontrados<10) {
                    nencontrados++;
                    printf("%s: %s. Mensaje: %s\n", deA, nodo_lista->info.remite, nodo_lista->info.mess);
                    nodo_lista = nodo_lista->siguiente;
                }
                if(nencontrados<10) printf("Fin de la búsqueda. Se han encontrado %d mensajes. [PULSE ENTER]\n", nencontrados);
				else printf("Mostrando 10 mensajes. El servidor informa que podría haber más,");
                fflush(stdin); // limpiamos el stdin
                getchar(); // Esperamos a que se pulse enter
            }
            break;
        default:
            exit(-1);
            break;
    } //End switch
#ifndef DEBUG
    clnt_destroy(clnt);
#endif  /* DEBUG */
}

int existeUsuario(char *host, CLIENT *clnt, cadena *cad_usu) {
/*Esta función se encarga de comprobar si un usuario existe cuando vamos a crear uno nuevo*/
    int *result_4 = 0;
    int continua = 0;
    /*Comprobamos si existe*/
    result_4 = existeuser_2((cadena*) cad_usu, clnt);
    if (result_4 == (int *) NULL) {
        clnt_perror(clnt, "call failed");
    }
    /*Si existe, preguntamos si queremos borrarlo*/
    if (*result_4 == 1) {
        system("clear");
        printf("El usuario ya existe\n¿Desea eliminarlo?(s/n)\n");
        fflush(stdin); //borramos el stdin
        char c = getchar();
        if (c == 's') {
            int *result_5;
            //fflush(stdin);
            credenciales cr_aux;
            cr_aux.login = strdup(*cad_usu);
            setbuf(stdin, NULL);
            char in_pass[50];
            char *pass_p;
			int tam; //Para guardar la longitud del string
            while (continua == 0) {
            pass_p = getpass("Introduzca la contraseña: ");
            if (pass_p == NULL) printf("Error al procesar la contraseña\n");
            else {
            strcpy(in_pass, pass_p); //Pasamos la cadena a un array para poder comprobarla mejor.
            tam = strlen(in_pass);
            if ((tam > 0) && (in_pass[0] != ' ') && in_pass[tam - 1] != ' ') continua = 1;
            if (tam >= 49) {
                while (getchar() != '\n');
                system("clear");
                continua = 0;
                printf("La contraseña introducida es demasiado larga.\n");
                fflush(stdin);
                getchar();
           		 }
      		  }
    		}
    		pass_p = crypt(pass_p, "13");
    		strcpy(in_pass, pass_p);
            cr_aux.passwd = strdup(in_pass);
            result_5 = eliminar_2((credenciales*) &cr_aux, clnt);
            if (result_5 == (int *) NULL) {
                clnt_perror(clnt, "call failed");
            }
            if (*result_5 == 0) {
                printf("\nError al eliminar al usuario ¿Contraseña incorrecta?\n\nPulse [ENTER] para continuar\n");
            } else printf("\nPetición de eliminación enviada...\n\nPulse [ENTER] para continuar\n");
            //fflush(stdin); // limpiamos el stdin
            getchar(); // Esperamos a que se pulse enter

        } else {
            //fflush(stdin); // limpiamos el stdin
            getchar(); // Esperamos a que se pulse enter
        }
        system("clear");
    }

    return (*result_4);


}

int menu() {
    int select = -1;
    char term;
    //setbuf(stdin, NULL); //Borramos el buffer de entrada por si acaso
    while (select == -1) {
        fflush(stdin);
        system("clear");
        printf("***************************************************************************\n");
        printf("Bienvenido al sistema de gestión de mensajes CumTwit\n");
        printf("***************************************************************************\n\n");
        printf("Seleccione una opción:\n\n\t1- Registrar/Dar de baja\n\t2- Enviar mensaje\n\t3- Leer mensaje\n\t4- Buscar\n\t0- Salir\n\t\t>> ");
        if (scanf("%d%c", &select, &term) != 2 || term != '\n') { //Si no se introduce un número y se pulsa enter
            while (getchar() != '\n'); //Para evitar que entre en un bucle
            select = -1;
        } else if (select > 4 || select < 0) select = -1;
    }
    return select;
}

usuario creaUsuario(char *host, CLIENT *clnt) {
/*Esta función solicita los datos de un nuevo usuario, comprueba si existe y luego los envía para que el servidor cree al usuario en cuestión*/
    usuario usAux; //La variable que devolveremos.
    int tam; //Aquí almacenamos el tamaño del array para comprobar que no se introducen cadenas muy largas.
    int continua = 0; //Usamos esta variable para evitar que el usuario introduzca caracteres no válidos.
    char in[50]; //El string en el que guardaremos el texto introducido.
    char pass1[100]; //Para comparar las contraseñas
	char pass2[100]; //Para comparar las contraseñas
    setbuf(stdin, NULL); //Vaciamos el buffer de entrada
    while (continua == 0) {
        system("clear");
        printf("\n\nIntroduzca el login del nuevo usuario: ");
        fgets(in, 50, stdin);
        //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
        if ((in[0] != '\n') && (in[0] != ' ') && in[strlen(in) - 2] != ' ') continua = 1;
        tam = strlen(in);
        if (tam >= 49) {
            while (getchar() != '\n');
            system("clear");
            continua = 0;
            printf("El nombre introducido es demasiado largo.\n");
            fflush(stdin);
            getchar();
        }
    }
    continua = 0; //Restablecemos el continua
    strtok(in, "\n"); //Evitamos que añada un \n al final
    usAux.login = strdup(in);
    cadena *pCadena = &usAux.login;
    //Comprobaciópn de si exite el usuario*************************************
    if (existeUsuario(host, clnt, pCadena) == 0) {
        //Fin de la comprobación********************************
        while (continua == 0) {
            printf("\n\nIntroduzca el nombre del nuevo usuario: ");
            fgets(in, 50, stdin);
            //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
            tam = strlen(in);           
            if ((in[0] != '\n') && (in[0] != ' ') && in[tam - 2] != ' ') continua = 1;
            if (tam >= 49) {
                while (getchar() != '\n');
                system("clear");
                continua = 0;
                printf("El nombre introducido es demasiado largo.\n");
                fflush(stdin);
                getchar();
            }
        }
        strtok(in, "\n"); //Evitamos que añada un \n al final
        usAux.nombre = strdup(in);
        continua = 0;
        while (continua == 0) {
            printf("\n\nIntroduzca el apellido del nuevo usuario: ");
            fgets(in, 50, stdin);
            //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
			tam = strlen(in);
            if ((in[0] != '\n') && (in[0] != ' ') && in[tam - 2] != ' ') continua = 1;            
            if (tam >= 49) {
                while (getchar() != '\n');
                system("clear");
                continua = 0;
                printf("El apellido introducido es demasiado largo.\n");
                fflush(stdin);
                getchar();
            }
        }
        strtok(in, "\n"); //Evitamos que añada un \n al final
        usAux.apellido = strdup(in);
        continua = 0;
        //Por motivos de seguridad comprobamos la contraseña dos veces.
        char *p_pass;
        while (continua == 0) {
            p_pass = getpass("\n\nIntroduzca la contraseña: ");
            if (p_pass == NULL) printf("Error al procesar la contraseña\n");
            else {
                strcpy(in, p_pass); //Pasamos la cadena a un array para poder comprobarla mejor.
                tam = strlen(in);
                if ((tam > 0) && (in[0] != ' ') && (in[tam - 1] != ' ')) continua = 1;
                if (tam >= 49) {
                    while (getchar() != '\n');
                    system("clear");
                    continua = 0;
                    printf("La contraseña introducida es demasiado larga.\n");
                    fflush(stdin);
                    getchar();
                }
            }
        }
        p_pass = crypt(p_pass, "13");
        strcpy(pass1, p_pass);
        //Volvemos a pedirla
        continua = 0;
        while (continua == 0) {
            p_pass = getpass("\n\nVuelva a introducir la contraseña: ");
            if (p_pass == NULL) printf("Error al procesar la contraseña\n");
            else {
                strcpy(in, p_pass); //Pasamos la cadena a un array para poder comprobarla mejor.
                tam = strlen(in);
                if ((tam > 0) && (in[0] != ' ') && (in[tam - 1] != ' ')) continua = 1;
                if (strlen(in) >= 49) {
                    while (getchar() != '\n');
                    system("clear");
                    continua = 0;
                    printf("La contraseña introducida es demasiado larga.\n");
                    fflush(stdin);
                    getchar();
                }
            }
        }
        p_pass = crypt(p_pass, "13");
        strcpy(pass2, p_pass);
        if(strcmp(pass1, pass2) != 0){ //Si NO son iguales
            /*Rellenamos el struct que vamos a devolver*/
            usAux.passwd = strdup("");
            usAux.ciudad = strdup("");
            usAux.pais = strdup("");
            printf("La contraseña no coincide\n");
            fflush(stdin);
            getchar();
        }
        else{
        usAux.passwd = strdup(pass1);
        continua = 0;
        while (continua == 0) {
            printf("\n\nIntroduzca la ciudad del nuevo usuario: ");
            fgets(in, 50, stdin);
            //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
            tam = strlen(in);
            if ((in[0] != '\n') && (in[0] != ' ') && in[tam - 2] != ' ') continua = 1;
            if (tam >= 49) {
                while (getchar() != '\n');
                system("clear");
                continua = 0;
                printf("La ciudad introducida es demasiado larga.\n");
                fflush(stdin);
                getchar();
            }
        }
        strtok(in, "\n"); //Evitamos que añada un \n al final
        usAux.ciudad = strdup(in);
        continua = 0;
        while (continua == 0) {
            printf("\n\nIntroduzca el pais del nuevo usuario: ");
            fgets(in, 50, stdin);
            //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
            tam = strlen(in);
            if ((in[0] != '\n') && (in[0] != ' ') && in[tam - 2] != ' ') continua = 1;
            if (tam >= 49) {
                while (getchar() != '\n');
                system("clear");
                continua = 0;
                printf("El pais introducido es demasiado largo.\n");
                fflush(stdin);
                getchar();
            }
        }
        strtok(in, "\n"); //Evitamos que añada un \n al final
        usAux.pais = strdup(in);
        //Primero transformamos la fecha a string
        time_t rawtime;
        if (time(&rawtime) == ((time_t) - 1)) { //Error
            printf("Ha ocurrido un error al programar la fecha y hora.\n Error: %s\n", strerror(errno));
            usAux.fecha = strdup(" ");
        } else {
            struct tm *timeinfo = localtime(&rawtime);
            if (strftime(in, sizeof (in) - 1, "%d/%m/%Y", timeinfo) == 0) { //Error
                printf("Ha ocurrido un error al guardar el string de la fecha.\n");
                usAux.fecha = strdup(" ");
            } else usAux.fecha = strdup(in);
        }

    }
    }
    return usAux;

}

int
main(int argc, char *argv[]) {
    char *host;
    int sel; //Selección del menú

    if (argc < 2) {
        printf("usage: %s server_host\n", argv[0]);
        exit(1);
    }
    host = argv[1];
    while (sel != 0) {
        sel = 0;
        sel = menu();
        system("clear");
        if (sel == 0) exit(0);
        cumtwit_2(host, sel);
    }
    exit(0);
}

credenciales *pideDatos() {
    char in[50];
    char *pass_p;
    int tam; //Aquí guardaremos el tamaño de los datos introducidos
    credenciales ret;
    system("clear");
    setbuf(stdin, NULL);
    int continua = 0;
    while (continua == 0) {
        printf("\n\nIntroduzca un usuario: ");
        fgets(in, 50, stdin);
        //Nos aseguramos de que la cadena introducida no esté vacía, no comience en espacio y no acabe en espacio
        tam = strlen(in);
        if ((in[0] != '\n') && (in[0] != ' ') && in[tam - 2] != ' ') continua = 1;
        if (tam >= 49) {
            while (getchar() != '\n');
            system("clear");
            continua = 0;
            printf("El usuario introducido es demasiado largo.\n");
            fflush(stdin);
            getchar();
        }
    }
    strtok(in, "\n"); //Evitamos que añada un \n al final
    ret.login = strdup(in);
    continua = 0;
    //Por motivos de seguridad no comprobamos la contraseña.
    char *p_pass;

    while (continua == 0) {
        p_pass = getpass("\n\nIntroduzca la contraseña: ");
        if (p_pass == NULL) printf("Error al procesar la contraseña\n");
        else {
            strcpy(in, p_pass); //Pasamos la cadena a un array para poder comprobarla mejor.
            tam = strlen(in);
            if ((tam > 0) && (in[0] != ' ') && in[tam - 1] != ' ') continua = 1;
            if (tam >= 49) {
                while (getchar() != '\n');
                system("clear");
                continua = 0;
                printf("La contraseña introducida es demasiado larga.\n");
                fflush(stdin);
                getchar();
            }
        }
    }
    p_pass = crypt(p_pass, "13");
    strcpy(in, p_pass);
    ret.passwd = strdup(in);
    system("clear");
    return &ret;
}

char * dameFecha() {
    /*Función auxiliar para obtener la representación en string de una fecha dada por un usuario*/
    int anyoActual; //Le pedimos al sistema que nos devuelva el año anyoActual
    char in[50]; //Variable auxiliar
    cadena salida; //Lo que se devolverá.
    int in_anyo = -1;
    int in_mes = -1;
    int in_dia = -1;
    char term;
    //===========================AÑO===========================
    time_t rawtime;
    if (time(&rawtime) == ((time_t) - 1)) { //Error
        printf("Ha ocurrido un error al programar la fecha y hora.\n Error: %s\n", strerror(errno));
        anyoActual = 2016;
    } else {
        struct tm *timeinfo = localtime(&rawtime);
        if (strftime(in, sizeof (in) - 1, "%Y", timeinfo) == 0) { //Error
            printf("Ha ocurrido un error al guardar el string de la fecha.\n");
            anyoActual = 2016;
        } else anyoActual = atoi(in);
    }
    if (anyoActual == 0) anyoActual = 2016; //En caso de error, atoi devuelve 0
    while (in_anyo == -1) {
        setbuf(stdin, NULL); //limpiamos el buffer de entrada
        printf("\nIntroduzca un año [2000-%d]: ", anyoActual);
        if (scanf("%d%c", &in_anyo, &term) != 2 || term != '\n') { //Si no se introduce un número y se pulsa enter		  	
            while (getchar() != '\n') //Para evitar que entre en un bucle
                in_anyo = -1;
        } else if (in_anyo > anyoActual || in_anyo < 2000) in_anyo = -1;
    }

    //===========================MES===========================
    while (in_mes == -1) {
        setbuf(stdin, NULL); //limpiamos el buffer de entrada
        printf("\nIntroduzca un mes [1-12]: ");
        if (scanf("%d%c", &in_mes, &term) != 2 || term != '\n') { //Si no se introduce un número y se pulsa enter		  	
            while (getchar() != '\n') //Para evitar que entre en un bucle
                in_mes = -1;
        } else if (in_mes < 1 || in_mes > 12) in_dia = -1;
    }


    //===========================DÍA===========================
    int daysInMonth = 31 - ((in_mes == 2) ? (3 - esBisiesto(in_anyo)) : ((in_mes - 1) % 7 % 2));
    while (in_dia == -1) {
        setbuf(stdin, NULL); //limpiamos el buffer de entrada
        printf("\nIntroduzca un día [1-%d]: ", daysInMonth);
        if (scanf("%d%c", &in_dia, &term) != 2 || term != '\n') { //Si no se introduce un número y se pulsa enter		  	
            while (getchar() != '\n') //Para evitar que entre en un bucle
                in_dia = -1;
        } else if (in_dia < 1 || in_dia > daysInMonth) in_dia = -1;
    }
    /*Ahora, en el caso de que el mes sea uno de los 9 primeros (Un solo dígito) le añadimos un cero delante para que coincida con el formato*/
    if ((in_mes < 10) && (in_dia >= 10)) sprintf(in, "%d/0%d/%d", in_dia, in_mes, in_anyo); //Transformamos el int a un string
    else if ((in_mes < 10) && (in_dia < 10)) sprintf(in, "0%d/0%d/%d", in_dia, in_mes, in_anyo); //Transformamos el int a un string
    else if ((in_mes >= 10) && (in_dia < 10)) sprintf(in, "0%d/%d/%d", in_dia, in_mes, in_anyo); //Transformamos el int a un string
    else sprintf(in, "%d/%d/%d", in_dia, in_mes, in_anyo); //Transformamos el int a un string

    return &in;

}

int esBisiesto(int a) {
    /*Método para calcular si el año introducido es o no bisiesto*/
    if (a % 400 == 0)
        return 1;
    else if (a % 100 == 0)
        return 0;
    else if (a % 4 == 0)
        return 1;
    else
        return 0;
}
