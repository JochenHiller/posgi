package org.posgi.java.samples.sample03;

import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;

public class BundleA implements BundleActivator {

    @Override
    public void start(BundleContext context) throws Exception {
        System.out.println("BundleA started");
    }

    @Override
    public void stop(BundleContext context) throws Exception {
        System.out.println("BundleA stopped");
    }
    
}
