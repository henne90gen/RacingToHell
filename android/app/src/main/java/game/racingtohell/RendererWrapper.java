package game.racingtohell;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class RendererWrapper implements GLSurfaceView.Renderer {

    private final Context context;
    private long startTime;
    private long endTime;

    public RendererWrapper(Context context) {
        this.context = context;
        startTime = 0;
        endTime = 0;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeWrapper.init_asset_manager(context.getAssets());
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeWrapper.on_surface_changed(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
//        endTime = System.nanoTime();
//        long timeDelta = endTime - startTime;
//        if (timeDelta < 33000000)
//            try {
//                Thread.sleep(33000000 - timeDelta);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//        startTime = System.nanoTime();
//        Log.i("RacingToHell", "");
        NativeWrapper.on_draw_frame();
    }
}
