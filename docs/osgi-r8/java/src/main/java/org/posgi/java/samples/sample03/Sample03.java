package org.posgi.java.samples.sample03;

import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.ServiceLoader;

import org.osgi.framework.Bundle;
import org.osgi.framework.Constants;
import org.osgi.framework.launch.Framework;
import org.osgi.framework.launch.FrameworkFactory;
import org.posgi.java.utils.SyntheticBundleBuilder;

public final class Sample03 {
    public static void main(String[] args) throws Exception {
        ServiceLoader<FrameworkFactory> loader = ServiceLoader.load(FrameworkFactory.class);
        FrameworkFactory factory = loader.iterator().next();
        Map<String, String> config = new HashMap<>();
        config.put(Constants.FRAMEWORK_STORAGE_CLEAN_ONFIRSTINIT, "true");
        System.out.println("Starting framework...");
        Framework framework = factory.newFramework(config);
        framework.start();
        System.out.println(framework);
        SyntheticBundleBuilder builder = new SyntheticBundleBuilder();
        builder
            .bundleSymbolicName("BundleA")
            .bundleVersion("1.0.0")
            .addManifestHeader(Constants.BUNDLE_ACTIVATOR, BundleA.class.getName())
            .addManifestHeader(Constants.IMPORT_PACKAGE, "org.osgi.framework")
            .addFile("org/posgi/java/samples/sample02/BundleA.class", 
                new File("./target/classes/org/posgi/java/samples/sample03/BundleA.class"))
            .asFile("./target/BundleA.jar");
        Bundle bundleA = framework.getBundleContext().installBundle("test:BundleA", 
            new File("./target/BundleA.jar").toURI().toURL().openStream());
        System.out.println("BundleA: " + bundleA + " state: " + bundleA.getState());

        bundleA.start();
        System.out.println("BundleA: " + bundleA + " state: " + bundleA.getState());
        Thread.sleep(1000);
        bundleA.stop();
        System.out.println("BundleA: " + bundleA + " state: " + bundleA.getState());

        framework.stop();
        framework.waitForStop(0);
        System.out.println("Stopped framework...");
    }
}
