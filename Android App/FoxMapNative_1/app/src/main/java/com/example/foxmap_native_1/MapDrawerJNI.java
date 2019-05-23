package com.example.foxmap_native_1;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.widget.ImageView;

public class MapDrawerJNI {
    static{
        System.loadLibrary("NativeDispatcher");
    }

    public static native void init(AssetManager assetsManager);
    public static native void surfaceCreated();
    public static native void surfaceChanged(int width, int height);
    public static native void drawFrame();
    public static native void onPause();
    public static native void onResume();
    public static native void commitMapMovement(float dx, float dy);
    public static native void commitMapZoom(float dz);
    public static native void setFloor(int floor);
    public static native void rebind();
}
