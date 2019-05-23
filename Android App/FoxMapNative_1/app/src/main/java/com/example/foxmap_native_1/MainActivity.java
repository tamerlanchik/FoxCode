package com.example.foxmap_native_1;

import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.pm.ConfigurationInfo;
import android.database.sqlite.SQLiteDatabase;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.PersistableBundle;
import android.support.constraint.ConstraintLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.SearchView;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.reflect.Array;
import java.util.Collections;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private static final String mCurrentStoreyKey = "STOREY_KEY";
    private static final int mStoreyRange[] = {3, 4};
    private String mServerAddress = "192.168.1.69";
    private int mServerPort = 80;
    private StorageMasterJNI mStorageMaster;

    private MenuItem mUpdateDataItem;
    private ProgressBar mProgressBar;
    private android.widget.SearchView mDestSearchView;
    private android.widget.SearchView mSourceSearchView;
    private ImageView mMapPlaceHolder;
    private ImageButton mReverseRouteButton;
    private TextView mFloorNumberTextView;
    private TextView mSelectedObjTextView;
    private FloatingActionButton mGoUpButton;
    private FloatingActionButton mGoDownButton;
    private GLMapView mMapView;
    private MapGuide mMapGuide;

    private NetworkMaster mNetworkMaster;

    private int mFloor = 3;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Проверяем поддержку необходимой версии OpenGL ES (2)
        if( !supportES3() ){
            Toast.makeText(getApplicationContext(),
                    getResources().getString(R.string.toast_no_opengl_support),
                    Toast.LENGTH_LONG)
                    .show();
            finish();
            return;
        }

        if(savedInstanceState != null){
            mFloor = savedInstanceState.getInt(mCurrentStoreyKey, 3);
        }

        mStorageMaster = new StorageMasterJNI(getApplicationContext(),
                getString(R.string.DatabaseName), getString(R.string.ServerAddress),
                getResources().getInteger(R.integer.ServerPort));
        int res = mStorageMaster.updateDatabaseRequest();
        switch(res){
            case 1:
                Toast.makeText(getApplicationContext(), "No network connection", Toast.LENGTH_SHORT)
                        .show();
                break;
            case 2:
                Toast.makeText(getApplicationContext(),
                        "Database error! Shutting down...", Toast.LENGTH_SHORT).show();

                finish();
                break;
        }

        mMapView = findViewById(R.id.map_view);
        //mMapView = new GLMapView(getApplicationContext());
        mMapView.init();
        mMapPlaceHolder = findViewById(R.id.wait_placeholder_image_view);

        mMapGuide = new MapGuide(getApplicationContext());

        mSourceSearchView = findViewById(R.id.from_search_view);
        mSourceSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                handleFindRequest(mSourceSearchView.getQuery().toString(), mDestSearchView.getQuery().toString());
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                return false;
            }
        });
        //https://github.com/googlesamples/android-ndk/tree/master/bitmap-plasma/app/src/main
        //https://github.com/googlesamples/android-ndk/blob/master/sensor-graph/accelerometer/src/main/cpp/sensorgraph.cpp
        mDestSearchView = findViewById(R.id.to_search_view);
        mDestSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                handleFindRequest(mSourceSearchView.getQuery().toString(), mDestSearchView.getQuery().toString());
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                return false;
            }
        });

        mReverseRouteButton = findViewById(R.id.reverse_route_button);
        mReverseRouteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String from = mSourceSearchView.getQuery().toString();
                String to = mDestSearchView.getQuery().toString();
                if(to.length() > 0){
                    mDestSearchView.setQuery(from, false);
                    mSourceSearchView.setQuery(to,true);
                    mSourceSearchView.requestFocus();
                }
                else if(from.length() > 0 && to.length() == 0){
                    mSourceSearchView.setQuery(to,false);
                    mDestSearchView.setQuery(from, true);
                    mDestSearchView.requestFocus();
                }
                else{

                }
                //handleFindRequest(mSourceSearchView.getQuery().toString(), mDestSearchView.getQuery().toString());
                //mSourceSearchView.clearFocus();
                //mDestSearchView.clearFocus();
            }
        });

        mProgressBar = findViewById(R.id.progressBar);

        mFloorNumberTextView = findViewById(R.id.storey_number_text_view);
        mFloorNumberTextView.setText(
                String.format(getResources().getString(R.string.storey_number_textview), mFloor));
        mSelectedObjTextView = findViewById(R.id.place_name_text_view);

        mGoUpButton = findViewById(R.id.storey_up_button);
        mGoUpButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if( mFloor <  mStoreyRange[1] ){
                    mFloor++;
                    mFloorNumberTextView.setText(
                            String.format(getResources().getString(R.string.storey_number_textview), mFloor));
                    mMapView.changeFloor(mFloor);
                }else{
                    Toast.makeText(getApplicationContext(),
                            getResources().getString(R.string.toast_max_floor_reached), Toast.LENGTH_SHORT)
                            .show();
                }
            }
        });
        mGoDownButton = findViewById(R.id.storey_down_button);
        mGoDownButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if( mFloor > mStoreyRange[0] ){
                    mFloor--;
                    mFloorNumberTextView.setText(
                            String.format(getResources().getString(R.string.storey_number_textview), mFloor));
                    mMapView.changeFloor(mFloor);
                   }else{
                    Toast.makeText(getApplicationContext(),
                            getResources().getString(R.string.toast_min_floor_reached),
                            Toast.LENGTH_SHORT).show();
                }
            }
        });

        /*String path = "/data/data/com.example.foxmap_native_1/databases/MapDB.db";
        SQLiteDatabase db = SQLiteDatabase.openDatabase(path, null, 0);
        if(db.isOpen() == true) {
            Log.d(TAG, "Database opened in Java");
            db.close();
        }else{
            Log.e(TAG, "Cannot open database in Java");
            return;
        }*/
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putInt(mCurrentStoreyKey, mFloor);
    }

    //Создаём кнопки в Тулбаре
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater= getMenuInflater();
        inflater.inflate(R.menu.search_toolbar, menu);
        mUpdateDataItem = menu.findItem(R.id.menu_item_update);

        mUpdateDataItem.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                startDataUpdate();
                mStorageMaster.updateDatabaseRequest();
                endDataUpdate(false);
                return true;
            }
        });

        /*mUpdateDataItem.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                class Updater extends AsyncTask<Void,Void,Void> {

                    @Override
                    protected void onPreExecute() {
                        startDataUpdate();
                    }

                    @Override
                    protected Void doInBackground(Void... voids) {
                        /*new Thread(new Runnable() {
                            @Override
                            public void run() {
                                new NetworkMaster("192.168.1.69", 80);
                            }
                        }).start();
                        Log.d(TAG, "Start starting server");
                        new NetworkMaster("192.168.1.69", 80);
                        Log.d(TAG, "server created");
                        try {
                            Thread.sleep(500);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        return null;
                    }

                    @Override
                    protected void onPostExecute(Void aVoid) {
                        super.onPostExecute(aVoid);
                        endDataUpdate(false);
                    }
                }

                new Updater().execute();
                return false;
            }
        });*/
        return true;
    }

    //Обработка события активации кнопки Тулбара
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return super.onOptionsItemSelected(item);
    }

    private void searchOnMap(String query){
        Toast.makeText(getApplicationContext(),
                String.format(getResources().getString(R.string.toast_object_on_map), query),
                Toast.LENGTH_SHORT).show();
    }
    private void displayRoute(String from, String to){
        Toast.makeText(getApplicationContext(),
                getResources().getString(R.string.toast_path_on_map), Toast.LENGTH_SHORT).show();
    }
    //Графическая реакция на начало обновления карты
    private void startDataUpdate(){

        mProgressBar.setVisibility(View.VISIBLE);
        mMapPlaceHolder.setVisibility(View.VISIBLE);
        //mMapView.setVisibility(View.INVISIBLE);
        Log.d(TAG, "startDataUpdate()");
    }

    //Графическая реакция на окончание обновления карты
    private void endDataUpdate(boolean result){
        Log.d(TAG, "endDataUpdate()");
       mProgressBar.setVisibility(View.GONE);
        mMapPlaceHolder.setVisibility(View.GONE);
        mMapView.setVisibility(View.VISIBLE);
        String resultMessage;
        if(result == true){
            resultMessage = getResources().getString(R.string.toast_map_syncronized);
        }else{
            resultMessage = getResources().getString(R.string.toast_failed_map_update);
        }
        Toast.makeText(getApplicationContext(), resultMessage, Toast.LENGTH_SHORT).show();

    }

    void handleFindRequest(String from, String to){
        if(from.length() > 0 && to.length() > 0){
            displayRoute(from, to);
            if(!mMapGuide.buildRoute(from, to)){
                Toast.makeText(getApplicationContext(), "Cannot find route, sorre!",
                        Toast.LENGTH_SHORT).show();
            }else{
                boolean res = mMapView.drawRouteRequest();
                if(res)
                    Log.d(TAG, "Маршрут построен");
                else{
                    Log.e(TAG, "Маршрут не найден");
                    Toast.makeText(getApplicationContext(), "Маршрут не найден. А оно вам надо?",
                            Toast.LENGTH_SHORT);
                }
            }
        }else{
            String target = from.length() > 0 ? from : to;
            searchOnMap(target);
            if(!mMapGuide.findOnMap(target)){
                Toast.makeText(getApplicationContext(), "Cannot find object, sorre!",
                        Toast.LENGTH_SHORT).show();
            }else{
                boolean res = mMapView.drawObjectMarkerRequest();
                if(res){
                    Log.d(TAG, "Обьект найден");
                }else{
                    Log.e(TAG, "Обьект не найден");
                    Toast.makeText(getApplicationContext(), "Здесь таких нет", Toast.LENGTH_SHORT);
                }
            }
        }
    }

    //  Вызывается автоматически при уходе Activity с первого плана
    // (переворот, переключение приложения)
    @Override
    protected void onPause() {
        super.onPause();
        mMapView.onPause();
    }

    //  Вызывается автоматически при появлении Activity на переднем плане
    // (переворот, запуск приложения)
    @Override
    protected void onResume() {
        super.onResume();
        mMapView.onResume();
    }

    // Проверка подержки смартфоном версии OpenGL ES не меньше 2
    private boolean supportES3() {
        ActivityManager activityManager =
                (ActivityManager) getSystemService(getApplicationContext().ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        return (configurationInfo.reqGlEsVersion >= 0x30000);
    }

    public void onButtonClicked() {

    }

    public int[] getStoreyRange(){
        return mStoreyRange;
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "Destroying...");
        ConstraintLayout layout =  findViewById(R.id.layout);
        layout.removeView(mMapView);
        super.onDestroy();
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "Stopping");
        ConstraintLayout layout =  findViewById(R.id.layout);
        layout.removeView(mMapView);
        super.onStop();
    }

    @Override
    protected void onStart() {
        Log.d(TAG, "Starting...");
        ConstraintLayout layout =  findViewById(R.id.layout);
        try{
            layout.addView(mMapView);
        }catch (Exception e){
            Log.d(TAG, "Cannot add view");
        }
        super.onStart();
    }
}

