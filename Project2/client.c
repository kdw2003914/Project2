#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

#define BUFSIZE 1024

// gcc client.c -lws2_32

DWORD WINAPI makeThread(void *data);

int main()
{    
    // Winsock Start - winsock.dll
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
    {
        printf("Error - Can not load 'winsock.dll' file\n");
        return 1;
    }
 
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("Error - Invalid socket\n");
        return 1;
    }
 
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("121.176.59.102");
 
    // 2.
    if (connect(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Error - Fail to connect\n");
        // 4. 
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return 1;
    } else  {
        printf("Server Connected\n");
    }

    HANDLE hThread;

    hThread = CreateThread(NULL, 0, makeThread, (void *)listenSocket, 0, NULL);
    CloseHandle(hThread);
     
    while (1)
    {        
        char messageBuffer[BUFSIZE] = {0};
        int receiveBytes = recv(listenSocket, messageBuffer, BUFSIZE, 0);
        if (receiveBytes > 0)
        {
            printf("%s\n", messageBuffer);
        }
        else {
            break;
        }
    }        
 
    printf("server be closen.\n");

    // 4. 
    closesocket(listenSocket);
 
    // Winsock End
    WSACleanup();

    Sleep(5000);

    return 0;
}

DWORD WINAPI makeThread(void *data) {
    SOCKET socket = (SOCKET)data;
    int first = 1;
    while (1){
        char messageBuffer[BUFSIZE];
        if (first) {
            first = 0;
            printf("* Typed your name\n");
        } else {
            printf("* Enter Message\n");
        }
        /*fgets(messageBuffer, sizeof(messageBuffer), stdin);
        bufferLen = strlen(messageBuffer);*/
        int i, bufferLen;
        for (i = 0; 1; i++)
        {
            messageBuffer[i] = getchar();
            if (messageBuffer[i] == '\n')
            {
                messageBuffer[i++] = '\0';
                break;
            }
        }
        bufferLen = i;

        int sendBytes = send(socket, messageBuffer, bufferLen, 0);
    }
    closesocket(socket);
    return 0;
}
