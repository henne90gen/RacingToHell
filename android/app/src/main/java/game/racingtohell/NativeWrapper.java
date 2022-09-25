package game.racingtohell;

import android.content.res.AssetManager;

public class NativeWrapper {

    static {
        System.loadLibrary("native-lib");
    }

    public static native void init_asset_manager(AssetManager assetManager);

    public static native void on_surface_changed(int width, int height);

    public static native void on_draw_frame();

    public static native void on_touch_event(float x, float y, boolean pressed);
}
