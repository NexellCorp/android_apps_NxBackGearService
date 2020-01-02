package com.nexell.backgearservice;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
//import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.pm.PackageManager;
import android.content.ComponentName;

public class MainActivity extends Activity {

	public Context m_Context;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		m_Context = this;
		this.startService(new Intent(this, BackGearService.class));

		finish();
	}
}
