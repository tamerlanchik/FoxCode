package com.example.foxmap_native_1;

import android.support.test.espresso.ViewAction;
import android.support.test.espresso.action.ViewActions;
import android.support.test.espresso.matcher.ViewMatchers;
import android.support.test.rule.ActivityTestRule;
import android.support.test.runner.AndroidJUnit4;
import android.widget.AutoCompleteTextView;
import android.widget.EditText;
import android.widget.TextView;

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
import static android.support.test.espresso.matcher.ViewMatchers.withId;
import static android.support.test.espresso.matcher.ViewMatchers.withText;
import static org.hamcrest.CoreMatchers.anything;
import static org.hamcrest.CoreMatchers.containsString;
import static org.hamcrest.CoreMatchers.endsWith;
import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.startsWith;
import static org.hamcrest.Matchers.greaterThan;
import static org.hamcrest.Matchers.hasToString;
import static org.hamcrest.Matchers.instanceOf;
import static org.hamcrest.Matchers.not;
import static org.hamcrest.Matchers.notNullValue;
import static org.hamcrest.core.AllOf.allOf;
import static org.junit.Assert.assertThat;

@RunWith(AndroidJUnit4.class)
public class MainActivityTest {
    @Rule
    public ActivityTestRule<MainActivity> mActivityRule = new ActivityTestRule<>(MainActivity.class);

    @Before
    public void setUp() throws Exception {
    }


    //@Test
    public void clickUpdateMap(){
        onView(withId(R.id.menu_item_update)).perform(click());
        onView(withId(R.id.wait_placeholder_image_view)).check(matches(anything()));
    }

    @Test
    public void checkStoreyNavigationTextView() {
        TextView mTextView = mActivityRule.getActivity().findViewById(R.id.storey_number_text_view);
        String tw_cargo[] = mTextView.getText().toString().split(" ");
        int currentFloor = -1;
        currentFloor = Integer.parseInt(tw_cargo[0]);
        int storeyRange[] = mActivityRule.getActivity().getStoreyRange();

        //  До верхней границы
        for(int i = currentFloor; i <= storeyRange[1]; i++){
            onView(withId(R.id.storey_up_button)).perform(click());
            onView(withText(Integer.toString(currentFloor+i) + " Этаж")).check(matches(anything()));
        }
        onView(withId(R.id.storey_up_button)).perform(click());
        /*onView(withText()).
                inRoot(withDecorView(
                        not(is(mActivityRule.getActivity().
                                getWindow().getDecorView())))).
                check(matches(isDisplayed()));*/
    }

    @Test
    public void checkToast(){
        onView(allOf(withId(R.id.from_search_view),
                withEffectiveVisibility(ViewMatchers.Visibility.VISIBLE))).perform(
                click());
        //onView(withId(R.id.from_search_view)).perform(click());
        double a = 5435;
        for(int i = 0; i < 10000; i++){
            for(int j = 0; j < 100000; j++){
                a /= 3;
            }
        }
        onView(withId(android.support.design.R.id.search_button)).perform(typeText("sd"), pressImeActionButton());
        /*double a = 5435;
        for(int i = 0; i < 10000; i++){
            for(int j = 0; j < 100000; j++){
                a /= 3;
            }
        }
        onView(withId(R.id.from_search_view))
                .perform(typeText("1"), ViewActions.closeSoftKeyboard());
        a = 5435;
        for(int i = 0; i < 10000; i++){
            for(int j = 0; j < 1000; j++){
                a /= 3;
            }
        }
        onView(withText("Looking for " + "1")).
        inRoot(withDecorView(
                not(is(mActivityRule.getActivity().
                        getWindow().getDecorView())))).
                check(matches(isDisplayed()));*/

    }
}