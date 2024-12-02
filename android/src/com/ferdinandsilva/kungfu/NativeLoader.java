package com.ferdinandsilva.kungfu; 
public class NativeLoader extends android.app.NativeActivity { 
    static {
        System.loadLibrary("kungfu");
    } 
} 
