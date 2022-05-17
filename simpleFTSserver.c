#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>

#define CHUNK_SIZE 1024
#define PATH_MAX 256

int errordisconnect(int sockfd,int clifd){
    if(clifd > 0){
        close(clifd);
    }
    close(sockfd);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    int rc, tmp;
    int listenfd, clientfd;
    int offset, size, nread, rcv_size;
    struct sockaddr_in servaddr, cliaddr;
    char buf[CHUNK_SIZE];
    char path[PATH_MAX];
    char* noent = "NO ENTRY";
    FILE* file;

    //ソケット生成
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0){
        printf("create sock fail\n");
        errordisconnect(listenfd,0);
    }

    //アドレス生成
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

    //ポート接続
    rc = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(rc < 0){
        printf("bind fail\n");
        errordisconnect(listenfd, 0);
    }

    //接続待ち
    rc = listen(listenfd, 5);
    if(rc < 0){
        printf("listen fail\n");
        errordisconnect(listenfd, 0);
    }

    //クライアントソケット生成
    tmp = sizeof(cliaddr);
    clientfd = accept(listenfd, (struct sockaddr*)&cliaddr, (socklen_t*)&tmp);
    if(rc < 0){
        printf("accept fail\n");
        errordisconnect(listenfd, clientfd);
    }

    /*コネクション処理*/

    //目標パスを受信待ち
    rc = recv(clientfd, path, PATH_MAX, 0);
    if(rc < 0){
        printf("accept fail\n");
        errordisconnect(listenfd, clientfd);
    }
    path[rc] = '\0'; //終端文字
    printf("[REQUEST PATH]: %s\n", path); //パスの表示
    
    //ファイルのオープン
    file = fopen(path, "r");
    if(file == NULL){
        printf("cannot open %s\n", path);
        rc = send(clientfd, noent, strlen(noent), 0);
        if(rc < 0){
            printf("send error fail\n");
            errordisconnect(listenfd, clientfd);
        }
    }

    //ファイルのリードと送信のループ

    return 0;
}