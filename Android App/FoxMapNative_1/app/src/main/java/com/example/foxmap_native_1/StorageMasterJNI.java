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

    public void updateDatabaseRequest(){
        new Thread(new Runnable() {
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
                }
            }
        }).start();
    }

    static {
        System.loadLibrary("NativeDispatcher");
    }
    private native int init(String path, AssetManager assetManager);
    private native void inflateDatabase();

}
