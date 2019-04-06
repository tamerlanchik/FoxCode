package com.example.foxmap_native_1;

import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
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
import android.widget.Toast;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    MenuItem mUpdateDataItem;
    ProgressBar mProgressBar;

    android.widget.SearchView mDestSearchView;
    android.widget.SearchView mSourceSearchView;
    MapDrawerJNI mMapDrawerJNI;
    GLMapView mMapView;
    ImageView mMapPlaceHolder;
    ImageButton mReverseRouteButton;

    /*private class PingServer extends AsyncTask<Void,Void,Void> {

        @Override
        protected Void doInBackground(Void... params) {
            try{
                String result = new DataCollector()
                        .getUrlString("https://www.bignerdranch.com");
                Log.i(TAG, "Fetched contents of URL: " + result);
            } catch (IOException ioe) {
                Log.e(TAG, "Failed to fetch URL: ", ioe);
            }
            return null;
        }
    }
*/
    static {
        //System.loadLibrary("native-lib");
    }

    //private class
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mMapView = findViewById(R.id.map_view);
        mMapView.init();
        mMapPlaceHolder = findViewById(R.id.wait_placeholder_image_view);
        //mMapDrawer = new MapDrawer(getApplicationContext(), mMapView);

        /*fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mSearchView.setIconified(false);
            }
        });*/

        mSourceSearchView = findViewById(R.id.from_search_view);
        mSourceSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                if(mDestSearchView.getQuery().length() > 0){
                    displayRoute(query, mDestSearchView.getQuery().toString());
                }else{
                    searchOnMap(query);
                }
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
                if(mSourceSearchView.getQuery().length() > 0){
                    displayRoute(mSourceSearchView.getQuery().toString(), query);
                }else{
                    searchOnMap(query);
                }
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
                CharSequence temp = mDestSearchView.getQuery();
                mDestSearchView.setQuery(mSourceSearchView.getQuery(), true);
                mSourceSearchView.setQuery(temp,false);
                //Второй параметр у setQuery() отвечает, вызывать ли
                //обработчик отправки запроса у этого SearchView (как если бы мы его нажали)
                // Вызываем только у одного из них (что вполне логично - запрос-то лишь один)
            }
        });

        mProgressBar = findViewById(R.id.progressBar);

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
                class Updater extends AsyncTask<Void,Void,Void> {

                    @Override
                    protected void onPreExecute() {
                        startDataUpdate();
                    }

                    @Override
                    protected Void doInBackground(Void... voids) {
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
        });

        /*mSearchView = (SearchView) mSearchItem.getActionView();
        mSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String s) {
                Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();

                //new PingServer().execute();
                return false;
            }

            @Override
            public boolean onQueryTextChange(String s) {
                Log.d("TAG", "onQueryTextChange");
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
        Toast.makeText(getApplicationContext(), "Looking for " + query, Toast.LENGTH_SHORT).show();
    }
    private void displayRoute(String from, String to){
        Toast.makeText(getApplicationContext(), "Generating a route...", Toast.LENGTH_SHORT).show();
    }
    //Графическая реакция на начало обновления карты
    private void startDataUpdate(){

        mProgressBar.setVisibility(View.VISIBLE);
        mMapPlaceHolder.setVisibility(View.VISIBLE);
        mMapView.setVisibility(View.GONE);
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
            resultMessage = getResources().getString(R.string.success_map_updated);
        }else{
            resultMessage = getResources().getString(R.string.cannot_update_map);
        }
        Toast.makeText(getApplicationContext(), resultMessage, Toast.LENGTH_SHORT).show();

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
}

