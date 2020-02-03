#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <dirent.h>
#include <time.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 1000
#define PORT 12345
#define SA struct sockaddr

struct user
{
    char username[MAX];
    char token[25];
    char channelName[MAX];
    int line;
} onlineUsers[1000];

struct channel
{
    char channelName[100];
    char members[100][100];
    int memberCount;
} activeChannels[100];

int server_socket, client_socket;
struct sockaddr_in server, client;
char prompt[MAX];
char token[25];
char first[MAX];
char second[MAX];
char name[MAX];
char username[MAX];
char password[MAX];
char channelName[MAX];
char address[MAX];
char pm[10 * MAX];
int onlineCount = 0;
int activeCount = 0;
FILE* fileptr;

void listening();
void reggister();
void channel_members();
void login();
void create_channel();
void join_channel();
void logout();
void send_message();
void refresh();
void leave();
void channel_members();
void search_members();
void search_messages();

void myaccept()
{
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

int findChannel(char key[MAX])
{
    char name[25];
    struct dirent *de;
    DIR *dr = opendir("Resources\\Channels");

    while ((de = readdir(dr)) != NULL){

        strcpy(name, de->d_name);

        if(!strcmp(key, name)){
            return 1;
        }
    }

    closedir(dr);
    return 0;
}

int findOnlineUser(const char keyToken[25], char Name[100])
{
    for(int i = 0; i < onlineCount; ++i)
    {
        if (!strncmp(keyToken,(onlineUsers[i].token), 25))
        {
            strcpy(Name, onlineUsers[i].username);
            return i;
        }
    }
    return -1;
}

int findActiveChannel(char nameOfChannel[100])
{
    for(int i = 0; i < activeCount; ++i)
    {
        if(!strcmp(nameOfChannel, activeChannels[i].channelName))
            return i;
    }

    return -1;
}

void createToken()
{
    srand(time(NULL));

    for(int i = 0; i < 25; ++i)
    {
        int a = rand()%62;

        if (a < 10)
            token[i] = (char)(a + 48);

        else if (a < 36)
            token[i] = (char)(a + 55);
        else
            token[i] = (char)(a + 61);
    }
}

// Driver function
void create_server_socket()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        exit(0);
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

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
}

int main()
{
    mkdir("Resources");
    mkdir("Resources\\Users");
    mkdir("Resources\\Channels");

    create_server_socket();

    listening();
}

void listening()
{
    while(1)
    {
        memset(prompt, 0, MAX * sizeof(char));
        memset(token, 0, 25 * sizeof(char));
        memset(first, 0, MAX * sizeof(char));
        memset(name, 0, MAX * sizeof(char));
        memset(username, 0, MAX * sizeof(char));
        memset(password, 0, MAX * sizeof(char));
        memset(channelName, 0, MAX * sizeof(char));
        memset(address, 0, MAX * sizeof(char));
        memset(pm, 0, 10 * MAX * sizeof(char));

        myaccept();

        recv(client_socket, prompt, MAX, 0);

        sscanf(prompt, "%s %s %*s", first, second);

        if (!strcmp(first, "register"))
            reggister();
        else if (!strcmp(first, "login"))
            login();
        else if (!strcmp(first, "create"))
            create_channel();
        else if (!strcmp(first, "join"))
            join_channel();
        else if (!strcmp(first, "logout"))
            logout();
        else if (!strcmp(first, "send"))
            send_message();
        else if (!strcmp(first, "refresh"))
            refresh();
        else if (!strcmp(first, "leave"))
            leave();
        else if (!strcmp(first, "channel"))
            channel_members();
        else if (!strcmp(first, "search"))
        {
            if (!strcmp(second, "message"))
                search_messages();
            else if (!strcmp(second, "username"))
                search_members();
        }

        closesocket(client_socket);
    }
}
void reggister()
{
    sscanf(prompt, "%*s %[^,], %[^\n]\n", username, password);

    sprintf(address,"%s%s%s" ,"Resources\\Users\\", username, ".txt");

    cJSON* response = cJSON_CreateObject();

    if ((fileptr = fopen(address, "r")) != NULL)
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("This username already exists!"));
        fclose(fileptr);
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));

        fileptr = fopen(address, "w");

        fprintf(fileptr, "%s, %s", username, password);
        fclose(fileptr);
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);

    printf("%s\n", cJSON_PrintUnformatted(response));
}

void login()
{
    sscanf(prompt, "%*s %[^,], %[^\n]\n", username, password);

    sprintf(address,"%s%s%s" ,"Resources\\Users\\", username, ".txt");

    cJSON* response = cJSON_CreateObject();
    char saveduser[MAX], savedpass[MAX];

    if ((fileptr = fopen(address, "r")) == NULL)
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("Username could not be found!"));
    }
    else
    {
        fscanf(fileptr, "%[^,], %[^\n]\n", saveduser, savedpass);

        if (strcmp(password, savedpass))
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("Password is wrong!"));
        }
        else
        {
            createToken();

            cJSON_AddItemToObject(response, "type", cJSON_CreateString("AuthToken"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString(token));

            strcpy(onlineUsers[onlineCount].username, username);
            strcpy(onlineUsers[onlineCount].token, token);

            onlineCount++;
        }
        fclose(fileptr);
    }
    strcpy(first, cJSON_PrintUnformatted(response));
    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void create_channel()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", channelName, token);

    sprintf(address, "%s%s%s", "Resources\\Channels\\", channelName, ".txt");
    sprintf(first, "%s%s", channelName, ".txt");
    cJSON* response = cJSON_CreateObject();
    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if (findChannel(first))
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This channel name already exists!"));
        }
        //when there is no problem!:
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));

            //creating a file for channel
            fileptr = fopen(address, "w");

            cJSON* nameOfChannel = cJSON_CreateObject();
            cJSON_AddItemToObject(nameOfChannel, "name", cJSON_CreateString(channelName));
            fprintf(fileptr, "%s\n", cJSON_PrintUnformatted(nameOfChannel));

            cJSON* creator = cJSON_CreateObject();
            cJSON_AddItemToObject(creator, "sender", cJSON_CreateString("server"));
            sprintf(first, "%s created the channel.", name);
            cJSON_AddItemToObject(creator, "content", cJSON_CreateString(first));
            fprintf(fileptr, "%s\n", cJSON_PrintUnformatted(creator));
            fclose(fileptr);

            //adding creator to channel & adding channel to active ones
            strcpy(activeChannels[activeCount].channelName, channelName);
            strcpy(activeChannels[activeCount].members[activeChannels[activeCount].memberCount], name);
            strcpy(onlineUsers[i].channelName, channelName);

            activeChannels[activeCount].memberCount++;
            activeCount++;
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }
    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void join_channel()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", channelName, token);
    sprintf(first, "%s%s", channelName, ".txt");
    sprintf(address, "%s%s%s", "Resources\\Channels\\", channelName, ".txt");

    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if (!findChannel(first))
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such channel!"));
        }
        //when there is no problem!:
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));

            //adding to channel
            int l = findActiveChannel(channelName);
            int j = l;
            if(l == -1)
                j = activeCount;

            strcpy(activeChannels[j].channelName, channelName);
            strcpy(activeChannels[j].members[activeChannels[j].memberCount], name);
            strcpy(onlineUsers[i].channelName, channelName);
            activeChannels[j].memberCount++;

            if (l == -1)
                activeCount++;

            fileptr = fopen(address, "a");
            cJSON* joiner = cJSON_CreateObject();
            cJSON_AddItemToObject(joiner, "sender", cJSON_CreateString("server"));
            sprintf(first, "%s joined the channel.", name);
            cJSON_AddItemToObject(joiner, "content", cJSON_CreateString(first));
            fprintf(fileptr, "%s\n", cJSON_PrintUnformatted(joiner));
            fclose(fileptr);
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }
    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void logout()
{
    sscanf(prompt, "%*s %s", token);

    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);
    if (i != -1)
    {
        strcpy(onlineUsers[i].token, "");
        strcpy(onlineUsers[i].username, "");
        strcpy(onlineUsers[i].channelName, "");

        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}


void send_message()
{
    sscanf(prompt, "%*s %[^,], %[^\n]\n", pm, token);
    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);

    sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is n0 problem:
        {
            fileptr = fopen(address, "a");
            cJSON* message = cJSON_CreateObject();
            cJSON_AddItemToObject(message, "sender", cJSON_CreateString(name));
            cJSON_AddItemToObject(message, "content", cJSON_CreateString(pm));
            fprintf(fileptr, "%s\n", cJSON_PrintUnformatted(message));

            fclose(fileptr);

            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
        }
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This user is not member of any channel!"));
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void refresh()
{
     sscanf(prompt, "%*s %[^\n]\n", token);
    int i = findOnlineUser(token, name);
    cJSON *response = cJSON_CreateObject();

    sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

    if (i != -1) {
        if (strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            int j;
            int readline = onlineUsers[i].line;
            fileptr = fopen(address, "r");
            cJSON *content = cJSON_CreateArray();

            cJSON_AddItemToObject(response, "type", cJSON_CreateString("List"));

            for (j = 0; j < readline + 1; j++) {
                fgets(pm, 10 * MAX, fileptr);
            } //passing lines which are already read.
            for (j = readline; fgets(pm, 10 * MAX, fileptr); j++) {
                cJSON *message = cJSON_Parse(pm);
                cJSON *item = cJSON_CreateObject();
                cJSON *test = cJSON_GetObjectItem(message, "sender");
                cJSON_AddItemToObject(item, "sender", test);
                cJSON_AddItemToArray(content, item);
            }

            cJSON_AddItemToObject(response, "content", content);

            onlineUsers[i].line = j;

            fclose(fileptr);
        } else {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This user is not member of any channel!"));
        }
    } else {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }
    send(client_socket, cJSON_Print(response), MAX, 0);
     printf("%s\n", cJSON_PrintUnformatted(response));
}

void leave()
{
    sscanf(prompt, "%*s %s", token);
    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            int x = findActiveChannel(onlineUsers[i].channelName);

            for(int j = 0; j < activeChannels[x].memberCount; ++j)
                if(!strcmp(onlineUsers[i].channelName, activeChannels[x].members[j]))
                {
                    strcpy(activeChannels[x].members[j], "");
                    break;
                }
            sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

            fileptr = fopen(address, "a");

            cJSON* left = cJSON_CreateObject();
            cJSON_AddItemToObject(left, "sender", cJSON_CreateString("server"));
            sprintf(first, "%s left the channel.", name);
            cJSON_AddItemToObject(left, "content", cJSON_CreateString(first));
            fprintf(fileptr, "%s\n", cJSON_PrintUnformatted(left));

            fclose(fileptr);

            strcpy(onlineUsers[i].channelName, "");

            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString(""));
        }
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This user is not member of any channel!"));
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void channel_members()
{
    sscanf(prompt, "%*s %*s %s", token);
    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);
    int x = findActiveChannel(onlineUsers[i].channelName);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            cJSON* content = cJSON_CreateArray();

            cJSON_AddItemToObject(response, "type", cJSON_CreateString("List"));
            for (int j = 0; j < activeChannels[x].memberCount; ++j)
                if(strcmp(activeChannels[x].members[j], ""))
                    cJSON_AddItemToArray(content, cJSON_CreateString(activeChannels[x].members[j]));

            cJSON_AddItemToObject(response, "content", content);
        }
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This user is not member of any channel!"));
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void search_members()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", username, token);
    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            int flag = 1;
            int x = findActiveChannel(onlineUsers[i].channelName);
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));

            for(int j = 0; j < activeChannels[x].memberCount; ++j)
                if(!strcmp(username, activeChannels[x].members[j]))
                {
                    flag = 0;
                    sprintf(first, "%s IS member of your channel.", username);
                    cJSON_AddItemToObject(response, "content", cJSON_CreateString(first));
                    break;
                }

            if (flag)
            {
                sprintf(first, "%s IS NOT member of your channel.", username);
                cJSON_AddItemToObject(response, "content", cJSON_CreateString(first));
            }
        }
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This user is not member of any channel!"));
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

void search_messages()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", first, token);
    cJSON* response = cJSON_CreateObject();

    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            char foundMessages[10 * MAX] = {};
            char* ptr;
            int counter = 0;

            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Successful"));

            sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

            fileptr = fopen(address, "r");
            fgets(pm, 10 * MAX, fileptr);

            for ( ; fgets(pm, 10 * MAX, fileptr); )
            {
                cJSON *message = cJSON_Parse(pm);
                cJSON *sender = cJSON_GetObjectItem(message, "sender");
                cJSON *content = cJSON_GetObjectItem(message, "content");

                strcpy(pm, content->valuestring);
                strcpy(username, sender->valuestring);

                ptr = pm;
                for(; strlen(ptr); )
                {
                    sscanf(ptr, "%s", name);
                    if (!strcmp(name, first))
                    {
                        strcat(foundMessages, username);
                        strcat(foundMessages, ": ");
                        strcat(foundMessages, pm);
                        strcat(foundMessages, "\n");
                        counter++;
                        break;
                    }

                    ptr += strlen(name);
                    ptr += 1;
                }
            }
            fclose(fileptr);

            if (counter == 0)
                cJSON_AddItemToObject(response, "content", cJSON_CreateString("No message contains this word!"));
            else
                cJSON_AddItemToObject(response, "content", cJSON_CreateString(foundMessages));
        }
        else
        {
            cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response, "content", cJSON_CreateString("This user is not member of any channel!"));
        }
    }
    else
    {
        cJSON_AddItemToObject(response, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response, "content", cJSON_CreateString("There is no such a AuthToken!"));
    }

    send(client_socket, cJSON_PrintUnformatted(response), MAX, 0);
    printf("%s\n", cJSON_PrintUnformatted(response));
}

