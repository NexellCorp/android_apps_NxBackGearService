//
// Created by jmbahn on 19. 8. 21.
//

#ifndef NXBACKGEARSERVICE_NXCOMMAND_H
#define NXBACKGEARSERVICE_NXCOMMAND_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum{
	STOP = 1,
	RUN,
	MAX_COMMAND
};

void* NX_GetCommandHandle();
int NX_CreateCmdFile(void*, const char*);
int NX_RemoveCmdFile(const char*);
int NX_WriteCmd(void*, const char*, bool);
int NX_StartCommandService(void*, const char*, bool);
void NX_StopCommandService(void*);
int NX_CheckReceivedStopCmd(void*);
int NX_QuickRearCamGetStatus(void *);
#ifdef __cplusplus
}
#endif



#endif //NXBACKGEARSERVICE_NXCOMMAND_H




