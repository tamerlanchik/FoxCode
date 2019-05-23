package com.example.foxmap_native_1;

import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Vector;

public class NetworkMaster {
    private final String TAG = "Net";
    public class Coordinate
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
    public class Hall
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
    public class Room extends Hall
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

    public NetworkMaster(String hostAddr, int port) throws Exception{
        try {
            Socket sock = new Socket();
            sock.connect(new InetSocketAddress(hostAddr, port), 500);
            if(sock.isConnected())
                Log.d(TAG, "Connected");
            else
                Log.d(TAG, "No connection");

            BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            String p = " ";
            if(br.ready()){
                p = br.readLine();
                Log.d(TAG, "Read string");
            }else{
                Log.d(TAG, "No data");
                return;
            }

            if(p.equals("NO UPDATES")){
                System.out.println(p);
                Log.d(TAG, p);
                sock.close();
            }
            else {
                Log.d(TAG, "Not empty answer");
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
        } catch (Exception ex) {
            throw new Exception("Cannot create socket" + ex.getMessage());
        }
    }
}
