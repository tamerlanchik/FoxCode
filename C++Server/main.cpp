
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <zconf.h>
#include "Server.h"

int main(int argc, char *argv[]) {

    Server server;

    int s1=server.Connect();

    //сходили в бд, взяли номер версии, записали в currentBDversion

    if (server.currentBDversion == server.BDversion) {

        write(s1,"NO UPDATES", 10);
        printf("NO UPDATES");
    }
    else {

        server.BDversion=server.currentBDversion;
        server.Fill(s1);
        printf("worked good");
    }
    close(s1);

    return 0;
}

