package com.nexell.backgearservice;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

public class BackGearService extends Service {
	private final String NX_DTAG = "BackGearService";

	public Context m_Context;
	private IBinder m_binder = new BackGearBinder();

	private BackGearDetect m_BackGearDetect;


	class BackGearBinder extends Binder {
		BackGearService getService(){
			return BackGearService.this;
		}
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onCreate() {
		m_Context = this;
		super.onCreate();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {

		m_BackGearDetect = new BackGearDetect(m_Context);

		m_BackGearDetect.BackGearDetectStart();

		return super.onStartCommand(intent, flags, startId);
	}

	@Override
	public void onDestroy() {
		m_BackGearDetect.BackGearDetectStop();
		super.onDestroy();
	}

	@Override
	public boolean onUnbind(Intent intent) {
		return super.onUnbind(intent);
	}
}


