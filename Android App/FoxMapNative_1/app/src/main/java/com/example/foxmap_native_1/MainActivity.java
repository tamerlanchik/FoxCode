package com.example.foxmap_native_1;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.SearchView;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    MenuItem mUpdateDataItem;
    ProgressBar mProgressBar;

    android.widget.SearchView mSearchView1;
    android.widget.SearchView mSearchView2;
    MapDrawer mMapDrawer;
    ImageView mMapImageView;

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

        mMapImageView = findViewById(R.id.map_image_view);
        mMapDrawer = new MapDrawer(getApplicationContext(), mMapImageView);

        /*fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mSearchView.setIconified(false);
            }
        });*/

        mSearchView1 = findViewById(R.id.to_search_view);
        mSearchView2 = findViewById(R.id.from_search_view);
        /*mSearchButton = (Button) findViewById(R.id.search_button);
        mSearchButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mMapDrawer.DrawRoute();
                Log.d(TAG, "Find route request");
                String a = mSearchView1.getQuery().toString();
                String b = mSearchView2.getQuery().toString();
                if(b.length() == 0){
                    Log.d(TAG, "Look for smth");
                }else{
                    Log.d(TAG, "Find route request");
                }
            }
        });*/

        mProgressBar = findViewById(R.id.progressBar);

    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater= getMenuInflater();
        inflater.inflate(R.menu.search_toolbar, menu);
        mUpdateDataItem = menu.findItem(R.id.menu_item_update);

        mUpdateDataItem.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                mProgressBar.setVisibility(View.VISIBLE);
                //call NetworkMaster
                boolean updateResult = false;
                mProgressBar.setVisibility(View.INVISIBLE);
                String resultMessage;
                if(updateResult == true){
                    resultMessage = getResources().getString(R.string.success_map_updated);
                }else{
                    resultMessage = getResources().getString(R.string.cannot_update_map);
                }
                Toast.makeText(getApplicationContext(), resultMessage, Toast.LENGTH_SHORT).show();
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

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return super.onOptionsItemSelected(item);
    }
}

