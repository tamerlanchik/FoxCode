package com.example.foxmap_native_1;

import android.content.Context;
import android.content.res.AssetManager;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.util.Log;

public class StorageMasterJNI {
    private static final String TAG = "StorageMasterJNI";
    Context mContext;
    StorageMasterJNI(Context context){
        mContext = context;
    }
    public void updateDatabaseRequest(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    new NetworkMaster(mContext.getString(R.string.ServerAddress),
                            mContext.getResources().getInteger(R.integer.ServerPort));
                } catch (Exception e) {
                    Log.e(TAG, "Cannot create socket: " + e.getMessage());
                }
            }
        }).start();
        String path = "/data/data/com.example.foxmap_native_1/databases/MapDB.db";
        SQLiteDatabase db = SQLiteDatabase.openDatabase(path, null, 0);
        if(db.isOpen() == true) {
            Log.d(TAG, "Database opened in Java");
            db.close();
        }else{
            Log.e(TAG, "Cannot open database in Java");
            return;
        }
        init(mContext.getAssets());
    }

    static {
        System.loadLibrary("NativeDispatcher");
    }
    private native void init(AssetManager manager);
    private native void inflateDatabase();

}
