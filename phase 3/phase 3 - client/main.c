#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winsock2.h>
#define MAX 1000
#define PORT 12345
#define SA struct sockaddr


void myGetObjectItem(char* response, char* part1, char* part1str, char* part2, char* part2str);
/*
این تابع پنج ورودی میگیرد که ورودی اول رشته ای به صورت جیسون است که از سرور گرفته است، ورودی دوم و چهارم آیتمی از حیسون است که
valueString
مربوط به آنها را به ترتیب در رشته های سوم و پنجم تابع ذخیره میکند.
استفاده از این تابع همانند استفاده دوبار از تابع
cJSON_GetObjectItem
می باشد.
خروجی ندارد
*/


void myGetArrayItems(char* response, char* part1, char* part1str, char* part2, char* part2str);
/*
این تابع پنج ورودی میگیرد که ورودی اول رشته ای به صورت جیسون است که از سرور گرفته است، ورودی دوم و چهارم آیتمی از حیسون است که
valueString
مربوط به آنها را به ترتیب در رشته های سوم و پنجم تابع ذخیره میکند.
تفاوت این تابع و تابع قبل در این است که ورودی پنحم یک آرایه به صورت جیسون است.
استفاده از این تابع همانند دو بار استفاده  از تابع
cJSON_GetObjectItem
می باشد.
خروجی ندارد
*/


void myPrintArrayItems(char* response);
/*
این تابع یک رشته به صورت جیسون را میگیرد و اعضای آیتم دوم آن که یک آرایه ای از جیسون ها است را به صورت
sender: content
پرینت می کند. از این تابع در رفرش و برای پرینت پیام های جدید استفاده می شود
خروجی ندارد.
*/


void setting();
/*
این تابع هنگامی که کاربر قصد تغییر تنظیمات چت اپلیکیشن خود را داشته باشد صدا زده میشود.
این تابع ۲ گزینه شامل رنگ نوشته ها و رنگ پس زمینه دارد که هر کدام از انها شامل ۱۶ رنگ می باشند
*/


int socket_creator();
/*
این تابع برای اتصال به سرور، سوکت را می سازد
*/


void menu();
/*
بعد از لاگین شدن این تابع صدا زده میشود و امکان ایجاد کانال، عضو شدن در دیگر کانال ها، و لاگ‌اوت را به ما میدهد
*/


void loginClient();
/*
اگر کاربر درخواست لاگین داشته باشد، این تابع صدا زده می شود و یوزرنیم و پسورد کاربر را میگیرد و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا کاربر را به منو منتقل کند.
*/


void registerClient();
/*
اگر کاربر درخواست ثبت نام داشته باشد، این تابع صدا زده می شود و یوزرنیم و پسورد مورد نظر کاربر را میگیرد و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا کاربر را به تابع
main
برای لاگین منتقل کند.
*/


void creatChannel();
/*
اگر کاربر درخواست ایجاد کانال داشته باشد، این تابع صدا زده می شود و اسم مورد نظر کاربر را برای کانال میگیرد و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا کاربر را به منوی چت در کانال منتقل کند.
*/


void join_channel();
/*
اگر کاربر درخواست عضو کانال موجود را داشته باشد، این تابع صدا زده می شود و اسم کانال میگیرد و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا کاربر را به منوی چت در کانال منتقل کند.
*/


void logout();
/*
اگر کاربر درخواست خارج شدن از حساب خود را داشته باشد، این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا کاربر را به تابع
main
برگرداند.
*/


void chatMenu();
/*
بعد از ساختن کانال یا عضو کانال شدن، این تابع صدا زده می شود و به کاربر امکان ارسال پیام در گروه، دیدن پیام های جدید، دیدن اعضای کانال، جستجوی کلمه ای در پیام ها، جستجوی کابری با
یوزر نیم او برای اینکه بداند در کانال حضور دارد یا خیر، و امکان ترک کردن کانال و بازگشت به منو را میدهد.
*/


void sendMessage();
/*
اگر کاربر عضو کانالی باشد، و درخواست ارسال پیام در کانال را داسته باشد،  این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا خیر.
*/


void refresh();
/*
اگر کاربر عضو کانالی باشد، و درخواست دیدن پیام های ناخوانده در کانال را داسته باشد،  این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا پیام های جدید را به کاربر نشان بدهد(آنها را از سرور دریافت کرده است!).
*/


void channelMembers();
/*
اگر کاربر عضو کانالی باشد، و درخواست دیدن اعضای کانال را داسته باشد،  این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا اعضای کانال را به کاربر نشان بدهد(آنها را از سرور دریافت کرده است!).
*/


void leave();
/*
اگر کاربر عضو کانالی باشد، و درخواست ترک کانال را داسته باشد،  این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا کانال را ترک کند و به
menu
برگردد.
*/


void search_in_messages();
/*
اگر کاربر عضو کانالی باشد، و درخواست جستجو برای کلمه ای خاص در پیام های کانال را داسته باشد،  این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا پیام های حاوی کلمه مورد نظر را به کاربر نشان بدهد.
*/


void search_in_members();
/*
اگر کاربر عضو کانالی باشد، و درخواست جستجو برای کاربری خاص در اعضای کانال را داسته باشد،  این تابع صدا زده می شود و این درخواست را به سرور می فرستد و بر اساس پاسخ سرور تصمیم گرفته می شود
که خطا نمایش داده شود و یا اینکه بگوید که کاربر مورد نظر عضو کانال هست یا نه..
*/



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

void myGetObjectItem(char* response, char* part1, char* part1str, char* part2, char* part2str)
{
    char first[15], firstStr[100];
    char second[15], secondStr[1000];

    sscanf(response, "{\"%[^\"]\":\"%[^\"]\",\"%[^\"]\":\"%[^\"]\"}", first, firstStr, second, secondStr);

    if(!strcmp(first, part1))
        strcpy(part1str, firstStr);
    if(!strcmp(second, part2))
        strcpy(part2str, secondStr);
}

void myGetArrayItems(char* response, char* part1, char* part1str, char* part2, char* part2str)
{
    char first[15], firstStr[100];
    char second[15], secondStr[1000];

    sscanf(response, "{\"%[^\"]\":\"%[^\"]\",\"%[^\"]\":%[^}]}", first, firstStr, second, secondStr);

    if(!strcmp(first, part1))
        strcpy(part1str, firstStr);
    if(!strcmp(second, part2))
        strcpy(part2str, secondStr);
}

void myPrintArrayItems(char* response)
{
    char one[1000];
    char temp[1000];
    char oneSender[1000];
    char oneContent[1000];
    char *onePtr = response + 1;
    char *tempo;

    puts("");
    tempo = strchr(onePtr, '{');
    onePtr = tempo;

    for( ;onePtr; )
    {
        sscanf(onePtr, "{%[^}]}%*[^\0]", temp);
        sprintf(one, "{%s}", temp);

        sscanf(onePtr, "{\n\t\t\t\"%*[^\"]\":\t\"%[^\"]\",\n\t\t\t\"%*[^\"]\":\t\"%[^\"]\n\t\t}", oneSender, oneContent);
        printf("%s: %s\n", oneSender, oneContent);
        onePtr += 1;
        tempo = strchr(onePtr, '{');
        onePtr = tempo;
    }
    puts("");
}

int socket_creator()
{
    int client_socket;

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

    myGetObjectItem(response, "type", type, "content", content);

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

    myGetObjectItem(response, "type", type, "content", content);

    if(!strcmp(type, "AuthToken"))
    {
        strcpy(token, content);
        system("cls");
        menu();
    }
    else
    {
        system("cls");
        printf("%s\n\n", content);
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

    myGetObjectItem(response, "type", type, "content", content);

    if(!strcmp(type, "Successful"))
        chatMenu();
    else
    {
        system("cls");
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

    myGetObjectItem(response, "type", type, "content", content);

    if(!strcmp(type, "Successful"))
    {
        system("cls");
        chatMenu();
    }

    else
    {
        system("cls");
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

    myGetObjectItem(response, "type", type, "content", content);

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

    myGetObjectItem(response, "type", type, "content", content);

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
    myGetObjectItem(response, "type", type, "content", content);

    if(!strcmp(type, "Error"))
    {
        printf("%s\n\n", content);
        chatMenu();
    }
    else
    {
        myPrintArrayItems(response);
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

    myGetArrayItems(response, "type", type, "content", content);

    if(!strcmp(type, "list") ||!strcmp(type, "List"))
    {
        printf("%s\n\n", content);
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

    myGetObjectItem(response, "type", type, "content", content);

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

    myGetObjectItem(response, "type", type, "content", content);

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

    myGetObjectItem(response, "type", type, "content", content);

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
