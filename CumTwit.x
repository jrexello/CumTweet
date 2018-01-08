const MAX_CAD= 120; /*Tamaño del mensaje*/
typedef string mensaje<MAX_CAD>;  /*El mensaje en sí*/
typedef struct usuario_t usuario; /*El tipo usuario*/
typedef string cadena<50>; /*Strings que no sean mensajes*/
typedef struct credenciales_t credenciales; /*Inforación de login y password*/
typedef struct paquete_t paquete; /*Estructura que contiene un mensaje e información relacionada con el mismo*/
typedef struct buscador_t buscador; /*Estructura con la información para solicitar una búsqueda*/
typedef struct nodo * lista;   /* Definición de puntero a la estructura nodo*/

struct buscador_t{
	int tipo; /*Qué tipo de búsqueda va a ser. 1-Recibidos; 2-Enviados.*/
	cadena login; /*El usuario que busca*/
	cadena login_buscado; /*El remitente*/
	cadena fecha; /*La fecha del envio/recibo*/  
};

struct paquete_t{
	mensaje mess; /*El mensaje*/
	cadena dest; /*El destinatario*/
	cadena remite; /*El remitente*/
	cadena fecha; /*La fecha a la que fue enviado*/
	cadena hora; /*La hora a la que fue enviada*/
};

struct usuario_t{
  cadena login; /*El nombre de usuario*/
  cadena nombre; /*El nombre real*/
  cadena apellido; /*El apellido real*/
  cadena passwd; /*Una contraseña*/
  cadena ciudad; /*Ciudad de origen*/
  cadena pais; /*Pais de origen*/
  cadena fecha; /*Fecha en la que se dió de alta*/
};

struct credenciales_t{
	cadena login; /*Nombre de usuario*/
	cadena passwd; /*Contraseña*/
};

/* Estructura de búsqueda. Tenemos un paquete y un puntero al siguiente resultado*/
struct nodo{
    paquete info;    /* La información recogida */
    lista siguiente ; /* siguiente entrada */
};

/* Una union que permite devolver una lista de elementos o error */
union res_busqueda switch (int error) {
     case 0: lista list; /* si no hay error se devuelve una lista con las entradas del directorio*/
     default: void;
};


program CUMTWIT{
  version CT_VERSION{
	void registrar(usuario)=1; /*Función para registrar o borrar usuarios*/
	int enviar(paquete)=2; /*Función para enviar paquete*/
	paquete leer(credenciales)=3; /*Función para leer tus mensajes recibidos*/
	int existeUser(cadena)=4; /*Función para comprobar si existe un usuario*/
	int eliminar(credenciales)=5; /*Función para eliminar un usuario*/
	int logearse(credenciales)=6; /*Función para comprobar credenciales*/
	res_busqueda busca(buscador)=7; /*Función que realiza las búsquedas*/
  }=2; /*Versión 2*/
}=0x20000007; 