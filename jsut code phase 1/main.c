#include <stdio.h>
#include "cJSON.h"
#include "cJSON.c"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12344
#define SA struct sockaddr
char AuthToken[MAX];
int client_socket, server_socket;
struct sockaddr_in servaddr, cli;
int flag2 = 1;
void leave()
{
    char sending[MAX];
    char buffer[MAX];
    sprintf(sending,"leave %s\n", AuthToken);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("GOOD LUCK...\n");
    return;
}
void members()
{
    char sending[MAX];
    char buffer[1000];
    sprintf(sending,"channel members %s\n", AuthToken);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON * recived = cJSON_Parse(buffer);
    //closesocket(client_socket);
    cJSON * parsed_recived = cJSON_GetObjectItem(recived, "content");
    int array_size = cJSON_GetArraySize(parsed_recived);
    printf("MEMBERS : \n");
    for(int i = 0; i< array_size ; i++)
    {
        printf("%s    ",(cJSON_GetArrayItem(parsed_recived, i)-> valuestring));
        if(i == array_size - 1)
            printf("\n");
    }
}
void refresh()
{
    char sending[MAX];
    char buffer [10000];
    sprintf(sending,"refresh %s\n", AuthToken);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    //closesocket(client_socket);
    cJSON * recived = cJSON_Parse(buffer);
    cJSON* parsed_recived = cJSON_GetObjectItem(recived, "content");
    int array_size = cJSON_GetArraySize(parsed_recived);
    for(int i = 0; i <array_size; i++)
    {
        printf ("%s say:  ", cJSON_GetObjectItem (cJSON_GetArrayItem (parsed_recived , i), "sender") -> valuestring);
        printf ("%s\n", cJSON_GetObjectItem (cJSON_GetArrayItem (parsed_recived , i), "content") -> valuestring);
        if(i == array_size - 1)
            printf("\n");
    }
    return;
}
void inchannel()
{
    int flag = 1;
    while(flag)
    {
        int insert = 0;
        printf("\nMenu 3\n\n1: Send Massage \n2: Refresh \n3: Channel Members \n4: leave\n");
        scanf("%d",&insert);
        switch(insert)
        {
        case 1:
            sendmassage();
            break;
        case 2:
            refresh();
            break;
            case 3 :
                members();
                break;
            case 4 :
                leave();
                flag = 0;
        }
    }
    return;
}
int connectsocket()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
        exit(0);
    // Assign IP and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }

}
int flag=0;

void sendmassage()
{
    char buffer[MAX];
    char sending[1000];
    int n;
    memset(sending, 0, sizeof(sending));
    memset(buffer, 0, sizeof(buffer));
    printf("Enter your message:\n");
    getchar();
    scanf("%[^\n]s", buffer);
    sprintf(sending,"send %s, %s\n", buffer, AuthToken);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON * recived = cJSON_Parse(buffer);
    printf("From server: %s\n", cJSON_GetObjectItem(recived,"content")-> valuestring);
    //closesocket(client_socket);
    printf("YOUR MASSAGE SUCCESSFULY SENT ....");
    return;
}

void registerkardan()
{
    char username[MAX];
    char password[MAX];
    char sending[MAX];
    printf("Enter Username \n");
    scanf("%s",username);
    printf("Enter password \n");
    scanf("%s",password);
    memset(sending, 0, sizeof(sending));
    sprintf(sending, "register %s, %s\n", username, password);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(sending, 0, sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    cJSON * recived = cJSON_Parse(sending);
    if(strcmp("Error",(cJSON_GetObjectItem(recived,"type")-> valuestring)) == 0)
    {
        printf("Frome server :Oops %s\n",cJSON_GetObjectItem(recived,"content")-> valuestring);
    }
    /*else
    {
        sprintf(sending, "login %s, %s\n", username, password);
        connectsocket();
        send(client_socket, sending, sizeof(sending), 0);
        memset(sending, 0,sizeof(sending));
        recv(client_socket, sending, sizeof(sending), 0);
        //closesocket(client_socket);
        cJSON * recived = cJSON_Parse(sending);
        strcpy(AuthToken, cJSON_GetObjectItem(recived,"content")-> valuestring);
        flag = 1;
    }*/
    //closesocket(client_socket);
    return;
}
void loginkardan()
{
    char username[MAX];
    char password[MAX];
    char sending[MAX];
    printf("Enter Username \n");
    scanf("%s",username);
    printf("Enter password \n");
    scanf("%s",password);
    sprintf(sending, "login %s, %s\n", username, password);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(sending, 0,sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    printf("\n\n\n%s\n\n\n", sending);
    //closesocket(client_socket);
    cJSON * recived = cJSON_Parse(sending);
    if(strcmp("AuthToken", cJSON_GetObjectItem(recived,"type")-> valuestring) == 0 )
    {
        strcpy(AuthToken, cJSON_GetObjectItem(recived,"content")-> valuestring);
        flag = 1;
    }
    else
        printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"content")-> valuestring);
    //closesocket(client_socket);
    return;
}
void create()
{
    char sending[MAX];
    char channelname[MAX];
    printf("Enter Your Channel name\n");
    scanf("%s",channelname);
    sprintf(sending, "create channel %s, %s\n", channelname, AuthToken);
    connectsocket();
    send(client_socket, sending, strlen(sending), 0);
    memset(sending, 0,sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    //closesocket(client_socket);
    cJSON * recived = cJSON_Parse(sending);
    //closesocket(client_socket);
    if(strcmp("Successful", cJSON_GetObjectItem(recived,"type")-> valuestring) == 0 )
        inchannel();
    else printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"content")-> valuestring);
        return;
}
void join()
{
    char sending[MAX];
    char channelname[MAX];
    printf("Enter Channel name to join\n");
    scanf("%s",channelname);
    sprintf(sending, "join channel %s, %s\n", channelname, AuthToken);
    connectsocket();
    send(client_socket, sending, strlen(sending), 0);
    memset(sending, 0,sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    //closesocket(client_socket);
    cJSON * recived = cJSON_Parse(sending);
    printf("%s", sending);
    printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"content")-> valuestring);
    //closesocket(client_socket);
    if(strcmp("Successful", cJSON_GetObjectItem(recived,"type")-> valuestring) == 0 )
        inchannel();
        return;
}
void logout()
{
    char sending[MAX];
    sprintf(sending, "logout %s\n",AuthToken);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(sending, 0,sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    printf("\n\n\n heyyou : \n\t %s \n\n\n", sending);

    //closesocket(client_socket);
    cJSON * recived = cJSON_Parse(sending);
    printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"type")-> valuestring);
    //closesocket(client_socket);
    printf("HAVE FUN ....\n");
    flag2 = 0;
}

void changecolor(){
printf("\nENTER YOUR BACK GROUND COLOR\n\t 0 = Black\n\t 1 = Blue\n\t 2 = Green\n\t 3 = Aqua\n\t 4 = Red\n\t 5 = Purple\n\t 6 = Yellow\n\t 7 = White\n\t 8 = Gray\n\t 9 = Light Blue\n\t A = Light Green\n\t B = Light Aqua\n\t C = Light Red\n\t D = Light Purple\n\t E = Light Yellow\n\t F = Bright White\n");
char a[2];
scanf("%s", a);
printf("\nnow\t ENTER YOUR FOREGROUND COLOR\n\t 0 = Black\n\t 1 = Blue\n\t 2 = Green\n\t 3 = Aqua\n\t 4 = Red\n\t 5 = Purple\n\t 6 = Yellow\n\t 7 = White\n\t 8 = Gray\n\t 9 = Light Blue\n\t A = Light Green\n\t B = Light Aqua\n\t C = Light Red\n\t D = Light Purple\n\t E = Light Yellow\n\t F = Bright White\n");
char b[2];
scanf("%s", b);
char c[10];
sprintf(c, "COLOR %s%s", a, b);
system(c);
return;
}

int main()
{
    printf("HELLO THIS IS YOUR CHAT APPLICATION!!!!\n");
    while(1)
    {
        flag = 0;
        flag2 = 1;
        int insert;
        while(flag == 0)
        {
            printf("First Menu : \n");
            printf("1: Login \n2: Register\n3: exit\n4: change color\n");
            scanf("%d",&insert);

            switch (insert)
                {
            case 1:
                loginkardan();
                break;
            case 2:
                registerkardan();
                break;
            case 3:
                printf("GOOD BAY SEE YOU LATER.....\n\n\n");
                return 0;
            case 4:
                changecolor();
                break;
                }
        }
        while(flag2)
        {
            flag = 0;
            printf("Menu 2: \n1: Create Channel \n2: Join Channel \n3: Logout \n");
            scanf("%d",&insert);
            switch(insert)
            {
            case 1:
                create();
                break;
            case 2:
                join();
                break;
            case 3 :
                logout();
                break;
            }
        }
    }
    return 0;

}
