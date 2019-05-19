package com.example.foxmap_native_1;

import android.content.Context;

import java.util.List;

public class MapGuide {
    private Context mContext;
    private GLMapView mMapView;
    static{
        System.loadLibrary("NativeDispatcher");
    }

    public MapGuide(Context context){
        mContext = context;
    }

    public native boolean buildRoute(String from, String to);
    public native boolean findOnMap(String obj);
}
