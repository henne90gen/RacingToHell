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

    public GLSurfaceViewWrapper(Context context, boolean isProbablyEmulator) {
        super(context);
//        setEGLContextFactory(new EGLContextFactory() {
//            @Override
//            public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
//                final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
//                int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE};
//                return egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
//            }
//
//            @Override
//            public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
//                egl.eglDestroyContext(display, context);
//            }
//        });
        setEGLContextClientVersion(2);

        if (isProbablyEmulator) {
            // Avoids crashes on startup with some emulator images.
            setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        }

        setRenderer(new RendererWrapper(context));
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        boolean pressed = true;
        if (e.getAction() == MotionEvent.ACTION_UP) {
            pressed = false;
        }
        NativeWrapper.on_touch_event(e.getX(), e.getY(), pressed);
        return true;
    }
}
