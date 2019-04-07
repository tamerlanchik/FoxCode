package com.example.foxmap_native_1;

import android.content.res.Resources;
import android.support.test.espresso.UiController;
import android.support.test.espresso.ViewAction;
import android.support.test.espresso.ViewInteraction;
import android.support.test.espresso.action.ViewActions;
import android.support.test.espresso.matcher.ViewMatchers;
import android.support.test.rule.ActivityTestRule;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;
import android.view.View;
import android.widget.AutoCompleteTextView;
import android.widget.EditText;
import android.widget.SearchView;
import android.widget.TextView;

import org.hamcrest.Matcher;
import org.hamcrest.Matchers;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import static android.support.test.espresso.Espresso.onData;
import static android.support.test.espresso.Espresso.onView;
import static android.support.test.espresso.action.ViewActions.click;
import static android.support.test.espresso.action.ViewActions.pressImeActionButton;
import static android.support.test.espresso.action.ViewActions.typeText;
import static android.support.test.espresso.assertion.ViewAssertions.matches;
import static android.support.test.espresso.matcher.RootMatchers.withDecorView;
import static android.support.test.espresso.matcher.ViewMatchers.isAssignableFrom;
import static android.support.test.espresso.matcher.ViewMatchers.isDisplayed;
import static android.support.test.espresso.matcher.ViewMatchers.withEffectiveVisibility;
import static android.support.test.espresso.matcher.ViewMatchers.withHint;
import static android.support.test.espresso.matcher.ViewMatchers.withId;
import static android.support.test.espresso.matcher.ViewMatchers.withParent;
import static android.support.test.espresso.matcher.ViewMatchers.withText;
import static org.hamcrest.CoreMatchers.anything;
import static org.hamcrest.CoreMatchers.containsString;
import static org.hamcrest.CoreMatchers.endsWith;
import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.isA;
import static org.hamcrest.CoreMatchers.startsWith;
import static org.hamcrest.Matchers.greaterThan;
import static org.hamcrest.Matchers.hasToString;
import static org.hamcrest.Matchers.instanceOf;
import static org.hamcrest.Matchers.not;
import static org.hamcrest.Matchers.notNullValue;
import static org.hamcrest.core.AllOf.allOf;
import static org.hamcrest.core.AnyOf.anyOf;
import static org.junit.Assert.assertThat;

@RunWith(AndroidJUnit4.class)
public class MainActivityTest {
    private static final int mToastShortTimeout = 2000;
    @Rule
    public ActivityTestRule<MainActivity> mActivityRule = new ActivityTestRule<>(MainActivity.class);

    @Before
    public void setUp() throws Exception {
    }


    //@Test
    //  Тестирование кнопки обновления карты (начало)
    public void clickUpdateMap(){
        onView(withId(R.id.menu_item_update)).perform(click());
        onView(withId(R.id.wait_placeholder_image_view)).check(matches(anything()));
    }

    //@Test
    //  Тестирование переключения этажей
    public void checkStoreyNavigationTextView() {
        Resources resources = mActivityRule.getActivity().getResources();
        TextView mTextView = mActivityRule.getActivity().findViewById(R.id.storey_number_text_view);
        String tw_cargo[] = mTextView.getText().toString().split(" ");
        int currentFloor = -1;
        currentFloor = Integer.parseInt(tw_cargo[0]);
        int storeyRange[] = mActivityRule.getActivity().getStoreyRange();

        //  До верхней границы
        for(; ++currentFloor <= storeyRange[1];){
            onView(withId(R.id.storey_up_button)).perform(click());
            onView(withText(
                    String.format(
                            resources.getString(R.string.storey_number_textview),
                            currentFloor)))
                    .check(matches(anything()));
        }
        //Проверка выхода за верхнюю границу
        onView(withId(R.id.storey_up_button)).perform(click());
        onView(withText(resources.getString(R.string.toast_max_floor_reached))).
                inRoot(withDecorView(
                        not(is(mActivityRule.getActivity().
                                getWindow().getDecorView())))).
                check(matches(isDisplayed()));

        //  До нижней границы
        currentFloor--;
        for(; --currentFloor >= storeyRange[0];){
            onView(withId(R.id.storey_down_button)).perform(click());
            onView(withText(
                    String.format(
                            resources.getString(R.string.storey_number_textview),
                            currentFloor)))
                    .check(matches(anything()));
        }

        //Проверка выхода за верхнюю границу
        onView(withId(R.id.storey_down_button)).perform(click());
        onView(withText(resources.getString(R.string.toast_min_floor_reached))).
                inRoot(withDecorView(
                        not(is(mActivityRule.getActivity().
                                getWindow().getDecorView())))).
                check(matches(isDisplayed()));
    }

    private class SearchViewSetQuery implements ViewAction{
        String mText;
        boolean mIsActivate;
        SearchViewSetQuery(String text, boolean isActivate){
            mText = text;
            mIsActivate = isActivate;
        }

        @Override
        public Matcher<View> getConstraints() {
            return allOf(isDisplayed(), isAssignableFrom(SearchView.class));
        }

        @Override
        public String getDescription() {
            return "Change view text";
        }

        @Override
        public void perform(UiController uiController, View view) {
            //((SearchView)view).setIconified(false);
            ((SearchView)view).setQuery(mText, mIsActivate);
            //((SearchView)view).clearFocus();
        }
    }

    private class SearchViewClear implements ViewAction{

        @Override
        public Matcher<View> getConstraints() {
            return allOf(isDisplayed(), isAssignableFrom(SearchView.class));
        }

        @Override
        public String getDescription() {
            return "Change view text";
        }

        @Override
        public void perform(UiController uiController, View view) {
            ((SearchView)view).setQuery("", false);
            //((SearchView)view).setIconified(true);
            //((SearchView)view).clearFocus();
        }
    }

    //@Test
    public void checkSearchViews(){
        String a = "503";
        String b = "Хавальня";
        Resources resources = mActivityRule.getActivity().getResources();

        /*
        Алгоритм:
            заполняем один или два SearchView запросом, отправляем его, регистрируем Toast,
            ждем время, пока сообщение не исчезнет, и очищаем SearchView
         */

        //  Тестируем отправку одной строки
        onView(withId(R.id.from_search_view)).perform(new SearchViewSetQuery(a, true));
        checkToast(String.format(resources.getString(R.string.toast_object_on_map), a));
        try {
            Thread.sleep(2500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        onView(withId(R.id.from_search_view)).perform(new SearchViewClear());
        onView(withId(R.id.to_search_view)).perform(new SearchViewClear());

        // Тестируем отправку двух строк в порядке: от From до To. Вызывает To
        onView(withId(R.id.from_search_view)).perform(new SearchViewSetQuery(a, false));
        onView(withId(R.id.to_search_view)).perform(new SearchViewSetQuery(b, true));
        checkToast(resources.getString(R.string.toast_path_on_map));
        try {
            Thread.sleep(2500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        onView(withId(R.id.from_search_view)).perform(new SearchViewClear());
        onView(withId(R.id.to_search_view)).perform(new SearchViewClear());

        // Тестируем отправку двух строк в порядке: от To до From. Вызывает From
        onView(withId(R.id.to_search_view)).perform(new SearchViewSetQuery(a, false));
        onView(withId(R.id.from_search_view)).perform(new SearchViewSetQuery(b, true));
        checkToast(resources.getString(R.string.toast_path_on_map));

        try {
            Thread.sleep(2500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        onView(withId(R.id.from_search_view)).perform(new SearchViewClear());
        onView(withId(R.id.to_search_view)).perform(new SearchViewClear());
    }

    @Test
    public void checkReverseButton() {
        String a = "503";
        String b = "Хавальня";
        Resources resources = mActivityRule.getActivity().getResources();
        SearchView mFrom = mActivityRule.getActivity().findViewById(R.id.from_search_view);
        SearchView mTo = mActivityRule.getActivity().findViewById(R.id.to_search_view);
        try{
        //  Реверс без ввода
            onView(withId(R.id.reverse_route_button)).perform(click());
            assert(mFrom.getQuery() == "" && mTo.getQuery() == "");

        //  Ввод в левое поле, потом реверс
            onView(withId(R.id.from_search_view)).perform(new SearchViewSetQuery(a, true));

            checkToast(String.format(resources.getString(R.string.toast_object_on_map), a));

            Thread.sleep(mToastShortTimeout);
            onView(withId(R.id.reverse_route_button)).perform(click());
            assert(mTo.getQuery().toString() == a);

            checkToast(String.format(resources.getString(R.string.toast_object_on_map), a));
            Thread.sleep(mToastShortTimeout);


        //  Ввод в правое поле, потом реверс
            onView(withId(R.id.to_search_view)).perform(new SearchViewSetQuery(b, true));

            checkToast(String.format(resources.getString(R.string.toast_object_on_map), b));
            Thread.sleep(mToastShortTimeout);

            onView(withId(R.id.reverse_route_button)).perform(click());

            assert(mFrom.getQuery().toString() == b);

            checkToast(String.format(resources.getString(R.string.toast_object_on_map), b));
            Thread.sleep(mToastShortTimeout);


        //  Ввод в два поля, потом реверс
            onView(withId(R.id.from_search_view)).perform(new SearchViewSetQuery(a, true));
            checkToast(String.format(resources.getString(R.string.toast_object_on_map), a));
            Thread.sleep(mToastShortTimeout);
            onView(withId(R.id.to_search_view)).perform(new SearchViewSetQuery(b, true));
            checkToast(resources.getString(R.string.toast_path_on_map));
            Thread.sleep(mToastShortTimeout);

            onView(withId(R.id.reverse_route_button)).perform(click());

            assert(mTo.getQuery().toString() == a && mFrom.getQuery() == b);

            checkToast(resources.getString(R.string.toast_path_on_map));
            Thread.sleep(mToastShortTimeout);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public void checkToast(String message){
        onView(withText(message)).
                inRoot(withDecorView(
                        not(is(mActivityRule.getActivity().
                                getWindow().getDecorView())))).
                check(matches(isDisplayed()));
    }


}