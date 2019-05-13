import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Vector;


public class client {

    public static class Coordinate
    {
        int x;
        int y;
        int z;
        public Coordinate(int x, int y, int z){
            this.x=x;
            this.y=y;
            this.z=z;
        }

    }
    public static class Hall
    {
        int ID;
        Coordinate LeftTop;
        Coordinate RightBottom;
        boolean Status;
        public Hall(int ID,Coordinate LeftTop,Coordinate RightBottom,boolean Status){
            this.ID=ID;
            this.LeftTop=LeftTop;
            this.RightBottom=RightBottom;
            this.Status=Status;
        }
    };
    public static class Room extends Hall
    {
        Vector<Coordinate> Input;
        Vector<Integer> Wight;
        Vector<Integer> HallID;
        String Type;
        public Room(int ID,Coordinate LeftTop,Coordinate RightBottom,boolean Status,Vector<Coordinate> Input,Vector<Integer> Wight,Vector<Integer> HallID,String Type){
            super(ID,LeftTop,RightBottom,Status);
            this.HallID=HallID;
            this.Input=Input;
            this.Wight=Wight;
            this.Type=Type;
        }
    };

    public static void main(String[] args) {

        try {

            Socket sock = new Socket("127.0.0.1", 1234);

            BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));

            String p = br.readLine();

            if(p.equals("NO UPDATES")){
                System.out.println(p);
                sock.close();
            }
            else {

                int hallsSize = br.read();
                int roomsSize = br.read();

                Vector<Hall> halls = new Vector<Hall>();
                Vector<Room> rooms = new Vector<Room>();

                for (int i = 0; i < hallsSize; i++) {
                   halls.add(new Hall());
                }
                for (int i = 0; i < roomsSize; i++) {
                  rooms.add(new Room());
                }

                sock.close();
            }
        } catch (Exception ex) {}
    }
}