#include <WinSock2.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main() {
    WSADATA lpWSAData;
    SOCKET s;

    int port_num = 3136;

    // Инициализация библиотеки Ws2_32.dll.
    if (WSAStartup(MAKEWORD(1,1),&lpWSAData)!=0) return(NULL);

    // Поучаем IP адрес по имени хоста
    struct hostent* hp;
    if (!(hp=gethostbyname("localhost"))) {
        return(NULL);
    }

    // Открываем сокет
    s=socket(AF_INET, SOCK_STREAM, 0);
    if (s==INVALID_SOCKET) {
        return(NULL);
    }

    // Заполняем структуру sockaddr_in
    struct sockaddr_in ssin;
    memset ((char *)&ssin, 0, sizeof(ssin));
    ssin.sin_family = AF_INET;
    ssin.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr[0];
    ssin.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr[1];
    ssin.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr[2];
    ssin.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr[3];
    ssin.sin_port = htons(port_num);

    // Соединяемся с хостом
    if (connect(s, (sockaddr *)&ssin, sizeof(ssin))==-1) {
        return(NULL);
    }

    int size=1024*1024; // 1Mb
    char *result=(char*)malloc(size);
    strcpy(result,"");
    char *result_ptr=result;
    int cnt = 1;
    while (cnt!=0 && size>0) {
        cnt=recv (s, result_ptr, sizeof(size),0);
        if (cnt>0) {
            result_ptr+=cnt;
            size-=cnt;
            }
        }
    *result_ptr=0;

    // Деинициализация библиотеки Ws2_32.dll
    WSACleanup();

    printf("%s", result);
}
