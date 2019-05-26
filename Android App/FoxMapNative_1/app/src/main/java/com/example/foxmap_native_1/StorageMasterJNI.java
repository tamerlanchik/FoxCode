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

    public int updateDatabaseRequest(boolean networkCrucial){
        int res = 0;
        if(!updateFromNetwork()){
            if(networkCrucial){
                return 1;
            }
            res = 1;
        }
        if(!updateDatabase())
            res = 2;
        return res;
    }

    static {
        System.loadLibrary("NativeDispatcher");
    }
    private native int init(String path, AssetManager assetManager);
    private native void inflateDatabase();

    private boolean updateFromNetwork() {
        try {
            new NetworkMaster(mServerAddress, mServerPort);
        } catch (Exception e) {
            return false;
        }
        return true;
    }
    private boolean updateDatabase(){
        String path = mContext.getDatabasePath(mDBName).toString();
        if(init(path, mContext.getAssets()) != 0){
            Log.e(TAG, "Cannot init database: error returned");
            return false;
        }
        return true;
    }
}
