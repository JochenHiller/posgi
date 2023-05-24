package org.posgi.java.samples.sample03;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertNotNull;

class Sample03Test {

    @Test
    void testBundleConstructor() {
        BundleA bundle = new BundleA();
        assertNotNull(bundle);
    }
}
