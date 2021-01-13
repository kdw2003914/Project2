#include <winsock2.h>
#include <Windows.h>
#include <wininet.h> 
#include <stdio.h>
#include <string.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "wininet.lib")
#pragma comment (lib, "./ew32.lib")
#pragma warning(disable:4996)

#define BUFSIZE 1024

// gcc client.c -lws2_32 -lwininet

DWORD WINAPI makeThread(void *data);

int Utf8ToAscii(char** dest, char* src)
{
    WCHAR* wc_buffer;
    int len;

    int wide_char_len = MultiByteToWideChar(0xFDE9u, 0, src, -1, 0, 0);
    wc_buffer = (WCHAR*)malloc(2 * wide_char_len | -((unsigned long long)wide_char_len >> 31 != 0));
    MultiByteToWideChar(0xFDE9u, 0, src, -1, wc_buffer, wide_char_len);
    len = WideCharToMultiByte(0, 0, wc_buffer, -1, 0, 0, 0, 0);
    *dest = (CHAR*)malloc(len);
    WideCharToMultiByte(0, 0, wc_buffer, -1, *dest, len, 0, 0);
    free(wc_buffer);
    return len;
}

DWORD ReadHtmlText(HINTERNET ah_http_file, char* ap_html_string)
{

    HANDLE h_wait_event = CreateEvent(NULL, TRUE, 0, NULL);
    if (h_wait_event == NULL) return 0;

    char buffer[1025];
    DWORD read_byte = 0, error_count = 0, total_bytes = 0;

    while (InternetReadFile(ah_http_file, buffer, 1024, &read_byte)) {

        memcpy(ap_html_string + total_bytes, buffer, read_byte);
        total_bytes += read_byte;
        if (read_byte < 1024) {


            error_count++;
            if (error_count > 10) break;
            else WaitForSingleObject(h_wait_event, 50);
        }
        else error_count = 0;
    }

    *(ap_html_string + total_bytes) = 0;
    CloseHandle(h_wait_event);
    return total_bytes;
}


void LoadDataFromWebPage(const char* domain, const char* path, char** rslt)
{

    HINTERNET h_session = InternetOpenA("BlogScanner", PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
    HINTERNET h_connect = InternetConnectA(h_session, domain, INTERNET_INVALID_PORT_NUMBER, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    HINTERNET h_http_file = HttpOpenRequestA(h_connect, "GET", path, "HTTP/1.0", NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

    char* p_utf8_html_str = (char*)malloc(2 * 1024 * 1024);
    if (p_utf8_html_str != NULL) {
        if (HttpSendRequest(h_http_file, NULL, 0, 0, 0) == TRUE) {

            ReadHtmlText(h_http_file, p_utf8_html_str);

            Utf8ToAscii(rslt, p_utf8_html_str);
        }
        free(p_utf8_html_str);
    }


    if (h_http_file != NULL) InternetCloseHandle(h_http_file);
    if (h_connect != NULL) InternetCloseHandle(h_connect);
    if (h_session != NULL) InternetCloseHandle(h_session);
}


int main()
{
   /* system("chcp 65001"); // 코드페이지를 UTF-8로 변경.
    system("cls");
    
    // Winsock Start - winsock.dll 로드
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
    {
        printf("Error - Can not load 'winsock.dll' file\n");
        return 1;
    }
 
    // 1. 소켓생성
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("Error - Invalid socket\n");
        return 1;
    }
 
    // 서버정보 객체설정
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
 
    // 2. 연결요청
    if (connect(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Error - Fail to connect\n");
        // 4. 소켓종료
        closesocket(listenSocket);
        // Winsock End
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Server Connected\n");
    }

    HANDLE hThread;

    hThread = CreateThread(NULL, 0, makeThread, (void *)listenSocket, 0, NULL);
    CloseHandle(hThread);
     
    while (1)
    {        
        char messageBuffer[BUFSIZE] = {0};
        // 데이터 읽
        int receiveBytes = recv(listenSocket, messageBuffer, BUFSIZE, 0);
        if (receiveBytes > 0)
        {
            printf("TRACE - Receive message : %s (%d bytes)\n", messageBuffer, receiveBytes);
        }
    }        
 
    // 4. 소켓종료
    closesocket(listenSocket);
 
    // Winsock End
    WSACleanup();

    char* buffer = NULL;
    //   LoadDataFromWebPage("stdict.korean.go.kr", "/api/search.do?certkey_no=2201&key=77CF6E24D5171B35E3614C01BE677683&type_search=search&q=단어", &buffer);
    LoadDataFromWebPage("stdict.korean.go.kr", "/api/search.do?certkey_no=2201&key=77CF6E24D5171B35E3614C01BE677683&type_search=search&q=단어", &buffer);
    if (buffer) {
        /*strcpy(diction, buffer);
        puts(diction);*/
  /*      free(buffer);
    }
    else {
        puts("Error");
    }*/
    char c[3];

    scanf("%s", c);
    printf("%s", c);

}

DWORD WINAPI makeThread(void *data) {
    SOCKET socket = (SOCKET)data;
    while (1){
        char messageBuffer[BUFSIZE];
        printf("* Enter Message\n->");
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