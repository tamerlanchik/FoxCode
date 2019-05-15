#include "DBMaster.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <zconf.h>

class Server{
public:
    int Connect();
    void Fill(int s1);
    //Здесь должен быть блок похода в бд
    int BDversion=0;
    int currentBDversion=0;
};
