package game.racingtohell;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

// OpenGL android code was taken from:
// http://www.learnopengles.com/calling-opengl-from-android-using-the-ndk/
public class MainActivity extends Activity {

    private GLSurfaceView glSurfaceView = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

        final boolean supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000 || isProbablyEmulator();

        Log.i("RacingToHell", "OpenGL Version: " + configurationInfo.getGlEsVersion());
        if (!supportsEs3) {
            // Should never be seen in production, since the manifest filters
            // unsupported devices.
            Toast.makeText(this, "This device does not support OpenGL ES 3.0.",
                Toast.LENGTH_LONG).show();
            return;
        }

        glSurfaceView = new GLSurfaceViewWrapper(this);
        if (isProbablyEmulator()) {
            // Avoids crashes on startup with some emulator images.
            glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        }
        glSurfaceView.setRenderer(new RendererWrapper(this));
        setContentView(glSurfaceView);
    }

    private boolean isProbablyEmulator() {
        return Build.FINGERPRINT.startsWith("generic")
            || Build.FINGERPRINT.startsWith("unknown")
            || Build.MODEL.contains("google_sdk")
            || Build.MODEL.contains("Emulator")
            || Build.MODEL.contains("Android SDK built for x86");
    }

    @Override
    protected void onPause() {
        super.onPause();

        if (glSurfaceView != null) {
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (glSurfaceView != null) {
            glSurfaceView.onResume();
        }
    }
}