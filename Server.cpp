#include "Server.h"

int Server::Connect() {

    int listenPort = 1234;

    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        perror("Cannot create a socket");
        exit(1);
    }

    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(listenPort);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int res = bind(s0, (struct sockaddr *) &myaddr, sizeof(myaddr));
    if (res < 0) {
        perror("Cannot bind a socket");
        exit(1);
    }

    struct linger linger_opt = {1, 0};
    setsockopt(s0, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    res = listen(s0, 1);
    if (res < 0) {
        perror("Cannot listen");
        exit(1);
    }

    struct sockaddr_in peeraddr{};
    socklen_t peeraddr_len;
    int s1 = accept(s0, (struct sockaddr *) &peeraddr, &peeraddr_len);
    if (s1 < 0) {
        perror("Cannot accept");
        exit(1);
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

    return s1;
}

void Server::Fill(int s1) {

    std::vector<Hall> halls;
    std::vector<Room> rooms;

    //Здесь должен быть блок похода в бд ---> наполнили структуру даннными

    int hallsSize = halls.size();
    int roomsSize = rooms.size();

    write(s1, &hallsSize, sizeof(hallsSize));
    write(s1, &roomsSize, sizeof(roomsSize));

    for (int i = 0; i < hallsSize; i++) {
        write(s1, &halls[i].ID, sizeof(halls[i].ID));
        write(s1, &halls[i].LeftTop, sizeof(halls[i].LeftTop));
        write(s1, &halls[i].RightBottom, sizeof(halls[i].RightBottom));
        write(s1, &halls[i].Status, sizeof(halls[i].Status));
    }

    for (int i = 0; i < roomsSize; i++) {
        write(s1, &rooms[i].HallID, sizeof(rooms[i].HallID));
        write(s1, &rooms[i].RightBottom, sizeof(rooms[i].RightBottom));
        write(s1, &rooms[i].LeftTop, sizeof(rooms[i].LeftTop));
        write(s1, &rooms[i].ID, sizeof(rooms[i].ID));
        write(s1, &rooms[i].Status, sizeof(rooms[i].Status));
        write(s1, &rooms[i].Input, sizeof(rooms[i].Input));
        write(s1, &rooms[i].Wight, sizeof(rooms[i].Wight));
        write(s1, &rooms[i].Type, sizeof(rooms[i].Type));
    }

}

