package com.gibbon.tools.world;

import com.gibbon.radakan.error.ErrorReporter;
import java.lang.Thread.UncaughtExceptionHandler;

public class EditorExceptionHandler implements UncaughtExceptionHandler {
    public void uncaughtException(Thread t, Throwable e) {
        ErrorReporter.reportError("Unchecked exception located!", e);
    }
}
