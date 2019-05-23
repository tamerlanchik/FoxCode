package com.example.foxmap_native_1;

import android.content.Context;
import android.content.res.AssetManager;
import android.database.sqlite.SQLiteDatabase;
import android.location.Address;
import android.net.Uri;
import android.util.Log;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;

public class StorageMasterJNI {
    private static final String TAG = "StorageMasterJNI";
    private String mDBName = "MapDB.db";
    private String mServerAddress = "127.0.0.1";
    private int mServerPort = 80;

    Context mContext;
    StorageMasterJNI(Context context){
        mContext = context;
    }
    StorageMasterJNI(Context context, String dbName){
        this(context);
        mDBName = dbName;
    }
    StorageMasterJNI(Context context, String dbName, String serverAddress, int port){
        this(context, dbName);
        mServerAddress = serverAddress;
        mServerPort = port;
    }

    public int updateDatabaseRequest(){
        class Task implements Runnable{
            private int result = 0;
            @Override
            public void run() {
                try {
                    new NetworkMaster(mServerAddress, mServerPort);
                } catch (Exception e) {
                    Log.e(TAG, "Cannot create socket: " + e.getMessage());
                    result = 1;
                    //return;
                }
                String path = mContext.getDatabasePath(mDBName).toString();
                if(init(path, mContext.getAssets()) != 0){
                    Log.e(TAG, "Cannot init database: error returned");
                    result = 2;
                    return;
                }
            }

            int getResult() {
                return result;
            }
        }
        /*Thread t = new Thread(new Runnable() {

            @Override
            public void run() {
                try {
                    new NetworkMaster(mServerAddress, mServerPort);
                } catch (Exception e) {
                    Log.e(TAG, "Cannot create socket: " + e.getMessage());
                    return;
                }
                //String path = "/data/data/com.example.foxmap_native_1/databases/MapDB.db";
                String path = mContext.getDatabasePath(mDBName).toString();
                if(init(path, mContext.getAssets()) != 0){
                    Log.e(TAG, "Cannot init database: error returned");
                    throw(new Exception());
                }
            }
        });*/
        Task task = new Task();
        Thread t = new Thread(task);
        t.start();
        try {
            t.join(5000);
            if(t.isAlive()){
                t.interrupt();
                Log.d(TAG, "IsAlive val = " + Boolean.toString(t.isAlive()));
                return 2;
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
            Log.e(TAG, "Error during UpdateDatabaseRequest()");
            return 2;
        }
        t.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread t, Throwable e) {
                Log.e(TAG, "Unknown exception: thread StorageMaster");
            }
        });
        switch(task.getResult()){
            case 1:
                Log.e(TAG, "Cannot create socket!");
                return 1;
            case 2:
                Log.d(TAG, "Cannot init storage!");
                return 2;
            default:
                Log.d(TAG, "Succesfully initialized storage!");
                break;
        }
        return 0;
    }

    static {
        System.loadLibrary("NativeDispatcher");
    }
    private native int init(String path, AssetManager assetManager);
    private native void inflateDatabase();

}
