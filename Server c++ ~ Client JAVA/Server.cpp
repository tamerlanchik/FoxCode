#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <zconf.h>

struct Hall
{
    int id;
    float coordinateX;
    float coordinateY;
    int floor;
    bool status;
};

struct Room
{
    int id;
    float coordinateX;
    float coordinateY;
    int floor;
    bool status;
    int idHall;
};

struct MoveObject
{
    int minFloor;
    int maxFloor;
};

int main(int argc, char *argv[]) {

    int listenPort = 1234;
    if (argc > 1)
        listenPort = atoi(argv[1]);

    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        perror("Cannot create a socket"); exit(1);
    }

    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(listenPort);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int res = bind(s0, (struct sockaddr*) &myaddr, sizeof(myaddr));
    if (res < 0) {
        perror("Cannot bind a socket"); exit(1);
    }

    struct linger linger_opt = { 1, 0 };
    setsockopt(s0, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    res = listen(s0, 1);
    if (res < 0) {
        perror("Cannot listen"); exit(1);
    }

    struct sockaddr_in peeraddr{};
    socklen_t peeraddr_len;
    int s1 = accept(s0, (struct sockaddr*) &peeraddr, &peeraddr_len);
    if (s1 < 0) {
        perror("Cannot accept"); exit(1);
    }

    printf(
            "Connection from IP %d.%d.%d.%d, port %d\n",
            (ntohl(peeraddr.sin_addr.s_addr) >> 24) & 0xff,
            (ntohl(peeraddr.sin_addr.s_addr) >> 16) & 0xff,
            (ntohl(peeraddr.sin_addr.s_addr) >> 8) & 0xff,
            ntohl(peeraddr.sin_addr.s_addr) & 0xff,
            ntohs(peeraddr.sin_port)
    );
    res = close(s0);

    std::vector<Hall>halls;
    std::vector<Room>rooms;
    std::vector<MoveObject>moveObjects;
    //Здесь должен быть блок похода в бд ---> наполнили структуру даннными
    int hallsSize=halls.size();
    int roomsSize=rooms.size();
    int moveObjectsSize=moveObjects.size();

    //все работает тест
    write(s1, "Helloo!\r\n", 9);
    //

    write(s1,&hallsSize, sizeof( hallsSize));
    write(s1,&roomsSize, sizeof( roomsSize));
    write(s1,&moveObjectsSize, sizeof( moveObjectsSize));

    for(int i=0;i<hallsSize;i++){
        write(s1,&halls[i].id, sizeof( halls[i].id));
        write(s1,&halls[i].floor, sizeof( halls[i].floor));
        write(s1,&halls[i].coordinateX, sizeof( halls[i].coordinateX));
        write(s1,&halls[i].coordinateY, sizeof( halls[i].coordinateY));
        write(s1,&halls[i].status, sizeof( halls[i].status));
    }

    for(int i=0;i<roomsSize;i++){
        write(s1,&rooms[i].id, sizeof( rooms[i].id));
        write(s1,&rooms[i].floor, sizeof( rooms[i].floor));
        write(s1,&rooms[i].coordinateX, sizeof( rooms[i].coordinateX));
        write(s1,&rooms[i].coordinateY, sizeof( rooms[i].coordinateY));
        write(s1,&rooms[i].status, sizeof( rooms[i].status));
        write(s1,&rooms[i].idHall, sizeof( rooms[i].idHall));
    }

    for(int i=0;i<moveObjectsSize;i++){
        write(s1,&moveObjects[i].maxFloor, sizeof( moveObjects[i].maxFloor));
        write(s1,&moveObjects[i].minFloor, sizeof( moveObjects[i].minFloor));
    }

    printf("worked good");

    close(s1);
    return 0;
}
