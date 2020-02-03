#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "cJSON.h"
#include "cJSON.c"
#include <winsock2.h>
#define MAX 1000
#define PORT 12345
#define SA struct sockaddr

void chat(int server_socket);
int socket_creator();
void menu();
void loginClient();
void registerClient();
void creatChannel();
void join_channel();
void logout();
void chatMenu();
void sendMessage();
void refresh();
void channelMembers();
void leave();
void search_in_messages();
void search_in_members();

struct sockaddr_in servaddr, cli;

int sock_number;

char token[MAX];
char content[MAX];
char type[MAX];
char buffer[3 * MAX];
char response[MAX];
char message[10000];
char order[50];

char username[MAX];
char password[MAX];

char channelName[MAX];

char textColor = 'f';
char backGroundColor = '0';

int socket_creator()
{
    int client_socket, server_socket;

	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        exit(0);
    }

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(client_socket, &servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else

    return client_socket;

	closesocket(client_socket);
}

void setting()
{
    system("cls");
    char order[20];
    int order2;

	printf("Setting:\n1: text color\n2: background color\n3: cancel\n\n");
	gets(order);
	system("cls");
	if ((strcmp(order, "3") && strcmp(order, "cancel")))
    {
        printf("Choose your favorite color:\n1:  Black\n2:  Blue\n3:  Green\n4:  Cyan\n5:  Red\n6:  Magenta\n7:  Brown\n8:  Light Gray\n9:  Dark Gray\n"
               "10: Light Blue\n11: Light Green\n12: Light Cyan\n13: Light Red\n14: Light Magenta\n15: Yellow\n16: White\n\n");
        scanf("%d%*c", &order2);
    }

	if (!strcmp(order, "1") || !strcmp(order, "text color"))
    {
        if(order2 < 11)
            textColor = (char)(order2 + 47);
        else if (order2 < 17)
            textColor = (char)(order2 + 86);
    }
    else if (!strcmp(order, "2") || !strcmp(order, "background color"))
    {
        if(order2 < 11)
            backGroundColor = (char)(order2 + 47);
        else if (order2 < 17)
            backGroundColor = (char)(order2 + 86);
    }
    else if (!strcmp(order, "3") || !strcmp(order, "cancel"));
    else
    {
        system("cls");
        setting();
    }

    char color[15];
    sprintf(color, "color %c%c", backGroundColor, textColor);

    system(color);
}

int main()
{
    char order[50] = {};
    memset(order, 0, 50);

	printf("Account Menu:\n1: Register\n2: Login\n\n0: Setting\n\n");
	gets(order);

	if (!strcmp(order, "1") || !strcmp(order, "register"))
        registerClient();
    else if (!strcmp(order, "2") || !strcmp(order, "login"))
        loginClient();
    else if (!strcmp(order, "0") || !strcmp(order, "Setting"))
        {
            setting();
            system("cls");
            main();
        }
    else
    {
        system("cls");
        main();
    }

	return 0;
}

void registerClient()
{
    system("cls");

    printf("Enter Username:\n");
    gets(username);
    printf("Enter password:\n");
    gets(password);

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "register %s, %s\n", username, password);
    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
    {
        system("cls");
        main();
    }
    else
    {
        system("cls");
        printf("%s\n\n", content);
        main();
    }
}
void loginClient()
{
    system("cls");

    printf("Enter Username:\n");
    gets(username);

    printf("Enter password:\n");
    gets(password);

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "login %s, %s\n", username, password);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(token, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "AuthToken"))
    {
        system("cls");
        menu();
    }
    else
    {
        system("cls");
        printf("%s\n\n", token);
        main();
    }
}

void menu()
{
    char order[50] = {};
    memset(order, 0, 50);

    printf("1: Create Channel\n2: Join Channel\n3: Logout\n\n0: Setting\n\n");

    gets(order);

    if (!strcmp(order, "1") || !strcmp(order, "Create Channel"))
        creatChannel();
    else if (!strcmp(order, "2") || !strcmp(order, "Join Channel"))
        join_channel();
    else if (!strcmp(order, "3") || !strcmp(order, "Logout"))
        logout();
    else if (!strcmp(order, "0") || !strcmp(order, "Setting"))
        {
            setting();
            system("cls");
            menu();
        }

    else
    {
        system("cls");
        menu();
    }
}
void creatChannel()
{
    system("cls");

    printf("Enter Your Channel Name:\n");
    gets(channelName);

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "create channel %s, %s\n", channelName, token);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
        chatMenu();
    else
    {
        printf("%s\n\n", content);
        menu();
    }
    system("cls");
}
void join_channel()
{
    system("cls");

    printf("Enter Your Channel Name:\n");
    gets(channelName);

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "join channel %s, %s\n", channelName, token);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
    {
        system("cls");
        chatMenu();
    }

    else
    {
        printf("%s\n\n", content);
        menu();
    }

}
void logout()
{
    system("cls");

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "logout %s\n", token);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
        main();
    else
    {
        printf("%s\n\n", content);
        main();
    }
}
void chatMenu()
{
    memset(order, 0, 50);

    printf("Welcom To Channel %s!\n", channelName);

    printf("1: Send Message\n2: Refresh\n3: Channel Members\n4: Leave Channel\n5: search in messages\n6: search in channel members\n\n0: Setting\n\n");

    gets(order);

    if (!strcmp(order, "1") || !strcmp(order, "Send Message"))
        sendMessage();
    else if (!strcmp(order, "2") || !strcmp(order, "Refresh"))
        refresh();
    else if (!strcmp(order, "3") || !strcmp(order, "Channel Members"))
        channelMembers();
    else if (!strcmp(order, "4") || !strcmp(order, "Leave Channel"))
        leave();
    else if (!strcmp(order, "5") || !strcmp(order, "search in messages"))
        search_in_messages();
     else if (!strcmp(order, "6") || !strcmp(order, "search in channel members"))
        search_in_members();
    else if (!strcmp(order, "0") || !strcmp(order, "Setting"))
        {
            setting();
            system("cls");
            chatMenu();
        }
    else
    {
        system("cls");
        chatMenu();
    }
}
void sendMessage()
{
    gets(message);
    system("cls");

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "send %s, %s\n", message, token);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
        chatMenu();
    else
    {
        printf("%s\n\n", content);
        chatMenu();
    }
}
void refresh()
{
    system("cls");

    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "refresh %s\n", token);

    sock_number = socket_creator();

    send(sock_number, buffer, 1000, 0);
    recv(sock_number, response, 1000, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    cJSON* item = cJSON_GetObjectItem(translated,"content");

    if(!strcmp(type, "list") || !strcmp(type, "List"))
    {
        printf("\n");
        int size = cJSON_GetArraySize(item);

        for(int i = 0; i < size; ++i)
        {
            cJSON* pm = cJSON_GetArrayItem(item,i);
            cJSON* sender = cJSON_GetObjectItem(pm,"sender");
            cJSON* content = cJSON_GetObjectItem(pm,"content");
            printf("%s: %s\n",sender->valuestring,content->valuestring);
        }
        printf("\n");
        chatMenu();
    }
    else
    {
        printf("%s\n\n", content);
        chatMenu();
    }
}
void channelMembers()
{
    system("cls");
    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "channel members %s\n", token);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);


    if(!strcmp(type, "Successful"))
    {
        printf("%s\n\n", cJSON_Print( cJSON_GetObjectItem(translated, "content")));
        chatMenu();
    }
    else
    {
        printf("%s\n\n", content);
        chatMenu();
    }
}
void leave()
{
    memset(buffer, 0, 3 * MAX);
    sprintf(buffer, "leave %s\n", token);

    sock_number = socket_creator();

    send(sock_number, buffer, 3 * MAX, 0);
    recv(sock_number, response, MAX, 0);
    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
    {
        system("cls");
        menu();
    }
    else
    {
        printf("%s\n\n", content);
        system("cls");
        menu();
    }
}
void search_in_messages()
{
    memset(buffer, 0, 3 * MAX);
    memset(message, 0, 10000);
    printf("Enter the word which you want to find messages containing it:\n");
    gets(buffer);
    sscanf(buffer, "%s%*[^\n]\n", message);

    sprintf(buffer, "search message %s, %s\n", message, token);
    sock_number = socket_creator();

    send(sock_number, buffer, MAX, 0);
    recv(sock_number, response, MAX, 0);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
    {
        system("cls");
        printf("%s\n\n", content);
        chatMenu();
    }
    else
    {
        system("cls");
        printf("%s\n\n", content);
        chatMenu();
    }
}

void search_in_members()
{
    memset(buffer, 0, MAX);

    printf("Enter the username which you want to search:\n");
    gets(username);

    sprintf(buffer, "search username %s, %s\n", username, token);
    sock_number = socket_creator();

    send(sock_number, buffer, MAX, 0);
    recv(sock_number, response, MAX, 0);

    //printf("#: %s", response);

    cJSON *translated = cJSON_Parse(response);
    strcpy(content, (cJSON_GetObjectItem(translated, "content"))->valuestring);
    strcpy(type, (cJSON_GetObjectItem(translated, "type"))->valuestring);

    if(!strcmp(type, "Successful"))
    {
        system("cls");
        printf("%s\n\n", content);
        chatMenu();
    }
    else
    {
        system("cls");
        printf("%s\n\n", content);
        chatMenu();
    }
}
