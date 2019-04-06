package com.example.foxmap_native_1;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLMapView extends GLSurfaceView {
    private static final String TAG = "GLMapView";
    public GLMapView(Context context) {
        super(context);
    }
    public GLMapView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }


    public void init(){
        setEGLContextClientVersion(2);     //На данный момент актуальна 3 версия (?)
        setRenderer(new GLSurfaceView.Renderer() {
            @Override
            //Вызывается при создании/пересоздании Surface
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                //Устанавливаем параметры OpenGL
                Log.d(TAG, "onSurfaceCreated()");
                MapDrawerJNI.surfaceCreated();
            }

            @Override
            //Вызывается при смене размера Surface (в т.ч. при создании и при смене ориентации экрана)
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                Log.d(TAG, "onSurfaceChanged");
                MapDrawerJNI.surfaceChanged(width, height);
            }

            @Override
            //Вызывается по готовности Surface отобразить новый кадр
            public void onDrawFrame(GL10 gl) {
                MapDrawerJNI.drawFrame();
            }
        });
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);    //перерисовка по требованию (не постоянно)

        //Переданный Runnable будет запущен в GL-потоке
        //так как нативная основа MapDriverJNI работает в нём.
        //  Стандартный способ общения с Renderer-ом
        //Инициализируем рисовальщика
        queueEvent(new Runnable() {
            @Override
            public void run() {
                MapDrawerJNI.init();
            }
        });
    }

    // Is called by Activity
    @Override
    public void onPause() {
        super.onPause();
        queueEvent(new Runnable() {
            @Override
            public void run() {
                MapDrawerJNI.onPause();
            }
        });
    }

    //  Is called by Activity
    @Override
    public void onResume() {
        super.onResume();
        queueEvent(new Runnable() {
            @Override
            public void run() {
                MapDrawerJNI.onResume();
            }
        });
    }
}
