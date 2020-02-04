#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"
#include "cJSON.c"
#include "time.h"
#define MAX 80
#define PORT 12344
#define SA struct sockaddr
int server_socket, client_socket;
struct sockaddr_in server, client;
char buffer[MAX];
int channelnum;
typedef struct {
char name[100];
char channelname[100];
} mem;
mem online[100];

char *jsonmaker( char *avali, char *dovomi, char *first, char *second){
cJSON *ful;
ful = cJSON_CreateObject();

cJSON_AddItemToObject(ful, avali, cJSON_CreateString(first));
cJSON_AddItemToObject(ful, dovomi, cJSON_CreateString(second));

    char *out;
    out = cJSON_PrintUnformatted(ful);
    return out;
}


char *tokenmaker(){
    time_t t = time(NULL);
    srand((unsigned) time(&t));
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *randomString = NULL;

        randomString = malloc(sizeof(char) * (36));

        if (randomString) {
            for (int n = 0;n < 35;n++) {
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[35] = '\0';
        }

    return randomString;
}


void logout(){
    char name [100];
    char *tempe;
    sprintf(name, "resurses\\%s", buffer);
    strcat(name, ".txt");
    FILE *logout;
    logout = fopen(name, "r");
    if(logout){
        char *username;
        char *pass;
        fgets(tempe, 255, logout);
        cJSON *recived = cJSON_Parse(tempe);
        username = cJSON_GetObjectItem(recived, "name")->valuestring;
        pass = cJSON_GetObjectItem(recived, "pass")->valuestring;
        printf("\nname = %s\n", username);
        fclose(logout);
        char namee[100];
        strcpy(namee, username);
        memset(username, 0, strlen(username));
        sprintf(username, "resurses\\%s.txt",namee);
        printf("\nuser = %s\n namee = %s", username, namee);
        logout = fopen(username, "w");
        char naaa[100];
        sprintf(naaa,"\b%s", jsonmaker("password", "Token", pass, namee));
        printf("\n\n\nit should be ......%s\n\n", naaa);
        fprintf(logout,naaa);
        fclose(logout);
        char sending[100];
        memset(sending, 0, sizeof(sending));
        sprintf(sending, "%s", jsonmaker("type", "content", "Successful", ""));
        send(client_socket, sending, sizeof(sending), 0);
        remove(name);
        }
    else{
        char sending[100];
    memset(sending, 0, sizeof(sending));
    sprintf(sending, "%s", jsonmaker("type", "content", "Error", "Authentication failed!"));
    send(client_socket, sending, sizeof(sending), 0);
    }
}


createchannel(){
    char channelname[100];
    char Auth[100];
    sscanf(buffer, "%*s %s %s", channelname, Auth);
    char addres[100];
    sprintf(addres, "resurses\\%s.txt", Auth);
    FILE *he;
    he = fopen(addres, "r");
    char temp[255];
    char name[100];
    fgets(temp, 255, he);
    cJSON *recived = cJSON_Parse(temp);
    strcpy( name, cJSON_GetObjectItem(recived, "name")->valuestring);
    //strcat(name, "\n");
    fclose(he);
    char nameofchannel[100];
    sprintf(nameofchannel, "resurses\\%s.txt", channelname);
    if(he = fopen(nameofchannel, "r")){
        fclose(he);
        char sending[100];
        sprintf(sending, "%s\n", jsonmaker("type", "content", "Error", "This name isn't available"));
        send(client_socket, sending, sizeof(sending), 0);
    }
    else{
    he = fopen(nameofchannel, "w");
    printf("\n he = %s\n", he);
    cJSON *array = cJSON_CreateArray();
    cJSON *member = cJSON_CreateString(name);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToArray(array, member);
    cJSON_AddStringToObject(obj, "type", "list");
    cJSON_AddItemToObject(obj, "content", array);
    char *out;
    out = cJSON_PrintUnformatted(obj);
    fprintf(he, "%s", out);
    fclose(he);
    char sending[100];
    sprintf(sending, "%s\n", jsonmaker("type", "content", "Successful", ""));
    send(client_socket, sending, sizeof(sending), 0);
    strcpy(online[channelnum].name, name);
    strcpy(online[channelnum].channelname, nameofchannel);
    channelnum++;
    }
}


void login(){
    char name[MAX];
    char name2[MAX];
    char pass[MAX];
    char sending[2*MAX];
    char tempe[255];
    sscanf(buffer, "%s %s", name, pass);
    FILE *loginer;
    sprintf(name2, "resurses\\%s", name);
    strcat(name2, ".txt");
    printf("\n%s\n", name2);
    if(loginer = fopen(name2, "r")){
        fgets(tempe, 255, loginer);
        cJSON *recived = cJSON_Parse(tempe);
    if(strcmp(pass, cJSON_GetObjectItem(recived, "password")->valuestring)){
           sprintf(sending,"%s\n", jsonmaker("type", "content", "Error", "Wrong password."));
           send(client_socket, sending, sizeof(sending), 0);
           }
           else{
            if(strcmp(name, cJSON_GetObjectItem(recived, "token")->valuestring)){
                memset(sending, 0, sizeof(sending));
                char useris[30];
                sprintf(useris, "The user %s is already logged in.", name);
                sprintf(sending, "%s\n", jsonmaker("type", "content", "Error", useris));
                send(client_socket, sending, sizeof(sending), 0);
                fclose(loginer);
                return;
            }
            memset(sending, 0, sizeof(sending));
            fclose(loginer);
            loginer = fopen(name2, "w");
            char *token = tokenmaker();
            fprintf(loginer, "%s", jsonmaker("password", "Token", pass, token));
           sprintf(sending, "%s\n", jsonmaker("type", "content", "AuthToken", token));
           FILE *tokener;
           fclose(loginer);
           char tokene [100];
           sprintf(tokene, "resurses\\%s", token);
           strcat(tokene, ".txt");
           tokener= fopen(tokene,"w");
           fprintf(tokener, "%s", jsonmaker("name", "pass", name, pass));
            fclose(tokener);
            send(client_socket,sending, sizeof(sending), 0);

           }
    }
    else {
        memset(sending, 0, sizeof(sending));
           sprintf(sending, "%s\n", jsonmaker("type", "content", "Error", "Username is not valid."));
            send(client_socket,sending, sizeof(sending), 0);
    }
}

void registering(){
    char name[MAX];
    char name2[MAX];
    char pass[MAX];
    char sending[2*MAX];
    printf("\n\n\n%s",buffer);
    sscanf(buffer, "%s %s", name, pass);
    printf("\n\n\n pass =  %s\n\n\n",pass);
    printf("\n\n\n name =  %s\n\n\n",name);
    FILE *registerer ;
    sprintf(name2, "resurses\\%s", name);
    strcat(name2, ".txt");
    if (registerer = fopen(name2, "r")){
        fclose(registerer);
        memset(sending, 0, sizeof(sending));
        sprintf(sending,"%s\n",jsonmaker("type", "content", "Error", "this username is not available."));
        send(client_socket, sending, sizeof(sending), 0);
    }
    else{
    registerer = fopen(name2,"w");
    memset(sending, 0, sizeof(sending));
    sprintf(sending, "%s\n", jsonmaker("Token", "password", name, pass));
    fprintf(registerer, "%s", sending);
    memset(sending, 0, sizeof(sending));
    sprintf(sending,"%s\n",jsonmaker("type", "content", "Successful", ""));
    send(client_socket, sending, sizeof(sending), 0);
    fclose(registerer);
    }
    return;
}

leave(){

char Auth[100];
    sscanf(buffer, "%s", Auth);
    char addres[100];
    sprintf(addres, "resurses\\%s.txt", Auth);
    FILE *he;
    he = fopen(addres, "r");
    char temp[1000];
    char name[100];
    fgets(temp, 255, he);
    cJSON *recived = cJSON_Parse(temp);
    strcpy( name, cJSON_GetObjectItem(recived, "name")->valuestring);
    //strcat(name, "\n");
    char channel[100];
    fclose(he);
    for(int i = 0; i < channelnum; i++){
        if(!strcmp(name , online[i].name)){
            strcpy(channel, online[i].channelname);
            break;
        }
    }
    FILE *channele;
    channele = fopen(channel, "r");
    memset(temp, 0, sizeof(temp));
    fgets(temp, 1000, channele);
    char* token = strtok(temp, name);
    fclose(channele);
    channele = fopen(channel, "w");
    char *find;
    find = strstr(temp, name);
    char before[1000];
    char after[1000];
    strcpy(after, (find + sizeof(name)));
    *find = '\0';
    strcpy(before, temp);
    strcat(before, after);
    fprintf(channele, "%s", before);
    char sending[100];
    memset(sending, 0, sizeof(sending));
    sprintf(sending, jsonmaker("type", "content", "Successful", ""));
    send(client_socket, sending, sizeof(sending), 0);
    fclose(channele);
}

void connectoin(){


    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");
        }

int main()
{
    WORD wVersionRequested;
    int err;
    WSADATA wsaData;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");


    while(1){
        connectoin();
        char order[MAX];
        memset(buffer, 0, sizeof(buffer));
        memset(order, 0, sizeof(order));
        // Read the message from client and copy it to buffer
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("From client: %s\n", buffer);
        sscanf(buffer, "%s %[^\n]s", order, buffer);
        printf("From client: %s\n", buffer);
        printf("order is :\n\t%s\n", order);
        if(!strcmp(order, "login")){
            login();
            continue;
        }
        if(!strcmp(order, "register")){
            registering();
            continue;
        }
        if(!strcmp(order, "create")){
            createchannel();
            continue;
        }
        if(!strcmp(order, "logout")){
            logout();
            continue;
        }
        if(!strcmp(order, "leave")){
            leave();
            continue;
        }
        if(!strcmp(order, "")){
            break;
        }

    }
        // close the socket
        closesocket(server_socket);
    return 0;
}
