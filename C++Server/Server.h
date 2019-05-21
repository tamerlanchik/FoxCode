#include "Entity.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <zconf.h>
#include"DBMaster.h"

class Server{
public:
    int Connect();
    void CreateSocket(int);
    static void * socketThread(void *arg);
    static void Fill(int s1);
    int BDversion;
    int currentBDversion=0;
};
