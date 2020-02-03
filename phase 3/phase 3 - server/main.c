#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <dirent.h>
#include <time.h>
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

void myAddItemtoObject(char* response, char* part1, char* part1str, char* part2, char* part2str)
/*
این تابع پنج ورودی استرینگ میگیرد که ورودی اول رشته ای به صورت جیسون است و فرایند اضافه کردن آیتم ها روی آن انجام می شود
و رشته ای به صورت جیسون با دو آیتم با نام هایی که در ورودی دوم و چهارم گرفته است و با
valuString
هایی که به ترتیب در ورودی های سوم و پنجم گرفته است خواهد ساخت و در آرگومان اول ذخیره می کند.
استفاده از این تابع همانند استفاده دوبار از تابع
cJSON_AddItemtoObject
می باشد.
خروجی ندارد
*/


void myCreateArray(char* array);
/*
این تابع یک رشته میگیرد که آن را برای ساخت آرایه ای به صورت جیسون شامل یک آرایه از استرینگ ها آماده می کند.
*/


void myAddItemtoArray(char* array, char* item);
/*
این تابع دو ورودی استرینگ میگیرد که ورودی اول آرایه ای به صورت جیسون است، ورودی دوم آیتمی از حیسون است که
که آن را به آرایه
array
که از قبل با تابع
myCreateArray
ساخته شده است اضافه میکند. استفاده از این تابع همانند استفاده از
cJSON_AddItemToArray
می باشد.
خروجی ندارد
*/


void myArrayCreated(char* array);
/*
این تابع به فرایند ساخت آرایه
array
خاتمه می دهد
*/


void myAddArraytoObject(char* response, char* part1, char* part1str, char* part2, char* part2str);
/*
این تابع پنج ورودی استرینگ میگیرد که ورودی اول رشته ای به صورت جیسون است و فرایند اضافه کردن آیتم ها روی آن انجام می شود
و رشته ای به صورت جیسون با آیتمی با نامی که در ورودی دوم گرفته است و با
valuString
که در ورودی سوم گرفته است و آیتمی با نامی که در آرگومان چهارم گرفته است و رشته ای به صورت آرایه جیسون که در ورودی پنجم گرفته است می سازد.
تفاوت این تابع و
myAddItemtoObject
در این است که آخرین آرگومان این تابع، رشته ای است که به صورت ارایه ای از جیسون است.
استفاده از این تابع همانند استفاده دوبار از تابع
cJSON_AddItemtoObject
می باشد.
خروجی ندارد
*/


void myGetItem(char* response, char* part1, char* part1str, char* part2, char* part2str);
/*
این تابع پنج ورودی استرینگ میگیرد که ورودی اول رشته ای به صورت جیسون است که از سرور گرفته است، ورودی دوم و چهارم آیتمی از حیسون است که
valueString
مربوط به آنها را به ترتیب در رشته های سوم و پنجم تابع ذخیره میکند.
استفاده از این تابع همانند استفاده دوبار از تابع
cJSON_GetObjectItem
می باشد.
خروجی ندارد
*/


int findChannel(char key[MAX]);
/*
این تابع اسم یک کانال را میگیرد و در کانال هایی که در پوشه
Resources\Channels
ذخیره شده است، جستجو می کند و اگر کانالی با این اسم پیدا کرد، ۱ و در غیر این صورت ۰ را خروجی می دهد
*/


int findOnlineUser(const char keyToken[25], char Name[100]);
/*
این تابع یک توکن را میگیرد و در بین افراد آنلاین جستجو میکند (زیرا توکن صرفا مخصوص افراد آنلاین است)
و اگر کاربر آنلاینی را با این توکن پیدا کرد، اسم او را در آرگومان دوم ذخیره میکند و یک خروجی میدهد و اگر چنین کاربری یافت نشد، ۰ خروجی می دهد.
*/


int findActiveChannel(char nameOfChannel[100]);
/*
این تابع اسم یک کانال را میگیرد و در کانال های فعال (کانال هایی که حداقل یک عضو دارند) ،
 جستجو می کند و اگر کانالی با این اسم پیدا کرد، ۱ و در غیر این صورت ۰ را خروجی می دهد
*/


void createToken();
/*
این تابع یک رشته شامل ۲۵ کاراکتر از کاراکتر های حروف انگلیسی بزرگ و کوچک و اعداد می سازد و آن را در متغیر گلوبالی به اسم توکن ذخیره می کند.
*/


void create_server_socket();
/*
این تابع برای اتصال کلاینت ها به سرور، سوکت را می سازد
*/


void myaccept();
/*
این تابع پس از درخواست کلاینت ها برای اتصال به سوکت ساخته شده توسط سرور، درخواست انها را قبول میکند. با توجه به اینکه پس از انجام هر درخواست کلاینت ها، این تابع در سرور اجرا می شود،
پس سرور می تواند به صورت
state less
تعداد زیادی کلاینت را هندل کند
*/


void listening();
/*
این تابع ابتدا درخواست کلاینت را دریافت میکند و با توجه به درخواست، بررسی درخواست را به تابع های دیگر محول میکند
*/


void reggister();
/*
اگر درخواست کلاینت ثبت نام کاربر جدید باشد، تابع
listening
بررسی درخواست را به این تابع محول می کند. این تابع دردست بودن نام کاربری را بررسی می کند و اگر در دسترس بود، کاربر را ثبت نام می کند و فولدری برای این کاربر در
Resources\Users
شامل یوزر نیم و پسورد کاربر می سازد و پیام موفقیت را به کلاینت ارسال میکند و در غیر این صورت خطای مناسب را به سرور ارسال می کند.
*/


void login();
/*
اگر درخواست کلاینت لاگین کاربر باشد، تابع
listening
بررسی درخواست را به این تابع محول می کند.این تابع درستی یوزرنیم و پسورد را بررسی می کند و اگر مشکلی نبود، توکن ایجاد کرده و آن را به کلاینت می دهد و در غیر این صورت خطای لازم را ارسال میکند.
*/


void create_channel();
/*
اگر درخواست کلاینت ایجاد کانال جدید باشد، این تابع اجرا می شود.
اگر اسم کانال مورد نظر کاربر تکراری بود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
فایلی شامل اسم کانال و اولین پیام که
... created the channel
در مسیر
Resources\Channels
ایجاد می کند.
و اسم کانال کاربر به اسم کانال تغییر میکند و اسم کاربر به اسم اعضای کانال اضافه می شود
و پیام موفقیت را به کلاینت ارسال میکند
*/


void join_channel();
/*
اگر درخواست کلاینت عضو شدن به کانال های موجود باشد، این تابع اجرا می شود.
اگر کانالی با اسم مورد نظر کاربر وجود نداشت بود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
در فایل مربوط به کانال، پیام
... joined the channel
در مسیر
Resources\Channels
ایجاد می کند.
و اسم کانال کاربر به اسم کانال تغییر میکند و اسم کاربر به اسم اعضای کانال اضافه می شود
و پیام موفقیت را به کلاینت ارسال میکند
*/


void logout();
/*
اگر درخواست کلاینت خروج از حساب کاربری خود باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
توکن کاربر را از بین می برد
و پیام موفقیت را به کلاینت ارسال میکند
*/


void send_message();
/*
اگر درخواست کلاینت ارسال پیام در کانال باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
در فایل مربوط به کانال،
در مسیر
Resources\Channels
پیام مورد نظر ذخیره می شود
و پیام موفقیت را به کلاینت ارسال میکند
*/


void refresh();
/*
اگر درخواست کلاینت دیدن پیام های جدید کانال باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
از فایل مربوط به کانال،
در مسیر
Resources\Channels
پیام های جدید به کلاینت ارسال میشوند
و آخرین خطی که از پیام ها خوانده شده برای کاربر تغییر میکند
*/


void leave();
/*
اگر درخواست کلاینت خروج از کانال باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
در فایل مربوط مربوط به کانال، پیام
... left the channel
در مسیر
Resources\Channels
ایجاد می کند.
و کانال مربوط به کاربر حذف میشود و کابر از اعضای کانال حذف می شود
و پیام موفقیت را به کلاینت ارسال میکند
*/


void channel_members();
/*
اگر درخواست کلاینت مشاهده اعضای کانال باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
لیست اعضای کانال به کلاینت ارسال می شود
*/


void search_members();
/*
اگر درخواست کلاینت سرچ در اعضای کانال باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
به کلاینت فرستاده می شود که کاربر مورد نظر در این کانال وجود دارد یا خیر.
*/


void search_messages();
/*
اگر درخواست کلاینت سرچ یک واژه در پیام های کانال باشد، این تابع اجرا می شود.
اگر  کاربر در کانالی عضو نبود و یا توکنی که چنین درخواستی داده بود در سرور وجود نداشت، پیغام خطای مناسب ارسال میکند و در غیر این صورت
پیام هایی که شامل این کلمه هستند به کلاینت فرستاده می شود یا این که فرستاده می شود که پیامی که شامل این کلمه باشد یافت نشد.
*/


void myAddItemtoObject(char* response, char* part1, char* part1str, char* part2, char* part2str)
{
    sprintf(response, "{\"%s\":\"%s\",\"%s\":\"%s\"}", part1, part1str, part2, part2str);
}

void myAddItemtoArray(char* array, char* item)
{
    strcat(array, item);
    strcat(array, ",");
}

void myCreateArray(char* array)
{
    strcpy(array, "[");
}

void myArrayCreated(char* array)
{
    int l = strlen(array);

    array[l - 1] = '\0';

    strcat(array, "]");
}

void myAddArraytoObject(char* response, char* part1, char* part1str, char* part2, char* part2str)
{
    sprintf(response, "{\"%s\":\"%s\",\"%s\":%s}", part1, part1str, part2, part2str);
}

void myGetItem(char* response, char* part1, char* part1str, char* part2, char* part2str)
{
    char first[15], firstStr[100];
    char second[15], secondStr[1000];

    sscanf(response, "{\"%[^\"]\":\"%[^\"]\",\"%[^\"]\":\"%[^\"]\"}", first, firstStr, second, secondStr);

    if(!strcmp(first, part1))
        strcpy(part1str, firstStr);
    if(!strcmp(second, part2))
        strcpy(part2str, secondStr);
}

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

    char response[1000];

    if ((fileptr = fopen(address, "r")) != NULL)
    {
        myAddItemtoObject(response, "type", "Error", "content", "This username already exists!");
        fclose(fileptr);
    }
    else
    {
        myAddItemtoObject(response, "type", "Successful", "content", "");

        fileptr = fopen(address, "w");

        fprintf(fileptr, "%s, %s", username, password);
        fclose(fileptr);
    }

    send(client_socket, response, MAX, 0);

    printf("%s\n", response);
}

void login()
{
    sscanf(prompt, "%*s %[^,], %[^\n]\n", username, password);

    sprintf(address,"%s%s%s" ,"Resources\\Users\\", username, ".txt");

    char response[1000];
    char saveduser[MAX], savedpass[MAX];

    if ((fileptr = fopen(address, "r")) == NULL)
    {
        myAddItemtoObject(response, "type", "Error", "content", "Username could not be found!");
    }
    else
    {
        fscanf(fileptr, "%[^,], %[^\n]\n", saveduser, savedpass);

        if (strcmp(password, savedpass))
        {
            myAddItemtoObject(response, "type", "Error", "content", "Password is wrong!");
        }
        else
        {
            createToken();

            myAddItemtoObject(response, "type", "AuthToken", "content", token);

            strcpy(onlineUsers[onlineCount].username, username);
            strcpy(onlineUsers[onlineCount].token, token);

            onlineCount++;
        }
        fclose(fileptr);
    }
    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void create_channel()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", channelName, token);

    sprintf(address, "%s%s%s", "Resources\\Channels\\", channelName, ".txt");
    sprintf(first, "%s%s", channelName, ".txt");
    char response[1000] = {};
    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if (findChannel(first))
        {
            myAddItemtoObject(response, "type", "Error", "content", "This channel name already exists!");
        }
        //when there is no problem!:
        else
        {
            myAddItemtoObject(response, "type", "Successful", "content", "");
            //creating a file for channel
            fileptr = fopen(address, "w");

            fprintf(fileptr, "{\"name\":\"%s\"}\n", channelName);

            char creator[1000] = {};
            sprintf(first, "%s created the channel.", name);
            myAddItemtoObject(creator, "sender", "server", "content", first);
            fprintf(fileptr, "%s\n", creator);
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
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }
    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void join_channel()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", channelName, token);
    sprintf(first, "%s%s", channelName, ".txt");
    sprintf(address, "%s%s%s", "Resources\\Channels\\", channelName, ".txt");

    char response[1000] = {};
    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if (!findChannel(first))
        {
            myAddItemtoObject(response, "type", "Error", "content", "There is no such channel!");
        }
        //when there is no problem!:
        else
        {
            myAddItemtoObject(response, "type", "Successful", "content", "");

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
            char joiner[1000] = {};
            sprintf(first, "%s joined the channel.", name);
            myAddItemtoObject(joiner, "sender", "server", "content", first);
            fprintf(fileptr, "%s\n", joiner);
            fclose(fileptr);
        }
    }
    else
    {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }
    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void logout()
{
    sscanf(prompt, "%*s %s", token);

    char response[1000] = {};

    int i = findOnlineUser(token, name);
    if (i != -1)
    {
        strcpy(onlineUsers[i].token, "");
        strcpy(onlineUsers[i].username, "");
        strcpy(onlineUsers[i].channelName, "");

        myAddItemtoObject(response, "type", "Successful", "content", "");
    }
    else
    {
         myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }

    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}


void send_message()
{
    sscanf(prompt, "%*s %[^,], %[^\n]\n", pm, token);
    char response[1000] = {};

    int i = findOnlineUser(token, name);

    sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is n0 problem:
        {
            fileptr = fopen(address, "a");
            char message[1000] = {};
            myAddItemtoObject(message, "sender", name, "content", pm);
            fprintf(fileptr, "%s\n", message);

            fclose(fileptr);

            myAddItemtoObject(response, "type", "Successful", "content", "");
        }
        else
        {
            myAddItemtoObject(response, "type", "Error", "content", "This user is not member of any channel!");
        }
    }
    else
    {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }

    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void refresh()
{
     sscanf(prompt, "%*s %[^\n]\n", token);
    int i = findOnlineUser(token, name);
    char response[10000] = {};

    sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

    if (i != -1) {
        if (strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            int j;
            int readline = onlineUsers[i].line;
            fileptr = fopen(address, "r");
            char content[10000] = {};
            myCreateArray(content);

            //cJSON_AddItemToObject(response, "type", cJSON_CreateString("List"));

            for (j = 0; j < readline + 1; j++)
            {
                fgets(pm, 10 * MAX, fileptr);
            } //passing lines which are already read.
            for (j = readline; fgets(pm, 10 * MAX, fileptr); j++)
            {
                myAddItemtoArray(content, pm);
            }
            myArrayCreated(content);

            myAddArraytoObject(response, "type", "List", "content", content);

            onlineUsers[i].line = j;

            fclose(fileptr);
        } else {
            myAddItemtoObject(response, "type", "Error", "content", "This user is not member of any channel!");
        }
    } else {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }
    send(client_socket, response, MAX, 0);
     printf("%s\n", response);
}

void leave()
{
    sscanf(prompt, "%*s %s", token);
    char response[1000] = {};

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

            char left[1000] = {};
            sprintf(first, "%s left the channel.", name);
            myAddItemtoObject(left, "sender", "server", "content", first);
            fprintf(fileptr, "%s\n", left);

            fclose(fileptr);

            strcpy(onlineUsers[i].channelName, "");
            myAddItemtoObject(response, "type", "Successful", "content", "");
        }
        else
        {
            myAddItemtoObject(response, "type", "Error", "content", "This user is not member of any channel!");
        }
    }
    else
    {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }

    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void channel_members()
{
    sscanf(prompt, "%*s %*s %s", token);
    char response[1000] = {};

    int i = findOnlineUser(token, name);
    int x = findActiveChannel(onlineUsers[i].channelName);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            char content[1000] = {};
            myCreateArray(content);

            for (int j = 0; j < activeChannels[x].memberCount; ++j)
                if(strcmp(activeChannels[x].members[j], ""))
                    myAddItemtoArray(content, activeChannels[x].members[j]);

            myArrayCreated(content);

            myAddItemtoObject(response, "type", "List", "content", content);
        }
        else
        {
            myAddItemtoObject(response, "type", "Error", "content", "This user is not member of any channel!");
        }
    }
    else
    {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }

    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void search_members()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", username, token);
    char response[1000] = {};

    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            int flag = 1;
            int x = findActiveChannel(onlineUsers[i].channelName);

            for(int j = 0; j < activeChannels[x].memberCount; ++j)
                if(!strcmp(username, activeChannels[x].members[j]))
                {
                    flag = 0;
                    sprintf(first, "%s IS member of your channel.", username);
                    myAddItemtoObject(response, "type", "Successful", "content", first);
                    break;
                }

            if (flag)
            {
                sprintf(first, "%s IS NOT member of your channel.", username);
                myAddItemtoObject(response, "type", "Successful", "content", first);
            }
        }
        else
        {
            myAddItemtoObject(response, "type", "Error", "content", "This user is not member of any channel!");
        }
    }
    else
    {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }

    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

void search_messages()
{
    sscanf(prompt, "%*s %*s %[^,], %[^\n]\n", first, token);

    char response[10000] = {};

    int i = findOnlineUser(token, name);

    if (i != -1)
    {
        if(strcmp(onlineUsers[i].channelName, "")) //if there is no problem:
        {
            char foundMessages[10 * MAX] = {};
            char* ptr;
            int counter = 0;

            sprintf(address, "%s%s%s", "Resources\\Channels\\", onlineUsers[i].channelName, ".txt");

            fileptr = fopen(address, "r");
            fgets(pm, 10 * MAX, fileptr);

            for ( ; fgets(pm, 10 * MAX, fileptr); )
            {
                char sender[1000], content[10000];

                myGetItem(pm, "sender", sender, "content", content);

                ptr = content;
                for(; strlen(ptr); )
                {
                    sscanf(ptr, "%s", name);
                    if (!strcmp(name, first))
                    {
                        strcat(foundMessages, sender);
                        strcat(foundMessages, ": ");
                        strcat(foundMessages, content);
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
                myAddItemtoObject(response, "type", "Successful", "content", "No message contains this word!");
            else
                myAddItemtoObject(response, "type", "Successful", "content", foundMessages);
        }
        else
        {
            myAddItemtoObject(response, "type", "Error", "content", "This user is not member of any channel!");
        }
    }
    else
    {
        myAddItemtoObject(response, "type", "Error", "content", "There is no such a AuthToken!");
    }

    send(client_socket, response, MAX, 0);
    printf("%s\n", response);
}

