package com.example.foxmap_native_1;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.widget.ImageView;

public class MapDrawer {
    ImageView mMap;
    Context mContext;
    public MapDrawer(Context context, ImageView imageView){
        mContext = context;
        mMap = imageView;
    }
    public void DrawRoute(){
        Drawable image = ContextCompat.getDrawable(mContext, R.drawable.map_routed);
        mMap.setBackground(image);
    }
}
