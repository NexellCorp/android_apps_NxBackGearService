package com.nexell.backgearservice;

import android.os.Handler;
import android.os.Message;

public class NxSvmConnectToServer extends Thread {
	private final String NX_DTAG = "NxSvmConnectToServer";

	Handler mHandler;
	CtrlBackGearService m_BackGearService;

	private int client_name = 0;

	private int preset_cnt = 0;
	private int resource_create = 0;

	private int enable_mode_edit = 0;
	private boolean m_ExitLoop = false;


	NxSvmConnectToServer(Handler mHandler, CtrlBackGearService backgearservice, int client_name){
		this.mHandler = mHandler;
		this.m_BackGearService = backgearservice;
		this.client_name = client_name;
	}

	@Override
	public void run() {
		int cam_cnt = -1;
		int skip_socket_connect = 0;
		Message msg = new Message();
		cam_cnt = m_BackGearService.SvmConnectToServer(client_name, skip_socket_connect);

		if(cam_cnt <= 0){
			skip_socket_connect = 1;

			while(!m_ExitLoop){
				cam_cnt = m_BackGearService.SvmConnectToServer(client_name, skip_socket_connect);
				if( cam_cnt > 0)
				{
					msg.what = 0;
					msg.arg1 = 1;
					msg.arg2 = enable_mode_edit;
					mHandler.sendMessage(msg);

					m_ExitLoop = true;

					try{
						Thread.sleep(10);
					}catch(InterruptedException e){

					}finally{
					};
				}

			}
		}else
		{
			msg.what = 0;
			msg.arg1 = 1;
			msg.arg2 = enable_mode_edit;
			mHandler.sendMessage(msg);
			try{
				Thread.sleep(10);
			}catch(InterruptedException e){
				e.printStackTrace();
			}finally{
				System.out.println("SVM Connected to Server.");
			};
		}
	}

}
