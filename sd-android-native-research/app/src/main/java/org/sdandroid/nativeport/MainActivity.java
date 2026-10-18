package org.sdandroid.nativeport;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class MainActivity extends Activity {
    private static final String TAG = "SDNativeResearch";
    private GLSurfaceView view;

    static { System.loadLibrary("sdnative"); }
    private static native void nativeInit();
    private static native void nativeResize(int width, int height);
    private static native void nativeRender(float seconds);
    private static native void nativeTouch(float x, float y, int action);

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        Log.i(TAG, "Starting native Android rendering smoke test");
        view = new GLSurfaceView(this);
        view.setEGLContextClientVersion(3);
        view.setPreserveEGLContextOnPause(true);
        view.setRenderer(new GLSurfaceView.Renderer() {
            private final long start = System.nanoTime();
            @Override public void onSurfaceCreated(GL10 gl, EGLConfig config) { nativeInit(); }
            @Override public void onSurfaceChanged(GL10 gl, int width, int height) { nativeResize(width, height); }
            @Override public void onDrawFrame(GL10 gl) { nativeRender((System.nanoTime() - start) / 1_000_000_000.0f); }
        });
        view.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        view.setOnTouchListener((v, event) -> {
            nativeTouch(event.getX(), event.getY(), event.getActionMasked());
            return true;
        });
        setContentView(view);
    }

    @Override protected void onPause() {
        if (view != null) view.onPause();
        super.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        if (view != null) view.onResume();
    }
}
