package com.nexell.backgearservice;

import android.os.Handler;
import android.os.Message;

public class QuickRearCamCmd extends Thread{
	//This is the class for receiving the status of NxQuickRearCam
	private final String NX_DTAG = "QuickRearCamCmd";

	Handler m_Handler;
	CtrlBackGearService m_ctrlBackGearService;

	private int received_stopped_cmd = 0;
	private boolean m_ExitLoop = false;

	QuickRearCamCmd(Handler handler, CtrlBackGearService ctrlbackgearservice)
	{
		this.m_Handler = handler;
		this.m_ctrlBackGearService = ctrlbackgearservice;
	}

	@Override
	public void run() {
		Message msg = new Message();

		received_stopped_cmd = 0;

		while(!m_ExitLoop) {

			received_stopped_cmd = m_ctrlBackGearService.GetQuickRearCamStatus();

			if (received_stopped_cmd == 1) {
				msg.what = 0;
				msg.arg1 = 1;
				m_Handler.sendMessage(msg);

				m_ExitLoop = true;
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
