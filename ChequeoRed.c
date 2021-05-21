/*
 * Instrucciones de uso
 * 1. gcc ChequeoRed.c -o ChequeoRed      --> Compila
 * 2. ./ChequeoRed                        --> ejecuta
 * 3. cat /var/log/ChequeoRed             --> revisar el log
 * 
 * ps -eo 'tty,pid,comm' | grep ^?        --> ver los procesos (para hacer kill)
 * 
 * Developed by: 
 * Mariño, Nahuel
 * Navarro, Fernando
 * 
 * Sistemas de Red Basados en Software Libre
 * Facultad de Ingeniería
 * Universidad Nacional de Mar Del Plata
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h> 
#include <string.h> // para las strings
#include <math.h> // para el while místico
#include <time.h> // para poner fecha a los msj
#include <sys/socket.h> //
#include <netinet/tcp.h> //
#include <netinet/in.h> //
#include <netdb.h> //
 
// Basado en:
// https://stackoverflow.com/questions/10535255/check-internet-connection-with-sockets
int hayInternet()
{
    int sockfd,val;
    char buffer[1024];
    struct hostent *google_ent=NULL;
    struct sockaddr_in google_addr;
    sockfd = -1;

    // Por ahora vamos a confiar en google...
    // Esperemos que nadie compre el dominio de google y lo tire porque
    // nuestro programa va a dejar de funcionar
    if((google_ent = gethostbyname("www.google.com")) != NULL)
    {
        if((sockfd = socket(google_ent->h_addrtype,SOCK_STREAM,IPPROTO_TCP)) != -1)
        {
            val = 1;
            if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, (char *) &val, sizeof(val)) == 0 && setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY, (char *) &val, sizeof(val)) == 0)
            {
                google_addr.sin_family = google_ent->h_addrtype;
                memcpy(&(google_addr.sin_addr), google_ent->h_addr, google_ent->h_length);
                google_addr.sin_port = htons(80);
                if( connect(sockfd,(struct sockaddr *) &google_addr,sizeof(google_addr)) == 0)
                {
                    if(write(sockfd,"GET /index.html HTTP/1.1\r\n\r\n", 29) >= 28)
                    {
                        shutdown(sockfd, SHUT_WR);
                        if(read(sockfd, buffer, 1024) != -1) // leimos, todo piola
                        {
                            close(sockfd);
                            return 1;
                        }
                    }
                }
            }
        }
    }

    if(sockfd!=-1)
        close(sockfd);
    return 0; // si llegue a este return no hay internet
}

void guardarLog(char* texto)
{
    FILE* arch;
    char fecha[100] = {};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(fecha,99,"ChequeoRed (%02d-%02d-%d %02d:%02d:%02d):", tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    char* log = strcat(fecha,texto);
    arch = fopen("/var/log/ChequeoRed","a");
    fprintf(arch,log);
    fclose(arch);
}

// FUENTE DE ESTA FUNCIÓN: 
// https://github.com/pasce/daemon-skeleton-linux-c
static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
}

int main()
{
    int estaConectado = 0;
    int i = 0;
    float pi = M_PI; // π de Math.h
    skeleton_daemon();

    // while (1)
    while (pi - 3.1415926535897 < 0.00001)
    {
        if(hayInternet())
        {
            if(estaConectado == 0)
            {
                estaConectado = 1;
                guardarLog("[CAMBIO]: HAY CONEXIÓN\n");
            }
        }
        else
        {
            if(estaConectado == 1)
            {
                estaConectado = 0;
                guardarLog("[CAMBIO]: SE FUE LA CONEXIÓN\n");
            }
        }
        sleep(5);
    }

    guardarLog("Proceso finalizado.");
    return EXIT_SUCCESS;
}