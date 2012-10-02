package ch.nuicell.ecam;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap.CompressFormat;
import android.os.Environment;
import android.os.Handler;
import android.widget.Toast;

public class Exporter implements Runnable{

	private final Handler mHandler = new Handler();
	private Context context;
	private CameraPreview cameraPreview;
	
	public Exporter(Context context, CameraPreview cameraPreview){
		this.context = context;
		this.cameraPreview = cameraPreview;
	}
	
	@Override
	public void run() {
		
		synchronized (((NuicellApplication)context.getApplicationContext()).sBitmapLock) {
			String datef = "yyyy_MM_dd_HH_mm_ss";
			SimpleDateFormat sdf = new SimpleDateFormat(datef);
			String fileName = sdf.format(new Date()) + ".png";
			File ext = new File(Environment.getExternalStorageDirectory()
					+ File.separator + "nuicell");
			ext.mkdirs();
			
			final File f = new File(ext, fileName);
			
			FileOutputStream fos = null;
			try {
				fos = new FileOutputStream(f);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			cameraPreview.bmp.compress(CompressFormat.PNG, 100, fos);
			try {
				fos.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				fos.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			mHandler.post(new Runnable() {
				@Override
				public void run() {
					Toast.makeText(context, f.getPath()+" saved", Toast.LENGTH_LONG).show();
					Intent intent = new Intent();
					intent.setAction("ch.nuicell.ecam.IMAGE_CAPTURED");
					intent.putExtra("PATH", f.getAbsolutePath());
					context.sendBroadcast(intent);
				}
			});
		}
		
		
	}

}
