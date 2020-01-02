package com.nexell.backgearservice;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.pm.PackageInfo;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.util.List;

public class BackGearDetect extends Thread {
	private final String NX_TAG = "BackGearDetect";

	private Context m_Context;

	//file for cmd and status
	public final String CMD_RECEIVE_FILE_NAME_QUICK = "/data/data/quickrearcam_status";
	public final String CMD_SEND_FILE_NAME_QUICK = "/data/data/quickrearcam_cmd";
	public final String CMD_FILE_NAME = "/data/data/com.nexell.backgearservice/rearcam_cmd";

    //quick rearcam type
	private final int CAM_TYPE_QUICKREAR    = 0;
	private final int CAM_1CAMTOPVIEW       = 1;
	private final int CAM_TYPE_4CAM         = 2;

	private int quickrearcam_type = -1;

	//to execute the package name of the application
	private String Package_Name;

    //GPIO information
	private final int BACK_GEAR_GPIO_NUM = 71;
	private final int BACK_GEAR_DETECTED = 0;
	private final int BACK_GEAR_RELEASED = 1;
	private final int GPIO_DIRECTION = 0;
	private final int GPIO_EDGE = 3;

	//for thread
	private boolean m_ExitLoop = false;

	//class for backgear service control
	private CtrlBackGearService m_ctrlBackGearService;
	//class for checking quick rearcam status(stopped)
	public QuickRearCamCmd m_QuickRearCamCmd;
	//class for SVM connection
	public NxSvmConnectToServer m_SvmConnection;

	//for saving current gpio status
	private int cur_gpio_status = -1;

	//for reading gpio status
	private int read_gpio_status = -1;

	//for checking existing the application
	private boolean app_exist = false;

	//client name for svm
	private int client_name = 0;

	public BackGearDetect(Context context) {

		this.m_ctrlBackGearService = new CtrlBackGearService();
		this.m_Context = context.getApplicationContext();

		//checking the application existing
		this.app_exist = getPackageList();

		if(quickrearcam_type == CAM_TYPE_QUICKREAR) {
			this.m_QuickRearCamCmd = new QuickRearCamCmd(m_Handler, m_ctrlBackGearService);
		}else
		{
			this.m_SvmConnection = new NxSvmConnectToServer(m_Handler, m_ctrlBackGearService, client_name);
		}

	}

	Handler m_Handler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			if(quickrearcam_type == CAM_TYPE_QUICKREAR) {
				if (msg.arg1 == 1) {
					Log.i(NX_TAG, "------------------quickrearcam stopped");
					if(!m_QuickRearCamCmd.interrupted()) {
						m_QuickRearCamCmd.interrupt();
					}
					m_ctrlBackGearService.StopCmdService();

					start();
				}
			}else
			{
				if(msg.arg1 == 1) {
					Log.i(NX_TAG, "-------------------svm connected to server");

					if(!m_SvmConnection.interrupted()) {
						m_SvmConnection.interrupt();
					}

					m_ctrlBackGearService.SvmDisconnectFromServer(0);

					start();
				}
			}
		}
	};

	public void run() {
		int ret = 0;

		boolean rearcam_running = false;


		//gpio export and setting
		m_ctrlBackGearService.GpioInit(BACK_GEAR_GPIO_NUM);
		m_ctrlBackGearService.GpioSetDirection(GPIO_DIRECTION);
		m_ctrlBackGearService.GpioSetEdge(GPIO_EDGE);

		m_ctrlBackGearService.CreateCmdFifo(CMD_FILE_NAME);

		m_ExitLoop = false;

		while(!m_ExitLoop)
		{
			try {
				//get gpio value
				read_gpio_status = m_ctrlBackGearService.GpioGetValue();

				if (cur_gpio_status != read_gpio_status) {
					switch (read_gpio_status) {
						case BACK_GEAR_DETECTED: {
							Log.i(NX_TAG, "------------------BACK_GEAR_DETECTED : ");

							ActivityManager activityManager = (ActivityManager)m_Context.getSystemService(Context.ACTIVITY_SERVICE);
							List<RunningAppProcessInfo>ProcInfos = activityManager.getRunningAppProcesses();
							for(int i=0; i< ProcInfos.size();i++)
							{
								if(ProcInfos.get(i).processName.equals(Package_Name))
								{
									Log.i(NX_TAG, "-------"+Package_Name+" already running");
									rearcam_running = true;
									break;
								}
							}

							if(app_exist == true && rearcam_running == false)
							{
								//excute rearcam application

								Log.i(NX_TAG, "-------"+Package_Name+" run");
								rearcam_running = true;
								Intent intent = m_Context.getPackageManager().getLaunchIntentForPackage(Package_Name);
								intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
								m_Context.startActivity(intent);

							}
							Thread.sleep(1000);

							break;
						}
						case BACK_GEAR_RELEASED: {
							Log.i(NX_TAG, "------------------BACK_GEAR_RELEASED : ");
							String msg = "stop";

							//send stop command to rearcam
							if(rearcam_running == true) {
								ret = m_ctrlBackGearService.WritCmd(CMD_FILE_NAME, false);

								if(ret < 0)
									Log.i(NX_TAG, "------------------write command fail");

								Thread.sleep(1000);

								rearcam_running = false;

								break;
							}
						}

						default:
							break;
					}
				}

				cur_gpio_status = read_gpio_status;
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		m_ctrlBackGearService.GpioDeInit();
	}

	public boolean getPackageList()
	{
		boolean isExist = false;

		//quickrearcam_type checking (NxQuickRearCam or SVM)
		quickrearcam_type = m_ctrlBackGearService.GetQuickRearCamType();
		Log.i(NX_TAG, "------------------QuickRearCam Type : " + quickrearcam_type);

		//setting package name to execute, according to quickrearcam type
		if(quickrearcam_type == CAM_TYPE_QUICKREAR)
		{
			Package_Name = "com.nexell.rearcam";
		}else
		{
			Package_Name = "com.nexell.svm";
		}

		//Checking exsting of the application by the package name
		PackageManager pkgMgr = m_Context.getPackageManager();
		List <ResolveInfo> mApps;
		Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
		mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		mApps = pkgMgr.queryIntentActivities(mainIntent, 0);


		try {
			for (int i=0; i < mApps.size(); i++)
			{
				if(mApps.get(i).activityInfo.packageName.startsWith(Package_Name))
				{
					isExist = true;
					break;
				}
			}
		}
		catch (Exception e)
		{
			isExist = false;
		}

		if(isExist != true){
			Log.i(NX_TAG, "------------------not exist application : " + Package_Name);
		}

		return isExist;
	}

	public void BackGearDetectStart()
	{
		if(quickrearcam_type == CAM_TYPE_QUICKREAR) {
			//send to stop command to quickrearcam and get quickrearcam status(stopped) for gpio contol
			m_ctrlBackGearService.CreateCmdFifo(CMD_SEND_FILE_NAME_QUICK);

			m_ctrlBackGearService.StartCmdService(CMD_RECEIVE_FILE_NAME_QUICK, true);
			m_QuickRearCamCmd.start();

		}else
		{
			//try to connect to nx_3d_avm_daemon to get gpio control
			m_SvmConnection.start();
		}
	}

	public void BackGearDetectStop()
	{
		m_ExitLoop = true;
	}

}
