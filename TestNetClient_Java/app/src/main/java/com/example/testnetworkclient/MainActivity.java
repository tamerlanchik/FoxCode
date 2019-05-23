package com.example.testnetworkclient;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.io.IOException;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Vector;

public class MainActivity extends AppCompatActivity {
    private Button mOpenButton;
    private Button mCloseButton;
    private SimpleNet mNet = null;
    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mNet = new SimpleNet("192.168.1.69", 80);
        mOpenButton = findViewById(R.id.button_open);
        mOpenButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                new Net("192.168.1.69", 80);
                            } catch (Exception e) {
                                Log.e(TAG, e.getMessage());
                            }
                        }
                    }).start();
            }
        });

        mCloseButton = findViewById(R.id.button_close);
        mCloseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        mNet.closeConnection();
                    }
                }).start();
            }
        });
    }
    class SimpleNet{
        private final String TAG = "SimpleNet";
        private String mAddress;
        private int mPort;
        private Socket mSocket = null;

        public SimpleNet(String addr, int port){
            mAddress = addr;
            mPort = port;
        }

        public void openConnection() throws Exception{
            try{
                if(mSocket==null){
                    mSocket = new Socket(mAddress, mPort);
                    Log.d(TAG, "Socket opened");
                }else{
                    Log.d(TAG, "Socket is already opened");
                }
            }catch (IOException e) {
                throw new Exception("Cannot create socket " + e.getMessage());
            }
        }

        public void closeConnection() {
            if(mSocket != null && !mSocket.isClosed()){
                try{
                    mSocket.close();
                } catch (IOException e) {
                    Log.d(TAG, "Cannot close socket");
                }finally {
                    mSocket = null;
                }
            }
        }

        @Override
        protected void finalize() throws Throwable {
            super.finalize();
            closeConnection();
        }
    }
    class Net{
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

        public Net(String hostAddr, int port) {
            try {
                Socket sock = new Socket(hostAddr, port);
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
            } catch (Exception ex) {}
        }
    }
}
