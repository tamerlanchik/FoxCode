
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

    DBMaster dbMaster("sqlite_lib/MapDB.db");
    dbMaster.ReadAllData();
    

    if (server.currentBDversion.Version == dbMaster.GetInfo().Version) {

        write(s1,"NO UPDATES", 10);
        printf("NO UPDATES");
        close(s1);

        return 0;
    }
    else {

        server.currentBDversion.Version=dbMaster.GetInfo().Version;
        server.CreateSocket(s1);
        printf("worked good");
    }

    return 0;
}

