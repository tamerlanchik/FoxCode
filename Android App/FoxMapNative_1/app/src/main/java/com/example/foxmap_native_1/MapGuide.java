package com.example.foxmap_native_1;

import android.content.Context;

import java.util.List;

public class MapGuide {
    private ItemsStorageSinglet mMapItems;
    static{
        System.loadLibrary("map_guide");
    }

    public MapGuide(Context context){
        mMapItems = ItemsStorageSinglet.get(context);
    }

    //Получает имена обьектов начала и конца пути
    //Возвращает список имён узлов - обьектов, которые надо посетить
    //ACHTUNG!! Как соединить хранилища на Java и С++?
    public native List<String> getRoute(String from, String to);
}
