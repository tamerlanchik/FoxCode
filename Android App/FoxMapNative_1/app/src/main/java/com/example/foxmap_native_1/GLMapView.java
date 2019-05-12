package com.example.foxmap_native_1;

import android.content.Context;
import android.graphics.PointF;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLMapView extends GLSurfaceView{
    private static final String TAG = "GLMapView";

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
        setRenderer(new Renderer());
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);    //перерисовка по требованию (не постоянно)
        this.setOnTouchListener(new TouchListener());
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
        }

        @Override
        //Вызывается по готовности Surface отобразить новый кадр
        public void onDrawFrame(GL10 gl) {
            MapDrawerJNI.drawFrame();
        }
    };

    class TouchListener implements OnTouchListener{
        private PointF mPrevA = new PointF(0,0);
        private PointF mPrevB = new PointF(0,0);
        private float mPrevDist = 0;
        private boolean mIsDragging = false;
        private PointF mScreenDimensions = new PointF(0,0);
        @Override
        public boolean onTouch(View v, MotionEvent event) {

            int actionMask = event.getActionMasked();
            int pointerIndex = event.getActionIndex();
            int pointerCount = event.getPointerCount();
            /*Log.d(TAG, "onTouch: action=" + Integer.toString(actionMask)
                + " pointerIndex=" + Integer.toString(pointerIndex) + " pointerCount=" + Integer.toString(pointerCount));
            */
            if(pointerCount > 2)
                return false;
            switch (actionMask){
                case MotionEvent.ACTION_DOWN:   // первое касание
                    mIsDragging = true;
                    mPrevA.set(event.getX(), event.getY());
                    mScreenDimensions.set(getWidth(), getHeight());
                    Log.d(TAG, "Start moving");
                    break;

                case MotionEvent.ACTION_POINTER_DOWN:   // второе и последующие касания
                    mIsDragging = false;
                    if(pointerIndex == 1){
                        Log.d(TAG, "Start scaling");
                    }else{
                        Log.d(TAG, "Pause scaling: too many tips: "+ Integer.toString(pointerCount));
                    }
                    mPrevB.set(event.getX(), event.getY());
                    mPrevDist = mPrevB.length(mPrevA.x, mPrevA.y);
                    break;
                case MotionEvent.ACTION_MOVE:
                    if(pointerCount == 1){
                        if(mIsDragging){

                            MapDrawerJNI.commitMapMovement(event.getX()-mPrevA.x, event.getY() - mPrevA.y);
                        }
                        mPrevA.set(event.getX(), event.getY());
                    }else{
                        float newL = mPrevDist;
                        float ds = 1;
                        switch(pointerIndex){
                            case 0:
                                newL = mPrevB.length(event.getX(), event.getY());
                                ds = 1 + (newL - mPrevDist)/mScreenDimensions.y;
                                mPrevA.set(event.getX(), event.getY());
                                break;
                            case 1:
                                newL = mPrevB.length(event.getX(), event.getY());
                                ds = 1 + (newL - mPrevDist)/mScreenDimensions.y;
                                mPrevA.set(event.getX(), event.getY());
                                break;
                        }
                        MapDrawerJNI.commitMapZoom(ds*1);
                        mPrevDist = newL;
                    }
                    requestRender();
                    break;
                case MotionEvent.ACTION_POINTER_UP:     //отпускаем второй и последующий палец
                    if(pointerIndex == 1){
                        Log.d(TAG, "Stop scaling");
                        //return false;
                    }else{
                        Log.d(TAG, "Continue scaling: " + Integer.toString(pointerCount-1));
                    }
                    break;
                case MotionEvent.ACTION_UP:     //отпускаем последний палец
                case MotionEvent.ACTION_CANCEL:
                    mIsDragging = false;
                    Log.d(TAG, "Stop moving");
                    break;
            }
            return true;
        }

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
