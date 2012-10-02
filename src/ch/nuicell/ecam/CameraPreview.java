package ch.nuicell.ecam;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;


import android.content.Context;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Bitmap.CompressFormat;

class CameraPreview extends SurfaceView implements SurfaceHolder.Callback, Runnable {

	private static final boolean DEBUG = true;
	private final Handler mHandler = new Handler();
	protected Context context;
	private SurfaceHolder holder;
    Thread mainLoop = null;
	public Bitmap bmp=null;

	private boolean cameraExists=false;
	private boolean shouldStop=false;
	private boolean capture=false;
	
	// /dev/videox (x=cameraId+cameraBase) is used.
	// In some omap devices, system uses /dev/video[0-3],
	// so users must use /dev/video[4-].
	// In such a case, try cameraId=0 and cameraBase=4
	private int cameraId=0;
	private int cameraBase=0;
	
	// This definition also exists in ImageProc.h.
	// Webcam must support the resolution 640x480 with YUYV format. 
	//static final int IMG_WIDTH=640;
	//static final int IMG_HEIGHT=480;
	static final int IMG_WIDTH=2592;
	static final int IMG_HEIGHT=1944;

	// The following variables are used to draw camera images.
    private int winWidth=0;
    private int winHeight=0;
    private Rect rect;
    private int dw, dh;
    private float rate;
  
    // JNI functions
    public native int prepareCamera(int videoid);
    public native int prepareCameraWithBase(int videoid, int camerabase);
    public native void processCamera();
    public native void stopCamera();
    public native void pixeltobmp(Bitmap bitmap);
    public native int setManualFocus(int value);
    public native int setExposure(int value);
    public native int setGain(int value);
    public native int setBrightness(int value);
    
    static {
        System.loadLibrary("ImageProc");
    }
    
	public CameraPreview(Context context) {
		super(context);
		this.context = context;
		if(DEBUG) Log.d("WebCam","CameraPreview constructed");
		setFocusable(true);
		
		holder = getHolder();
		holder.addCallback(this);
		holder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);	
	}
	
	
	public CameraPreview(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.context = context;
		if(DEBUG) Log.d("WebCam","CameraPreview constructed");
		setFocusable(true);
		
		holder = getHolder();
		holder.addCallback(this);
		holder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);	
		
	}
	
    @Override
    public void run() {
        while (true && cameraExists) {
        	//obtaining display area to draw a large image
        	if(winWidth==0){
        		winWidth=this.getWidth();
        		winHeight=this.getHeight();

        		if(winWidth*3/4<=winHeight){
        			dw = 0;
        			dh = (winHeight-winWidth*3/4)/2;
        			rate = ((float)winWidth)/IMG_WIDTH;
        			rect = new Rect(dw,dh,dw+winWidth-1,dh+winWidth*3/4-1);
        		}else{
        			dw = (winWidth-winHeight*4/3)/2;
        			dh = 0;
        			rate = ((float)winHeight)/IMG_HEIGHT;
        			rect = new Rect(dw,dh,dw+winHeight*4/3 -1,dh+winHeight-1);
        		}
        	}
        	
        	// obtaining a camera image (pixel data are stored in an array in JNI).
        	processCamera();
        	// camera image to bmp
        	pixeltobmp(bmp);
        	
            Canvas canvas = getHolder().lockCanvas();
            if (canvas != null)
            {
            	// draw camera bmp on canvas
            	canvas.drawBitmap(bmp,null,rect,null);
            	
            	synchronized (((NuicellApplication)context.getApplicationContext()).sBitmapLock) {
					
				}

            	getHolder().unlockCanvasAndPost(canvas); 
            }

            if(shouldStop){
            	shouldStop = false;  
            	break;
            }	        
        }
    }
    
    public void captureNextImage(){
    	this.capture = true;
    }

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		if(DEBUG) Log.d("WebCam", "surfaceCreated");
		if(bmp==null){
			bmp = Bitmap.createBitmap(IMG_WIDTH, IMG_HEIGHT, Bitmap.Config.ARGB_8888);
		}
		// /dev/videox (x=cameraId + cameraBase) is used
		int ret = prepareCameraWithBase(cameraId, cameraBase);
		
		if(ret!=-1) cameraExists = true;
		
        mainLoop = new Thread(this);
        mainLoop.start();		
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		if(DEBUG) Log.d("WebCam", "surfaceChanged");
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		if(DEBUG) Log.d("WebCam", "surfaceDestroyed");
		if(cameraExists){
			shouldStop = true;
			while(shouldStop){
				try{ 
					Thread.sleep(100); // wait for thread stopping
				}catch(Exception e){}
			}
		}
		stopCamera();
	}
	
}
