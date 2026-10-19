package org.sdandroid.nativeport;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.ParcelFileDescriptor;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class MainActivity extends Activity {
    private static final String TAG = "SDNativeResearch";
    private static final int PICK_ARCHIVE = 41;
    private GLSurfaceView view;
    private TextView status;

    static { System.loadLibrary("sdnative"); }
    private static native void nativeInit();
    private static native void nativeResize(int width, int height);
    private static native void nativeRender(float seconds);
    private static native void nativeTouch(float x, float y, int action);
    private static native boolean nativeCheckPrivateFile(String path);
    private static native String nativeInspectFileDescriptor(int fd, long fileSize, String displayName);

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        runPrivateFileIoTest();

        FrameLayout root = new FrameLayout(this);
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
        root.addView(view, new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

        Button pick = new Button(this);
        pick.setText("Inspect .BIG / .BIX file");
        FrameLayout.LayoutParams buttonParams = new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT,
                Gravity.TOP | Gravity.CENTER_HORIZONTAL);
        buttonParams.topMargin = dp(18);
        root.addView(pick, buttonParams);
        pick.setOnClickListener(v -> openArchivePicker());

        ScrollView panel = new ScrollView(this);
        status = new TextView(this);
        status.setTextColor(0xFFFFFFFF);
        status.setTextSize(13);
        status.setPadding(dp(12), dp(10), dp(12), dp(10));
        status.setText("Native GLES3 renderer active.\\nChoose a .BIG or .BIX file to inspect its first 64 bytes.\\nThis reads only the header; it does not extract or decode assets.");
        panel.addView(status);
        panel.setBackgroundColor(0xCC101820);
        FrameLayout.LayoutParams panelParams = new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, dp(210), Gravity.BOTTOM);
        root.addView(panel, panelParams);
        setContentView(root);
    }

    private int dp(int value) {
        return Math.round(value * getResources().getDisplayMetrics().density);
    }

    private void openArchivePicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        intent.putExtra(Intent.EXTRA_MIME_TYPES, new String[] {
                "application/octet-stream", "application/x-binary"
        });
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        startActivityForResult(intent, PICK_ARCHIVE);
    }

    @Override protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode != PICK_ARCHIVE || resultCode != RESULT_OK || data == null || data.getData() == null) return;
        Uri uri = data.getData();
        try (ParcelFileDescriptor pfd = getContentResolver().openFileDescriptor(uri, "r")) {
            if (pfd == null) throw new IOException("Android did not provide a readable file descriptor");
            String name = uri.getLastPathSegment();
            try {
                android.database.Cursor cursor = getContentResolver().query(uri,
                        new String[] { android.provider.OpenableColumns.DISPLAY_NAME }, null, null, null);
                if (cursor != null) {
                    try {
                        if (cursor.moveToFirst()) name = cursor.getString(0);
                    } finally { cursor.close(); }
                }
            } catch (Exception ignored) { }
            String report = nativeInspectFileDescriptor(pfd.getFd(), pfd.getStatSize(), name == null ? "selected file" : name);
            status.setText(report);
            Log.i(TAG, report);
        } catch (Exception e) {
            String message = "Could not inspect selected file: " + e.getMessage();
            status.setText(message);
            Log.e(TAG, message, e);
        }
    }

    private void runPrivateFileIoTest() {
        File testFile = new File(getFilesDir(), "sd-native-private-io-test.txt");
        byte[] data = "SD_NATIVE_PRIVATE_IO_OK".getBytes(StandardCharsets.UTF_8);
        try (FileOutputStream out = new FileOutputStream(testFile)) {
            out.write(data);
            out.flush();
            boolean passed = nativeCheckPrivateFile(testFile.getAbsolutePath());
            Log.i(TAG, "Native app-private file I/O test: " + (passed ? "PASS" : "FAIL"));
        } catch (IOException e) {
            Log.e(TAG, "Could not prepare app-private file I/O test", e);
        }
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
