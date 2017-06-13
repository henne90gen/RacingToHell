package game.racingtohell;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class GLSurfaceViewWrapper extends GLSurfaceView {

    public GLSurfaceViewWrapper(Context context) {
        super(context);
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
