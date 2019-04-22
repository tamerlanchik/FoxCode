package com.example.foxmap_native_1;

import org.junit.Before;
import org.junit.Test;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.core.Is.is;

public class TestMapItemTest {
    private TestMapItem mSubject;
    @Before
    public void setUp() throws Exception {
        mSubject = new TestMapItem();
    }

    @Test
    public void testTest() {
        assertThat(mSubject.test(5), is(5*2));
    }
}