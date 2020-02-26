//
// Created by jmbahn on 19. 8. 21.
//

#include "nxcommand.h"

#include <stdio.h>
#include <poll.h>
#include <stdint.h>	//	intXX_t
#include <fcntl.h>	//	O_RDWR
#include <string.h>	//	memset
#include <unistd.h>
#include <stddef.h>	//	struct sockaddr_un
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include "CNX_BaseClass.h"
#include "nxcommand.h"

#define NX_DTAG "[service_nxcommand]"
#include <NX_DbgMsg.h>


#define POLL_TIMEOUT_MS		(2000)

class NX_CCommand : protected CNX_Thread
{
public:
	void RegEventCallback( void (*cbFunc)( int32_t) );
	void StartService(const char* cmd_file_name, bool quick_cmd);
	void StopService();
	int IsReceivedStopCmd();
	int GetQuickRearCamStatus();
	int CreateCmdFile(const char* cmd_file_name);
	int RemoveCmdFile(const char* cmd_file_name);
	int WriteCmd(const char* cmd_file_name, bool quick_cmd);

	NX_CCommand();
	~NX_CCommand();

	//  Implementation Pure Virtual Function
	virtual void  ThreadProc();


private:
	//
	// Hardware Depend Parameter
	//

	static NX_CCommand* m_pCommandServer;

	int				m_bExitLoop;
	int 			fd_gpio;
	int				fd_cmd;

	struct pollfd poll_fds;
	int poll_ret;
	unsigned char rx_buf[256];

	int m_iReceiveStopCmd;
	int m_iReceiveQuickStoppedCmd;
	bool m_bQuickCmd;

	const char *m_pStopCmdFileName;
	const char *m_pStatusCmdFileName;



	//	for Callback Function
	void				(*m_EventCallBack)(int32_t);






private:
	NX_CCommand (const NX_CCommand &Ref);
	NX_CCommand &operator=(const NX_CCommand &Ref);
};

//------------------------------------------------------------------------------
NX_CCommand::NX_CCommand()
	:m_bExitLoop ( false )
    , m_iReceiveStopCmd( 0 )
    , m_iReceiveQuickStoppedCmd ( 0 )
    , m_bQuickCmd( false )
{

}

//------------------------------------------------------------------------------
NX_CCommand::~NX_CCommand()
{
}


//------------------------------------------------------------------------------
int NX_CCommand::CreateCmdFile(const char* m_pCmdFileName)
{
    int ret = -1;

    NxDbgMsg( NX_DBG_INFO, "%s()---- %s\n", __FUNCTION__, m_pCmdFileName);

    if( 0 == access(m_pCmdFileName, F_OK))
    {
    	remove(m_pCmdFileName);
    }

    ret = mknod(m_pCmdFileName, S_IFIFO | 0666, 0 );

    return ret;
}

//------------------------------------------------------------------------------
int NX_CCommand::RemoveCmdFile(const char* m_pCmdFileName)
{
    int ret = -1;

    NxDbgMsg( NX_DBG_INFO, "%s()---- %s\n", __FUNCTION__, m_pCmdFileName);


    if( 0 == access(m_pCmdFileName, F_OK))
    {
    	ret = remove(m_pCmdFileName);
    }

    return ret;
}

//----------------------------------------------------------------------------
int NX_CCommand::WriteCmd(const char* m_pCmdFileName, bool quick_cmd)
{
    int cmd_fd = 0;
    int ret = 0;

    cmd_fd = open(m_pCmdFileName, O_RDWR);

    NxDbgMsg( NX_DBG_INFO, "%s()---- : cmd_fd : %d\n", __FUNCTION__, cmd_fd);

    if(cmd_fd >= 0)
    {
        if(quick_cmd == true)
        {
            ret = write(cmd_fd, "quick_stop", sizeof("quick_stop"));
        }else
        {
            ret = write(cmd_fd, "stop", sizeof("stop"));
        }

        NxDbgMsg( NX_DBG_INFO, "%s()---- : write res : %d\n", __FUNCTION__, ret);

        close( cmd_fd );
        return 1;
    }

    return -1;
}


void NX_CCommand::StartService(const char *cmd_file_name, bool quick_cmd)
{
	m_bExitLoop = false;

	m_bQuickCmd = quick_cmd;

	m_pStopCmdFileName = cmd_file_name;

	Start();
}

void NX_CCommand::StopService()
{
	m_bExitLoop = true;
	Stop();
	close( fd_cmd );
}

int NX_CCommand::IsReceivedStopCmd()
{
    return m_iReceiveStopCmd;
}

int NX_CCommand::GetQuickRearCamStatus()
{
    return m_iReceiveQuickStoppedCmd;
}

void  NX_CCommand::ThreadProc()
{
#if 0
    usleep(2000000);

	fd_cmd  = open( m_pStopCmdFileName, O_RDWR );

	do{

		memset( &poll_fds, 0, sizeof(poll_fds) );
		poll_fds.fd = fd_cmd;
		poll_fds.events = POLLIN;

		poll_ret = poll( &poll_fds, 2, POLL_TIMEOUT_MS );
		if( poll_ret == 0 )
		{
			//printf("Poll Timeout\n");
			continue;
		}
		else if( 0 > poll_ret )
		{
			printf("Poll Error !!!\n");
		}
		else
		{
			int read_size;

			if( poll_fds.revents & POLLIN )
			{
				memset( rx_buf, 0, sizeof(rx_buf) );
				read_size = read( fd_cmd, rx_buf, sizeof(rx_buf));
				printf("CMD : Size = %d, %s\n", read_size, rx_buf);

				if(m_bQuickCmd == true)
				{
				    if(!strncmp((const char*)rx_buf, "stopped", sizeof("stopped")))
                    {
                        m_iReceiveQuickStoppedCmd = 1;
                    }

				}else
				{
                    if(!strncmp((const char*)rx_buf, "stop", sizeof("stop")))
                    {
                        m_iReceiveStopCmd = 1;
                    }
				}

			}
		}

	}while( !m_bExitLoop );

#else
    int ret = -1;
    NxDbgMsg( NX_DBG_INFO,"%s()---- thread start\n", __FUNCTION__);
    while(!m_bExitLoop ){
        if(m_iReceiveQuickStoppedCmd == 0)
        {
            ret = access(m_pStopCmdFileName, F_OK);
            NxDbgMsg( NX_DBG_INFO, "%s()---- %s access ret : %d\n", __FUNCTION__, m_pStopCmdFileName, ret);
            if( 0 == ret)
            {
                m_iReceiveQuickStoppedCmd = 1;
                NxDbgMsg( NX_DBG_INFO,"quick rearcam stop !!!\n");
            }
        }

    }

    remove(m_pStopCmdFileName);
	
#endif
}

//
//		External Interface
//
void* NX_GetCommandHandle()
{
	NX_CCommand *pInst = new NX_CCommand();

	return (void*)pInst;
}

int NX_CreateCmdFile(void *pObj, const char *m_pCtrlFileName)
{
    int ret = 0;
    NX_CCommand *pInst = (NX_CCommand *)pObj;

    ret = pInst->CreateCmdFile(m_pCtrlFileName);

    return ret;
}

int NX_RemoveCmdFile(const char *m_pCtrlFileName)
{
    int ret = 0;

    if( 0 == access(m_pCtrlFileName, F_OK))
    {
    	ret = remove(m_pCtrlFileName);
    }

    return ret;
}

int NX_WriteCmd(void *pObj, const char *m_pCtrlFileName, bool quick_cmd)
{
    int ret = 0;
    NX_CCommand *pInst = (NX_CCommand *)pObj;

    ret = pInst->WriteCmd(m_pCtrlFileName, quick_cmd);

    return ret;
}

int NX_StartCommandService(void *pObj, const char *m_pCtrlFileName, bool quick_cmd)
{
	NX_CCommand *pInst = (NX_CCommand *)pObj;

    NxDbgMsg( NX_DBG_INFO, "%s()---- quick_cmd %d  cmd_file_name : %s\n", __FUNCTION__, quick_cmd, m_pCtrlFileName);
	pInst->StartService(m_pCtrlFileName, quick_cmd);
	return 0;
}

void NX_StopCommandService(void *pObj)
{
	NX_CCommand *pInst = (NX_CCommand*)pObj;

	pInst->StopService();

	usleep(100000);

	//delete pInst;
}

int NX_CheckReceivedStopCmd(void *pObj)
{
    NX_CCommand *pInst = (NX_CCommand*)pObj;

    return pInst->IsReceivedStopCmd();
}

int NX_QuickRearCamGetStatus(void *pObj)
{
    NX_CCommand *pInst = (NX_CCommand*)pObj;

    return pInst->GetQuickRearCamStatus();
}
