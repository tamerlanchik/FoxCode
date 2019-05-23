#include "Server.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *Server:: socketThread(void *arg)
{
    int newSocket = *((int *)arg);

    pthread_mutex_lock(&lock);

    Fill(newSocket);

    printf("Exit socketThread \n");
    close(newSocket);
    pthread_exit(NULL);
}

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
    return s0;
}
void Server::CreateSocket(int s0) {
    struct sockaddr_in peeraddr{};
    socklen_t peeraddr_len;
    pthread_t tid[60];
    int i = 0;
    int newSocket = accept(s0, (struct sockaddr *) &peeraddr, &peeraddr_len);
    if( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )
        printf("Failed to create thread\n");
    if( i >= 50)
    {
        i = 0;
        while(i < 50)
        {
            pthread_join(tid[i++],NULL);
        }
        i = 0;
    }

    printf(
            "Connection from IP %d.%d.%d.%d, port %d\n",
            (ntohl(peeraddr.sin_addr.s_addr) >> 24) & 0xff,
            (ntohl(peeraddr.sin_addr.s_addr) >> 16) & 0xff,
            (ntohl(peeraddr.sin_addr.s_addr) >> 8) & 0xff,
            ntohl(peeraddr.sin_addr.s_addr) & 0xff,
            ntohs(peeraddr.sin_port)
    );
}



void Server::Fill(int s1) {

    std::vector<Hall> halls;
    std::vector<Room> rooms;

    DBMaster dbMaster("sqlite_lib/MapDB.db");
    dbMaster.ReadAllData();

    halls=dbMaster.GetHalls();
    rooms=dbMaster.GetRooms();

    int hallsSize = halls.size();
    int roomsSize = rooms.size();

    write(s1, &hallsSize, sizeof(hallsSize));
    write(s1, &roomsSize, sizeof(roomsSize));

    for (int i = 0; i < hallsSize; i++) {
        write(s1, &halls[i].ID, sizeof(halls[i].ID));
        for(int j=0;j<3;j++){
            write(s1, &halls[i].LeftTop.x, sizeof(halls[i].LeftTop.x));
            write(s1, &halls[i].LeftTop.y, sizeof(halls[i].LeftTop.y));
            write(s1, &halls[i].LeftTop.z, sizeof(halls[i].LeftTop.z));
        }
        for(int j=0;j<3;j++){
            write(s1, &halls[i].RightBottom.x, sizeof(halls[i].RightBottom.x));
            write(s1, &halls[i].RightBottom.y, sizeof(halls[i].RightBottom.y));
            write(s1, &halls[i].RightBottom.z, sizeof(halls[i].RightBottom.z));
        }
        std::string st;
        if(halls[i].Status){
            st ="true";
        }else{
            st="false";
        }
        write(s1, &st, st.size());
    }

    for (int i = 0; i < roomsSize; i++) {

        write(s1, &rooms[i].ID, sizeof(rooms[i].ID));
        for(int j=0;j<3;j++){
            write(s1, &rooms[i].LeftTop.x, sizeof(rooms[i].LeftTop.x));
            write(s1, &rooms[i].LeftTop.y, sizeof(rooms[i].LeftTop.y));
            write(s1, &rooms[i].LeftTop.z, sizeof(rooms[i].LeftTop.z));
        }
        for(int j=0;j<3;j++){
            write(s1, &rooms[i].RightBottom.x, sizeof(rooms[i].RightBottom.x));
            write(s1, &rooms[i].RightBottom.y, sizeof(rooms[i].RightBottom.y));
            write(s1, &rooms[i].RightBottom.z, sizeof(rooms[i].RightBottom.z));
        }
        std::string st;
        if(rooms[i].Status){
            st ="true";
        }else{
            st="false";
        }
        write(s1, &st, st.size());



        write(s1, &rooms[i].Type, rooms[i].Type.size());

        int vecsizeHallID=rooms[i].HallID.size();
        write(s1, &vecsizeHallID, sizeof(vecsizeHallID));

        for(int j=0;j<rooms[i].HallID.size();j++){
            write(s1, &rooms[i].HallID[j], sizeof(rooms[i].HallID[j]));
        }

        int vecsizeWight=rooms[i].Wight.size();
        write(s1, &vecsizeWight, sizeof(vecsizeWight));

        for(int j=0;j<rooms[i].Wight.size();j++){
            write(s1, &rooms[i].Wight[j], sizeof(rooms[i].Wight[j]));
        }

        int vecsizeInput=rooms[i].Input.size();
        write(s1, &vecsizeInput, sizeof(vecsizeInput));

        for(int j=0;j<rooms[i].Input.size();j++){
            write(s1, &rooms[i].Input[j].x, sizeof(rooms[i].Input[j].x));
            write(s1, &rooms[i].Input[j].y, sizeof(rooms[i].Input[j].y));
            write(s1, &rooms[i].Input[j].z, sizeof(rooms[i].Input[j].z));
        }

    }

}

