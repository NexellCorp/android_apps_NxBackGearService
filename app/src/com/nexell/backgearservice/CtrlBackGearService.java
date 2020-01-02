package com.nexell.backgearservice;

import android.util.Log;

public class CtrlBackGearService {
	private static final String NX_TAG = "CtrlBackGearService";

	private boolean connect_status = false;

	public CtrlBackGearService() {}

	//gpio export
	public synchronized boolean GpioInit(int gpio)
	{
		return (0 == NX_JniGpioInit(gpio));
	}

	//gpio unexport
	public synchronized boolean GpioDeInit()
	{
		return (0 == NX_JniGpioDeInit());
	}

	//set gpio direction
	public synchronized boolean GpioSetDirection(int direction)
	{
		return (0 == NX_JniGpioSetDirection(direction));
	}

	//get gpio value
	public synchronized int GpioGetValue()
	{
		return NX_JniGpioGetValue();
	}

	//set gpio edge
	public synchronized int GpioSetEdge(int edge){
		return NX_JniGpioSetEdge(edge);
	}

	//create command file
	public synchronized int CreateCmdFifo(String cmd_file_name)
	{
		return NX_JniCreateCmdFile(cmd_file_name);
	}

	//write command to command file
	public synchronized int WritCmd(String cmd_file_name, boolean quick_cmd)
	{
		return NX_JniWriteCmd(cmd_file_name, quick_cmd);
	}

	//Check status of QuickRearCam
	public synchronized int GetQuickRearCamStatus()
	{
		return NX_JniCheckQuickRearCamStatus();
	}

	//start command service
	public synchronized int StartCmdService(String cmd_file_name, boolean quick_cmd)
	{
		Log.i(NX_TAG, "------------------StartCmdService" );
		return NX_JniNxStartCmdService(cmd_file_name, quick_cmd);
	}

	//stop commnad service
	public synchronized int StopCmdService()
	{
		return NX_JniNXStopCommandService();
	}

	//conntect to nx_3d_avm_daemon
	public synchronized int SvmConnectToServer(int client_name, int skip_socket_connect) {
		int cam_cnt = -1;

		if(connect_status == false) {
			cam_cnt= NX_JniNxSvmConnectToServer(client_name, skip_socket_connect);
		}
		if(cam_cnt > 0)
		{
			connect_status = true;
		}else
		{
			connect_status = false;
		}

		return cam_cnt;
	}

	//disconntect from nx_3d_avm_daemon
	public synchronized void SvmDisconnectFromServer(int release_resource) {
		NX_JniNxSvmDisconnectFromServer(release_resource);
		connect_status = false;
	}

	//get quickrearcam type (QuickRearCam / SVM / 1CamTopView)
	public synchronized int GetQuickRearCamType()
	{
		return NX_JniGetQuickRearCamType();
	}


	//----------------------------------------------------------------------------------
	//jni library & api
	static {
		System.loadLibrary("nxbackgearservice_jni");
	}

	public native int NX_JniGpioInit(int gpio);
 	public native int NX_JniGpioDeInit();
 	public native int NX_JniGpioSetDirection(int direction);
	public native int NX_JniGpioGetValue();
 	public native int NX_JniGpioSetEdge(int edge);
 	public native int NX_JniCreateCmdFile(String cmd_file_name);
 	public native int NX_JniWriteCmd(String cmd_file_named, boolean quick_cmd);
	public native int NX_JniCheckQuickRearCamStatus();
 	public native int NX_JniNxStartCmdService(String cmd_file_name, boolean quick_cmd);
 	public native int NX_JniNXStopCommandService();
 	public native int NX_JniNxSvmConnectToServer(int client_name, int skip_socket_connect);
 	public native int NX_JniNxSvmDisconnectFromServer(int release_resource);
 	public native int NX_JniGetQuickRearCamType();

}
