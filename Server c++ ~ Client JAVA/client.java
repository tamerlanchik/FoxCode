import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Vector;


public class client {
    public static class Hall
    {
        int id;
        float coordinateX;
        float coordinateY;
        int floor;
        int status; //true-1, false-0
        public Hall(int id,float coordinateX,float coordinateY,int floor,int status){
            this.id=id;
            this.coordinateX=coordinateX;
            this.coordinateY=coordinateY;
            this.floor=floor;
            this.status=status;
        }
    };

    public static class Room
    {
        int id;
        float coordinateX;
        float coordinateY;
        int floor;
        int status;//true-1, false-0
        int idHall;
        Room(int id, float coordinateX, float coordinateY, int floor, int status, int idHall){
            this.id=id;
            this.coordinateX=coordinateX;
            this.coordinateY=coordinateY;
            this.floor=floor;
            this.status=status;
            this.idHall=idHall;
        }
    };

    public static class MoveObject
    {
        int minFloor;
        int maxFloor;
        MoveObject(int minFloor, int maxFloor){
            this.maxFloor=maxFloor;
            this.minFloor=minFloor;
        }
    };
    public static void main(String[] args) {
        try {
            Socket sock = new Socket("127.0.0.1", 1234);
            BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));

            //тест что все пересылается
            String p = br.readLine();
            System.out.println(p);
            //

            int hallsSize = br.read();
            int roomsSize = br.read();
            int moveObjectsSize = br.read();

            Vector<Hall> halls=new Vector<Hall>();
            Vector<Room> rooms=new Vector<Room>();
            Vector<MoveObject> moveObjects=new Vector<MoveObject>();


            for(int i=0;i<hallsSize;i++){
                halls.add(new Hall(br.read(),Float.parseFloat(br.readLine()),Float.parseFloat(br.readLine()),br.read(),br.read()));
            }
            for(int i=0;i<roomsSize;i++){
                rooms.add(new Room(br.read(),Float.parseFloat(br.readLine()),Float.parseFloat(br.readLine()),br.read(),br.read(),br.read()));
            }
            for(int i=0;i<moveObjectsSize;i++){
                moveObjects.add(new MoveObject(br.read(),br.read()));
            }

            sock.close();
        } catch (Exception ex) {}
    }
}