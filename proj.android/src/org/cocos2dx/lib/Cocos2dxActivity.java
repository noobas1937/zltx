/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

import android.graphics.drawable.Drawable;
import android.opengl.Visibility;
import android.util.TypedValue;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import org.cocos2dx.lib.Cocos2dxHelper.Cocos2dxHelperListener;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.os.Message;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.opengl.GLSurfaceView;
import com.elex.chatservice.view.ICocos2dxScreenLockListener;

public abstract class Cocos2dxActivity extends Activity implements Cocos2dxHelperListener, ICocos2dxScreenLockListener {
    // ===========================================================
    // Constants
    // ===========================================================

    private final static String TAG = Cocos2dxActivity.class.getSimpleName();

    // ===========================================================
    // Fields
    // ===========================================================
    
    private Cocos2dxGLSurfaceView mGLSurfaceView = null;
    private int[] mGLContextAttrs = null;
    private Cocos2dxHandler mHandler = null;   
    private static Cocos2dxActivity sContext = null;
    private Cocos2dxVideoHelper mVideoHelper = null;
    private Cocos2dxWebViewHelper mWebViewHelper = null;

    public class Cocos2dxEGLConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        protected int[] configAttribs;
        public Cocos2dxEGLConfigChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize)
        {
            configAttribs = new int[] {redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize};
        }
        public Cocos2dxEGLConfigChooser(int[] attribs)
        {
            configAttribs = attribs;
        }
        
        public EGLConfig selectConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs, int[] attribs)
        {
            for (EGLConfig config : configs) {
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,
                        EGL10.EGL_STENCIL_SIZE, 0);
                if ((d >= attribs[4]) && (s >= attribs[5])) {
                    int r = findConfigAttrib(egl, display, config,
                            EGL10.EGL_RED_SIZE, 0);
                    int g = findConfigAttrib(egl, display, config,
                             EGL10.EGL_GREEN_SIZE, 0);
                    int b = findConfigAttrib(egl, display, config,
                              EGL10.EGL_BLUE_SIZE, 0);
                    int a = findConfigAttrib(egl, display, config,
                            EGL10.EGL_ALPHA_SIZE, 0);
                    if ((r >= attribs[0]) && (g >= attribs[1])
                            && (b >= attribs[2]) && (a >= attribs[3])) {
                        return config;
                    }
                }
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {
            int[] value = new int[1];
            if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
                return value[0];
            }
            return defaultValue;
        }
        
        @Override
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) 
        {
            int[] numConfigs = new int[1];
            if(egl.eglGetConfigs(display, null, 0, numConfigs))
            {
                EGLConfig[] configs = new EGLConfig[numConfigs[0]];
                int[] EGLattribs = {
                        EGL10.EGL_RED_SIZE, configAttribs[0], 
                        EGL10.EGL_GREEN_SIZE, configAttribs[1],
                        EGL10.EGL_BLUE_SIZE, configAttribs[2],
                        EGL10.EGL_ALPHA_SIZE, configAttribs[3],
                        EGL10.EGL_DEPTH_SIZE, configAttribs[4],
                        EGL10.EGL_STENCIL_SIZE,configAttribs[5],
                        EGL10.EGL_RENDERABLE_TYPE, 4, //EGL_OPENGL_ES2_BIT
                        EGL10.EGL_NONE
                                    };
                int[] choosedConfigNum = new int[1];
                
                egl.eglChooseConfig(display, EGLattribs, configs, numConfigs[0], choosedConfigNum);
                if(choosedConfigNum[0]>0)
                {
                    return selectConfig(egl, display, configs, configAttribs);
                }
                else
                {
                    int[] defaultEGLattribs = {
                            EGL10.EGL_RED_SIZE, 5, 
                            EGL10.EGL_GREEN_SIZE, 6,
                            EGL10.EGL_BLUE_SIZE, 5,
                            EGL10.EGL_ALPHA_SIZE, 0,
                            EGL10.EGL_DEPTH_SIZE, 0,
                            EGL10.EGL_STENCIL_SIZE,0,
                            EGL10.EGL_RENDERABLE_TYPE, 4, //EGL_OPENGL_ES2_BIT
                            EGL10.EGL_NONE
                                        };
                    int[] defaultEGLattribsAlpha = {
                            EGL10.EGL_RED_SIZE, 4, 
                            EGL10.EGL_GREEN_SIZE, 4,
                            EGL10.EGL_BLUE_SIZE, 4,
                            EGL10.EGL_ALPHA_SIZE, 4,
                            EGL10.EGL_DEPTH_SIZE, 0,
                            EGL10.EGL_STENCIL_SIZE,0,
                            EGL10.EGL_RENDERABLE_TYPE, 4, //EGL_OPENGL_ES2_BIT
                            EGL10.EGL_NONE
                                        };
                    int[] attribs = null;
                    //choose one can use
                    if(this.configAttribs[3] == 0)
                    {
                        egl.eglChooseConfig(display, defaultEGLattribs, configs, numConfigs[0], choosedConfigNum);
                        attribs = new int[]{5,6,5,0,0,0};
                    }
                    else
                    {
                        egl.eglChooseConfig(display, defaultEGLattribsAlpha, configs, numConfigs[0], choosedConfigNum);
                        attribs = new int[]{4,4,4,4,0,0};
                    }
                    if(choosedConfigNum[0] > 0)
                    {
                        return selectConfig(egl, display, configs, attribs);
                    }
                    else
                    {
                        Log.e(DEVICE_POLICY_SERVICE, "Can not select an EGLConfig for rendering.");
                        return null;
                    }
                }
            }
            Log.e(DEVICE_POLICY_SERVICE, "Can not select an EGLConfig for rendering.");
            return null;
        }

    }
    
    public static Context getContext() {
        return sContext;
    }

    public static void SetContentView()
    {

        sContext.resetcontentview();
    }

    public void resetcontentview() {
//        if(waitingFlashScreen)
//        {
//           waitingFlashScreen = false;
//            if(mFrameLayout != null)
//            setContentView(mFrameLayout);
//
//        }
//        mGLSurfaceView.setVisibility(View.VISIBLE);
        Log.d(TAG, "LIUDI FrontImage REset");
        Message msg = new Message();
        msg.what = Cocos2dxHandler.HADNLER_RESET_FRONTIMAGE;
        this.mHandler.sendMessage(msg);
//        msg.obj = new Cocos2dxHandler.DialogMessage(pTitle, pMessage);
    }

    public void removeFrontImage()
    {
        if(mFrontImage != null)
        {
            mFrameLayout.removeView(mFrontImage);
            mFrontImage=null;
        }
            
    }
    
    public void setKeepScreenOn(boolean value) {
        final boolean newValue = value;
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mGLSurfaceView.setKeepScreenOn(newValue);
            }
        });
    }
    
    protected void onLoadNativeLibraries() {
        try {
            ApplicationInfo ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;
            String libName = bundle.getString("android.app.lib_name");
            Log.d(TAG, "LIUDI, load native"+libName);
            if(libName != null && libName != "")
            	System.loadLibrary(libName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    // ===========================================================
    // Constructors
    // ===========================================================
    
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        Log.d(TAG, "LIUDI, 3.6, Hello World");
        super.onCreate(savedInstanceState);
        if ((getIntent().getFlags() & Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT) != 0)
        {  
            finish();
            return;
        }
        //CocosPlayClient.init(this, false);
        onLoadNativeLibraries();

        sContext = this;
        this.mHandler = new Cocos2dxHandler(this);
        
        Cocos2dxHelper.init(this);
        String loadingMusicPath = "sounds/background/m_logo.ogg";
        Cocos2dxHelper.preloadBackgroundMusic(loadingMusicPath);
        Cocos2dxHelper.playBackgroundMusic(loadingMusicPath, false);
        
        this.mGLContextAttrs = getGLContextAttrs();
        this.init();

        if (mVideoHelper == null) {
            mVideoHelper = new Cocos2dxVideoHelper(this, mFrameLayout);
        }
        
        if(mWebViewHelper == null){
            mWebViewHelper = new Cocos2dxWebViewHelper(mFrameLayout);
        }
    }

    //native method,call GLViewImpl::getGLContextAttrs() to get the OpenGL ES context attributions
    private static native int[] getGLContextAttrs();
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    @Override
    protected void onResume() {
        super.onResume();

        Cocos2dxHelper.onResume();
        this.mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        
        Cocos2dxHelper.onPause();
        this.mGLSurfaceView.onPause();
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
	
	public void handleMessage(Message message){
		mHandler.sendMessage(message);
	}
	
	
	public void handle2dxResume()
	{
		this.mGLSurfaceView.handle2dxResume();
	}
	
	public void handle2dxPause()
	{
		this.mGLSurfaceView.handle2dxPause();
	}

    @Override
    public void showDialog(final String pTitle, final String pMessage) {
        Message msg = new Message();
        msg.what = Cocos2dxHandler.HANDLER_SHOW_DIALOG;
        msg.obj = new Cocos2dxHandler.DialogMessage(pTitle, pMessage);
        this.mHandler.sendMessage(msg);
    }

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) { 
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG;
		msg.obj = new Cocos2dxHandler.EditBoxMessage(pTitle, pContent, pInputMode, pInputFlag, pReturnType, pMaxLength);
		this.mHandler.sendMessage(msg);
	}
    
    @Override
    public void runOnGLThread(final Runnable pRunnable) {
        this.mGLSurfaceView.queueEvent(pRunnable);
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        for (OnActivityResultListener listener : Cocos2dxHelper.getOnActivityResultListeners()) {
            listener.onActivityResult(requestCode, resultCode, data);
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

	protected boolean waitingFlashScreen = false;
    protected FrameLayout mFrameLayout = null;
    protected ImageView mFrontImage = null;
    // ===========================================================
    // Methods
    // ===========================================================
    public void init() {
        
        // FrameLayout
        ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                                       ViewGroup.LayoutParams.MATCH_PARENT);
        mFrameLayout = new FrameLayout(this);
        mFrameLayout.setLayoutParams(framelayout_params);

        // Cocos2dxEditText layout
        ViewGroup.LayoutParams edittext_layout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                                       ViewGroup.LayoutParams.WRAP_CONTENT);
        Cocos2dxEditText edittext = new Cocos2dxEditText(this);
        edittext.setLayoutParams(edittext_layout_params);

        // ...add to FrameLayout
        mFrameLayout.addView(edittext);

        // Cocos2dxGLSurfaceView
        this.mGLSurfaceView = this.onCreateView();

        // ...add to FrameLayout
        mFrameLayout.addView(this.mGLSurfaceView);

        // Switch to supported OpenGL (ARGB888) mode on emulator
        if (isAndroidEmulator())
           this.mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);

        this.mGLSurfaceView.setCocos2dxRenderer(new Cocos2dxRenderer());
        this.mGLSurfaceView.setCocos2dxEditText(edittext);

        // Set framelayout as the content view
        if(!waitingFlashScreen)
        	setContentView(mFrameLayout);
//        this.mGLSurfaceView.setVisibility(View.INVISIBLE);
        if(mFrontImage == null)
        {
            mFrontImage = new ImageView(this);
            mFrameLayout.addView(mFrontImage);
            initFrontImage();
        }

        
	}

    public void initFrontImage()
    {
        try{

            TypedValue a = new TypedValue();
            getTheme().resolveAttribute(android.R.attr.windowBackground, a, true);
            if(mFrontImage != null)
            {
                if(a.type >= TypedValue.TYPE_FIRST_COLOR_INT && a.type <= TypedValue.TYPE_LAST_COLOR_INT)
                {
                    int color = a.data;
                    mFrontImage.setBackgroundColor(color);
                }
                else
                {
                    Drawable d = this.getResources().getDrawable(a.resourceId);
                    mFrontImage.setImageDrawable(d);
//                    mFrontImage.setLayoutParams(new LinearLayout.LayoutParams(
//                            LinearLayout.LayoutParams.MATCH_PARENT,
//                            LinearLayout.LayoutParams.MATCH_PARENT));
                    mFrontImage.setLayoutParams(new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
                    mFrontImage.setScaleType(ImageView.ScaleType.FIT_XY);
                }
            }
        }
        catch (Exception e)
        {
            Log.d(TAG, "LIUDI FrontImage Wrong");
        }

    }

    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        //this line is need on some device if we specify an alpha bits
        if(this.mGLContextAttrs[3] > 0) glSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

        Cocos2dxEGLConfigChooser chooser = new Cocos2dxEGLConfigChooser(this.mGLContextAttrs);
        glSurfaceView.setEGLConfigChooser(chooser);

        return glSurfaceView;
    }

   private final static boolean isAndroidEmulator() {
      String model = Build.MODEL;
      Log.d(TAG, "model=" + model);
      String product = Build.PRODUCT;
      Log.d(TAG, "product=" + product);
      boolean isEmulator = false;
      if (product != null) {
         isEmulator = product.equals("sdk") || product.contains("_sdk") || product.contains("sdk_");
      }
      Log.d(TAG, "isEmulator=" + isEmulator);
      return isEmulator;
   }

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
}
