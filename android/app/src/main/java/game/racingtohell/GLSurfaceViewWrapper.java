package game.racingtohell;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

public class GLSurfaceViewWrapper extends GLSurfaceView {

    public GLSurfaceViewWrapper(Context context) {
        super(context);
        setEGLContextClientVersion(3);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        boolean pressed = e.getAction() != MotionEvent.ACTION_UP;
        NativeWrapper.on_touch_event(e.getX(), e.getY(), pressed);
        return true;
    }
}
