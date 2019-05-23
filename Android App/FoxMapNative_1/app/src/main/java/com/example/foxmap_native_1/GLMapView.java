package com.example.foxmap_native_1;

import android.content.Context;
import android.graphics.PointF;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLMapView extends GLSurfaceView{
    private static final String TAG = "GLMapView";
    private Renderer mRenderer;

    public GLMapView(Context context) {
        super(context);
    }

    public GLMapView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void init(){
        // Если не вызвать функцию ниже, будет ошибка
        // "glDrawArrays is called with VERTEX_ARRAY client state disabled!"
        // Thanks to Chineese friends of ours!
        setEGLContextClientVersion(3);     //На данный момент актуальна 3 версия (?)
        mRenderer = new Renderer();
        setRenderer(mRenderer);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);    //перерисовка по требованию (не постоянно)
        this.setOnTouchListener(new TouchListener(getContext()));
    }

    class Renderer implements GLSurfaceView.Renderer{
        @Override
        //Вызывается при создании/пересоздании Surface
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //Устанавливаем параметры OpenGL
            Log.d(TAG, "onSurfaceCreated()");
            MapDrawerJNI.init(getContext().getAssets());
            MapDrawerJNI.surfaceCreated();

        }

        @Override
        //Вызывается при смене размера Surface (в т.ч. при создании и при смене ориентации экрана)
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Log.d(TAG, "onSurfaceChanged");
            MapDrawerJNI.surfaceChanged(width, height);
            requestRender();
        }

        @Override
        //Вызывается по готовности Surface отобразить новый кадр
        public void onDrawFrame(GL10 gl) {
            MapDrawerJNI.drawFrame();
        }

    };

    class TouchListener implements OnTouchListener{
        private PointF mPrev = new PointF(0,0);
        private boolean mIsDragging = false;
        private ScaleGestureDetector mScaleDetector;
        private GestureDetector mPanDetector;

        public TouchListener(Context context) {
            mScaleDetector = new ScaleGestureDetector(context, new ScaleGestureDetector.SimpleOnScaleGestureListener(){

                @Override
                public boolean onScale(ScaleGestureDetector detector) {
                    MapDrawerJNI.commitMapZoom(detector.getScaleFactor());
                    return true;
                }
            });

            mPanDetector = new GestureDetector(context, new GestureDetector.SimpleOnGestureListener(){
                @Override
                public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
                    MapDrawerJNI.commitMapMovement(-distanceX, -distanceY);
                    return true;
                }
            });
        }

        @Override
        public boolean onTouch(View v, MotionEvent event) {
            mScaleDetector.onTouchEvent(event);
            mPanDetector.onTouchEvent(event);
            requestRender();
            return true;
        }

    }

    public boolean drawRouteRequest(){
        requestRender();
        queueEvent(new Runnable() {
            @Override
            public void run() {
                MapDrawerJNI.rebind();
            }
        });
        return true;
    }
    public boolean drawObjectMarkerRequest(){
        drawRouteRequest();
        return true;
    }

    public void changeFloor(final int floor){
        queueEvent(new Runnable() {
            @Override
            public void run() {
                MapDrawerJNI.setFloor(floor);
            }
        });
        requestRender();
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
