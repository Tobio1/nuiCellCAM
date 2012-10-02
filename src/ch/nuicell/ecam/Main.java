package ch.nuicell.ecam;

import com.camera.simplewebcam.R;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class Main extends Activity {
	
	CameraPreview cp;
	SeekBar seekbarFocus, seekbarExposure, seekbarGain, seekbarBrightness;
	TextView textviewFocus, textviewExposure, textviewGain, textviewBrightness;
	Button buttonCapture;
	Bitmap bmp;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);
		
		cp = (CameraPreview) findViewById(R.id.surface);
		textviewFocus = (TextView) findViewById(R.id.tvFocus);
		textviewExposure = (TextView) findViewById(R.id.tvExposure);
		textviewGain = (TextView) findViewById(R.id.tvGain);
		textviewBrightness = (TextView) findViewById(R.id.tvBrightness);
		buttonCapture = (Button) findViewById(R.id.btCapture);
		
		buttonCapture.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				//cp.captureNextImage();
				
				Exporter exporter = new Exporter(getApplicationContext(), cp);
				Thread thread = new Thread(exporter);
				thread.start();
				
			}
		});
		
		seekbarFocus = (SeekBar) findViewById(R.id.seekbarFocus);
		seekbarFocus.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				int value = progress;
				cp.setManualFocus(value);
				textviewFocus.setText(String.valueOf(value));
			}
		});
		
		seekbarExposure = (SeekBar) findViewById(R.id.seekbarExposure);
		seekbarExposure.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				int value = progress + 8;
				cp.setExposure(value);
				textviewExposure.setText(String.valueOf(value));
			}
		});
		
		seekbarGain = (SeekBar) findViewById(R.id.seekbarGain);
		seekbarGain.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				int value = progress + 32;
				cp.setExposure(value);
				textviewGain.setText(String.valueOf(value));
			}
		});
		
		seekbarBrightness = (SeekBar) findViewById(R.id.seekbarBrightness);
		seekbarBrightness.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) { 
				
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				int value = progress - 10;
				cp.setBrightness(value);
				textviewBrightness.setText(String.valueOf(value));
			}
		});
		
		
	}
	
}
