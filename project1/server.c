/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Your Name Here
* PROJECT:	CS 3251 Project 1 - Professor Jun Xu
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*/
///////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* supports all sorts of functionality */
#include <unistd.h>     /* for close() */
#include <string.h>     /* support any string ops */
#include <time.h>

#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define BUFSIZE 40     /* Your name can be as many as 40 chars*/

/**
 * Account IDs:
 * myCD         ----------> 0
 * my529        ----------> 1
 * my401k       ----------> 2
 * mySavings    ----------> 3
 * myChecking   ----------> 4
 * */

/* The main function */

int main(int argc, char *argv[])
{

    int serverSock;                    /* Server Socket */
    int clientSock;                    /* Client Socket */
    struct sockaddr_in changeServAddr; /* Local address */
    struct sockaddr_in changeClntAddr; /* Client address */
    unsigned short changeServPort;     /* Server port */
    socklen_t clntLen;                 /* Length of address data struct */

    char nameBuf[BUFSIZE]; /* Buff to store account name from client */
    int balance;           /* Place to record account balance result */
    int accountID1;
    int accountID2;

    int balances[5] = {1000, 2000, 3000, 4000, 5000}; // an array to hold accounts balance
    char client_msg[512];
    char server_msg[512];
    char CMD[512]; // the request type: 5 => BALANCE || 6 => WITHDRAW || 7 => TRANSFER

    /* Create new TCP Socket for incoming requests*/
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    /* Construct local address structure*/
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_port = htons(9010);
    changeServAddr.sin_addr.s_addr = INADDR_ANY;

    /* Bind to local address structure */

    bind(serverSock, (struct sockaddr *)&changeServAddr, sizeof(changeServAddr));

    /* Listen for incoming connections */
    listen(serverSock, 5);

    /* Variables to manage security problem */
    time_t wTime1[256]; //withdrawing times from first account
    time_t wTime2[256];
    time_t wTime3[256];
    time_t wTime4[256];
    time_t wTime5[256];

    int withdrawNumber1 = 0; //from account1 (myCD)
    int withdrawNumber2 = 0; //from account2 (my529)
    int withdrawNumber3 = 0; //from account3 (my401k)
    int withdrawNumber4 = 0; //from account4 (mySavings)
    int withdrawNumber5 = 0; //from account5 (myChecking)

    int isAllowed = 1; // is withdrae allowed ? 1:yes 0:no

    /* Loop server forever */

    while (1)
    {

        /* Accept incoming connection */
        clntLen = sizeof(changeClntAddr);
        clientSock = accept(serverSock, (struct sockaddr *)&changeClntAddr, &clntLen);

        // receive the command (CMD) type
        recv(clientSock, &CMD, sizeof(CMD), 0);

        /* Extract the account ID from the packet */
        recv(clientSock, &client_msg, sizeof(client_msg), 0);
        accountID1 = atoi(client_msg);

        if (atoi(CMD) == 5)
        { /* Look up account balance, and return it to client */
            snprintf(server_msg, sizeof(server_msg), "%d", balances[accountID1]);
            send(clientSock, server_msg, sizeof(server_msg), 0);
        }

        if (atoi(CMD) == 6)
        {
            /* Recieve withdraw ammount */
            recv(clientSock, &client_msg, sizeof(client_msg), 0);
            int withdraw_ammount = atoi(client_msg);

            /* Manage security if account is myCD */
            isAllowed = 1;

            if (accountID1 == 0)
            {
                withdrawNumber1++;                    // update withdraw attempts for this account
                wTime1[withdrawNumber1] = time(NULL); //store current time to the corresponding withdraw attempts

                /* Make sure that one min has passed from before the coming attempt*/
                if (withdrawNumber1 > 3)
                {
                    if (difftime(wTime1[withdrawNumber1], wTime1[withdrawNumber1 - 3]) <= 60)
                    {
                        char security_error[512] = "Error: too many withdrawals in aminute !!";
                        snprintf(server_msg, sizeof(server_msg), "%s", security_error);
                        send(clientSock, server_msg, sizeof(server_msg), 0);
                        withdrawNumber1--;
                        isAllowed = 0;
                    }
                }
            }

            /* Manage security if account is my529 */
            if (accountID1 == 1)
            {
                withdrawNumber2++;
                wTime2[withdrawNumber2] = time(NULL);

                if (withdrawNumber2 > 3)
                {
                    if (difftime(wTime2[withdrawNumber2], wTime2[withdrawNumber2 - 3]) <= 60)
                    {
                        char security_error[512] = "Error: too many withdrawals in aminute !!";
                        snprintf(server_msg, sizeof(server_msg), "%s", security_error);
                        send(clientSock, server_msg, sizeof(server_msg), 0);
                        withdrawNumber2--;
                        isAllowed = 0;
                    }
                }
            }

            /* Manage security if account is my401k */
            if (accountID1 == 2)
            {
                withdrawNumber3++;
                wTime3[withdrawNumber3] = time(NULL);

                if (withdrawNumber3 > 3)
                {
                    if (difftime(wTime3[withdrawNumber3], wTime3[withdrawNumber3 - 3]) <= 60)
                    {
                        char security_error[512] = "Error: too many withdrawals in aminute !!";
                        snprintf(server_msg, sizeof(server_msg), "%s", security_error);
                        send(clientSock, server_msg, sizeof(server_msg), 0);
                        withdrawNumber3--;
                        isAllowed = 0;
                    }
                }
            }

            /* Manage security if account is mySavings */
            if (accountID1 == 3)
            {
                withdrawNumber4++;
                wTime4[withdrawNumber4] = time(NULL);

                if (withdrawNumber4 > 3)
                {
                    if (difftime(wTime4[withdrawNumber4], wTime4[withdrawNumber4 - 3]) <= 60)
                    {
                        char security_error[512] = "Error: too many withdrawals in aminute !!";
                        snprintf(server_msg, sizeof(server_msg), "%s", security_error);
                        send(clientSock, server_msg, sizeof(server_msg), 0);
                        withdrawNumber4--;
                        isAllowed = 0;
                    }
                }
            }

            /* Manage security if account is myChecking */
            if (accountID1 == 4)
            {
                withdrawNumber5++;
                wTime5[withdrawNumber5] = time(NULL);

                if (withdrawNumber5 > 3)
                {
                    if (difftime(wTime5[withdrawNumber5], wTime5[withdrawNumber5 - 3]) <= 60)
                    {
                        char security_error[512] = "Error: too many withdrawals in a minute !!";
                        snprintf(server_msg, sizeof(server_msg), "%s", security_error);
                        send(clientSock, server_msg, sizeof(server_msg), 0);
                        withdrawNumber5--;
                        isAllowed = 0;
                    }
                }
            }

            /* Chech if there is enough balance to withdraw form with the givin ammount */
            if (isAllowed == 1)
            {
                if (withdraw_ammount > balances[accountID1])
                {
                    char fail_msg[512] = "Withdraw Failed: no enough blalance";
                    snprintf(server_msg, sizeof(server_msg), "%s", fail_msg);
                    send(clientSock, server_msg, sizeof(server_msg), 0);
                }
                else
                {
                    balances[accountID1] = balances[accountID1] - withdraw_ammount;
                    char success_msg[512] = "Withdraw Succeded!";
                    snprintf(server_msg, sizeof(server_msg), "%s", success_msg);
                    send(clientSock, server_msg, sizeof(server_msg), 0);
                }
            }
        }

        if (atoi(CMD) == 7)
        {
            /* Extract the second account ID from the packet */
            recv(clientSock, &client_msg, sizeof(client_msg), 0);
            accountID2 = atoi(client_msg);

            /* Extract the ammount to be transfered */
            recv(clientSock, &client_msg, sizeof(client_msg), 0);
            int transfer_ammount = atoi(client_msg);

            /* Chech if there is enough balance to make the the transfer with the givin ammount */
            if (transfer_ammount > balances[accountID1])
            {
                char fail_msg[512] = "Transfer Failed: no enough blalance in (from) account";
                snprintf(server_msg, sizeof(server_msg), "%s", fail_msg);
                send(clientSock, server_msg, sizeof(server_msg), 0);
            }
            else
            {
                balances[accountID1] = balances[accountID1] - transfer_ammount;
                balances[accountID2] = balances[accountID2] + transfer_ammount;
                char success_msg[512] = "Transfer Succeded!";
                snprintf(server_msg, sizeof(server_msg), "%s", success_msg);
                send(clientSock, server_msg, sizeof(server_msg), 0);
            }
        }
    }
}
