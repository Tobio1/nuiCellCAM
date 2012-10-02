package ch.nuicell.ecam;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class Receiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		
		String path = intent.getStringExtra("PATH");
		Log.d("NUICELL", path);
	}

}
