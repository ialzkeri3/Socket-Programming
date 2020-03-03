/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Your Name Here
* PROJECT:	CS 3251 Project 1 - Professor Jun Xu 
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*/
///////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/* Constants */
#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define REPLYLEN 32

/* The main function */

int accountID(char acount_name[])
{
    char accounts[5][15] = {"myCD", "my529", "my401k", "mySavings", "myChecking"};
    int i;

    for (i = 0; i <= 4; i++)
    {
        if (strcmp(accounts[i], acount_name) == 0)
        {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{

    int clientSock;               /* socket descriptor */
    struct sockaddr_in serv_addr; /* server address structure */

    int accountID1; /* Account Name  */
    int accountID2;
    char *CMD;
    char *servIP;            /* Server IP address  */
    unsigned short servPort; /* Server Port number */

    char sndBuf[SNDBUFSIZE]; /* Send Buffer */
    char rcvBuf[RCVBUFSIZE]; /* Receive Buffer */

    int balance; /* Account balance */
    int ammount;

    /* Get the Account ID from the command line */
    accountID1 = accountID(argv[2]);

    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Create a new TCP socket*/
    clientSock = socket(AF_INET, SOCK_STREAM, 0);

    /* Construct the server address structure */
    servIP = "127.0.0.1";
    servPort = htons(9010);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = servPort;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* Establish connecction to the server */
    int connection_status = connect(clientSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (connection_status == -1)

    {
        printf("couldn't connect sockets \n");
        return 0;
    }

    if (argc < 5 || argc > 7)
    {

        printf("Error, reguest doesn't satisfy requirements \nRequest should be of the form: \n./client CMD [testaccount] [amount] ipAddr port\nWhere CMD --> BAL, WITHDRAW, TRANSFER\n[testaccount] should include two accounts for transfer\n[ammount] is not included for BAL\n");
        return 0;
    }

    /* Send CMD type to the server */
    snprintf(sndBuf, sizeof(sndBuf), "%d", argc);
    send(clientSock, sndBuf, sizeof(sndBuf), 0);

    if (argc == 5)
    {

        if (strcmp(argv[1], "BAL") != 0)
        {
            printf("Error, reguest doesn't satisfy requirements \nRequest should be of the form: \n./client CMD [testaccount] [amount] ipAddr port\nWhere CMD --> BAL, WITHDRAW, TRANSFER\n[testaccount] should include two accounts for transfer\n[ammount] is not included for BAL\n");
            return 0;
        }

        if (accountID1 == -1)
        {
            printf("Error: Wrong account \n");
            return 0;
        }

        /* send account ID to the server */
        snprintf(sndBuf, sizeof(sndBuf), "%d", accountID1);
        send(clientSock, sndBuf, sizeof(sndBuf), 0);

        /* Receive and print response from the server */
        recv(clientSock, &rcvBuf, sizeof(rcvBuf), 0);
        printf("Balance of %s is %s\n", argv[2], rcvBuf);

        return 0;
    }

    if (argc == 6)
    {

        if (strcmp(argv[1], "WITHDRAW") != 0)
        {
            printf("Error, reguest doesn't satisfy requirements \nRequest should be of the form: \n./client CMD [testaccount] [amount] ipAddr port\nWhere CMD --> BAL, WITHDRAW, TRANSFER\n[testaccount] should include two accounts for transfer\n[ammount] is not included for BAL\n");
            return 0;
        }

        if (accountID1 == -1)
        {
            printf("Error: Wrong account \n");
            return 0;
        }

        /* Extract ammount from the command line*/
        ammount = atoi(argv[3]);

        /* send account ID and ammount to the server */
        snprintf(sndBuf, sizeof(sndBuf), "%d", accountID1);
        send(clientSock, sndBuf, sizeof(sndBuf), 0);

        snprintf(sndBuf, sizeof(sndBuf), "%d", ammount);
        send(clientSock, sndBuf, sizeof(sndBuf), 0);

        /* Receive and print response from the server */
        recv(clientSock, &rcvBuf, sizeof(rcvBuf), 0);
        printf("%s \n", rcvBuf);

        return 0;
    }

    if (argc == 7)
    {
        if (strcmp(argv[1], "TRANSFER") != 0)
        {
            printf("Error, reguest doesn't satisfy requirements \nRequest should be of the form: \n./client CMD [testaccount] [amount] ipAddr port\nWhere CMD --> BAL, WITHDRAW, TRANSFER\n[testaccount] should include two accounts for transfer\n[ammount] is not included for BAL\n");
            return 0;
        }

        if (accountID1 == -1)
        {
            printf("Error: First account is wrong \n");
            return 0;
        }

        accountID1 = accountID(argv[2]);
        accountID2 = accountID(argv[3]);
        ammount = atoi(argv[4]);

        if (accountID2 == -1)
        {
            printf("Error: Second account is wrong \n");
            return 0;
        }

        /* send account IDs and ammount to the server */
        snprintf(sndBuf, sizeof(sndBuf), "%d", accountID1);
        send(clientSock, sndBuf, sizeof(sndBuf), 0);

        snprintf(sndBuf, sizeof(sndBuf), "%d", accountID2);
        send(clientSock, sndBuf, sizeof(sndBuf), 0);

        snprintf(sndBuf, sizeof(sndBuf), "%d", ammount);
        send(clientSock, sndBuf, sizeof(sndBuf), 0);

        /* Receive and print response from the server */
        recv(clientSock, &rcvBuf, sizeof(rcvBuf), 0);
        printf("%s \n", rcvBuf);

        return 0;
    }

    return 0;
}
