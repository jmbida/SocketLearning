#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// code pour le serveur 
// Le rendre compatible avec windows et linux a l'aide de la directive des préprocesseur(macros)

#ifdef _WIN32
    // _WIN32 retourne vrai si le code est compilé sur un système windows
    // On inclut les bibliothèques nécessaires pour windows
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    // _WIN32 retourne faux si le code est compilé sur un système linux (GNU/Linux) ou autre que windows
    // On inclut les bibliothèques nécessaires pour linux
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif

/////////////////Quelques contantes pour la bonne pratique et faciliter des future configuration ou des tests////////////////////////////////////////////

#define PORT 8080 // Port du serveur
#define PENDING_CONNECTIONS_MAX 5 // Nombre de connexions en attente autorisées
#define BUFFER_SIZE 1024 // Taille du tampon pour l'envoi et la réception de données

int main() 
{
// Etape 1: Demarage ou initialisation de la bibliothèque ou système de sockets

    // Initialisation de la bibliothèque de sockets pour windows
    #ifdef _WIN32
        
        WSADATA wsa;

        // WSAStartup est une fonction qui initialise la bibliothèque de sockets pour windows ici on verifie si l'initialisation a réussi ou pas
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            fprintf(stderr, "(SERVEUR) : Echec de l'initialisation de Winsock\n");// Affichage d'un message d'erreur sur la sortie d'erreur standard (stderr)
            exit(1);// On quitte le programme en cas d'erreur
        }
        
    #endif
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Initialisation de la bibliothèque de sockets pour linux (GNU/Linux) ce fait automatiquement lors de l'inclusion des bibliothèques nécessaires pour linux donc pas besoin d'une fonction spécifique pour l'initialisation

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Etape 2: Création d'une socket 
    
    // Creation du socket pour windows et linux (similaire pour les deux systèmes)
    int socketFd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET pour IPv4 (domain), SOCK_STREAM pour TCP (Type de socket), 0 pour le protocole par défaut (TCP dans ce cas)
    if (socketFd == -1) // Vérification de la création du socket
    {
        fprintf(stderr, "(SERVEUR) : Echec de la création du socket\n");// Affichage d'un message d'erreur sur la sortie d'erreur standard (stderr)
        exit(1);// On quitte le programme en cas d'erreur
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Etape 3: Configuration du socket 

    // Configuration du socket pour windows et linux (similaire pour les deux systèmes)
    // 1. Configuration de l'adresse du serveur
    struct sockaddr_in socketAddress; // Structure pour stocker l'adresse du serveur
    socketAddress.sin_family = AF_INET; // Famille d'adresses (IPv4)
    socketAddress.sin_port = PORT; // Port du serveur 
    socketAddress.sin_addr.s_addr = INADDR_ANY; // Adresse IP du serveur (INADDR_ANY pour accepter les connexions de n'importe quelle adresse)

    // 2. Association du socket à l'adresse et au port spécifiés
    int socketAddressSize = sizeof(socketAddress); // Taille de la structure d'adresse du serveur
    int bindResult = bind(socketFd, (struct sockaddr *)&socketAddress, socketAddressSize); // Association du socket à l'adresse et au port spécifiés

    if (bindResult == -1) // Vérification de l'association du socket
    {
        fprintf(stderr, "(SERVEUR) : Echec de l'association du socket\n");// Affichage d'un message d'erreur sur la sortie d'erreur standard (stderr)
        exit(1);// On quitte le programme en cas d'erreur
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Etape 4: Mise en écoute du socket pour accepter les connexions entrantes

    // Mise en écoute du socket pour windows et linux (similaire pour les deux systèmes)
    if (listen(socketFd, PENDING_CONNECTIONS_MAX) == -1) // Vérification de la mise en écoute du socket
    {
        fprintf(stderr, "(SERVEUR) : Echec de la mise en attente de connexion entrante\n");// Affichage d'un message d'erreur sur la sortie d'erreur standard (stderr)
        exit(1);// On quitte le programme en cas d'erreur
    }

    puts("En attente de nouvelle connexion...");// Printf d'un message indiquant que le serveur est en attente de nouvelles connexions pour l'esthétique l'interface du serveur

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Etape 5: Acceptation d'une connexion entrante

    int connectedSocketFd = accept(socketFd, (struct sockaddr *)&socketAddress, (socklen_t *)&socketAddressSize); // Acceptation d'une connexion entrante et création d'un nouveau socket pour la communication avec le client
    if (connectedSocketFd == -1) // Vérification de l'acceptation de la connexion entrante
    {
        fprintf(stderr, "(SERVEUR) : Echec de la connexion\n");// Affichage d'un message d'erreur sur la sortie d'erreur standard (stderr)
        exit(1);// On quitte le programme en cas d'erreur
    }

    puts("Connexion etablie!");// Printf d'un message indiquant que la connexion a été acceptée pour l'esthétique l'interface du serveur
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Etape 6: Communication avec le client (envoi et réception de données)

    // Communication avec le client pour windows et linux (similaire pour les deux systèmes)
    // Bientot disponible !!!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Etape 7: Fermeture et libération de la bibliothèque ou système de sockets

    // Libération de la bibliothèque de sockets pour windows
    #ifdef _WIN32
        closesocket(connectedSocketFd); // Fermeture du socket de communication avec le client
        closesocket(socketFd); // Fermeture du socket d'écoute du serveur
        WSACleanup();
    #else
        close(connectedSocketFd); // Fermeture du socket de communication avec le client pour linux (GNU/Linux)
        close(socketFd); // Fermeture du socket d'écoute du serveur pour linux (GNU/Linux)
    #endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Libération de la bibliothèque de sockets pour linux () ce fait automatiquement lors de la fermeture du programme donc pas besoin d'une fonction spécifique pour la libération

    return 0;
}