package com.example.bomonsearch2;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.widget.ImageView;

public class MapImageView extends android.support.v7.widget.AppCompatImageView {

    public MapImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void DrawRoute(){
        Drawable image = ContextCompat.getDrawable(getContext(), R.drawable.map_routed);
        setBackground(image);
    }
}
