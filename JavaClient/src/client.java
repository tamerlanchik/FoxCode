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
                    int id=br.read();
                    Coordinate lefttop= new Coordinate(br.read(),br.read(),br.read());
                    Coordinate rightbottom= new Coordinate(br.read(),br.read(),br.read());
                    String st=br.readLine();
                    boolean status;
                    status= st.equals("true");
                    halls.add(new Hall(id,lefttop,rightbottom,status));
                }
                for (int i = 0; i < roomsSize; i++) {
                    int id=br.read();
                    Coordinate lefttop= new Coordinate(br.read(),br.read(),br.read());
                    Coordinate rightbottom= new Coordinate(br.read(),br.read(),br.read());
                    String st=br.readLine();
                    boolean status;
                    status= st.equals("true");

                    String type=br.readLine();

                    int vecsizeHallID=br.read();
                    Vector<Integer> vecHallid = new Vector<>();
                    for(int j=0;j<vecsizeHallID;j++){
                        vecHallid.add(br.read());
                    }

                    int vecsizeWight=br.read();
                    Vector<Integer> vecWight = new Vector<>();
                    for(int j=0;j<vecsizeWight;j++){
                        vecWight.add(br.read());
                    }

                    int vecsizeInput=br.read();
                    Vector<Coordinate> vecInput = new Vector<>();
                    for(int j=0;j<vecsizeInput;j++){
                        Coordinate cor=new Coordinate(br.read(),br.read(),br.read());
                        vecInput.add(cor);
                    }

                  rooms.add(new Room(id,lefttop,rightbottom,status,vecInput,vecWight,vecHallid,type));
                }

                sock.close();
            }
        } catch (Exception ex) {}
    }
}