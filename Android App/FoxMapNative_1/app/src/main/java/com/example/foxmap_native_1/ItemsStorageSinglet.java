package com.example.foxmap_native_1;

import android.content.Context;

import java.util.ArrayList;
import java.util.List;

//Класс-хранилище объектов карты.
//Синглетный - значит, существует только один экземпляр, привязанный к приложению, а не к активити
//(время жизни этого экземпляра совпадает со временем жизни приложения, т.е. оно будет жить всегда, пока нужно)
public class ItemsStorageSinglet {
    private static ItemsStorageSinglet sStorage;

    private List<MapItem> mItems;   //Должен быть граф

    public static ItemsStorageSinglet get(Context context) {
        if( sStorage == null) {
            sStorage = new ItemsStorageSinglet(context);
        }
        return sStorage;
    }

    private ItemsStorageSinglet(Context context) {

    }
}
