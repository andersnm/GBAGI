/***************************************************************************
 *  GBAGI: The Game Boy Advance Adventure Game Interpreter
 *  Copyright (C) 2003,2004 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 ***************************************************************************/

/*****************************************************************************/
#include "gbagi.h"
#include "logic.h"
#include "commands.h"
#include "gamedata.h"
#include "errmsg.h"
#include "system.h"
#include "views.h"
#include "status.h"
#include "text.h"
#include "input.h"
#include "screen.h"
#include "picture.h"
/*****************************************************************************/
LOGIC *curLog,*log0;
BOOL IF_RESULT;
#ifdef _WINDOWS
#define _PRINT_LOG
#endif
U8 *code;
U16 logScan[256];
/*****************************************************************************/

char *GetMessage(LOGIC *log, int num)
{                                             
	return (char*)( log->messages + bGetW(log->messages+(num<<1)) );
}
/*****************************************************************************/   
#ifdef _PRINT_LOG
FILE *flog;
#endif
void InitLogicSystem()
{
	curLog 			= NULL;
    log0			= NULL;

	memset(logScan,0,sizeof(logScan));

#ifdef _PRINT_LOG
    flog=fopen("flog.txt","w");  
#endif
}
/*****************************************************************************/
U8 *CallLogic(U8 num)
{
	LOGIC *prevLog,log;
	U8 *pLog = (U8*)logDir[num];
    U8 *c=code,*c2;


#ifdef _FULL_BLIT_RESFRESHES
	EraseBlitLists();
	DrawBlitLists();
#endif

    // load the logic
    log.num			= num;
    log.code		= pLog+7;
    log.messages	= log.code+bGetW(pLog+5);
    log.msgTotal	= *log.messages++;

    // set the active pointer
    prevLog			= curLog;
    curLog			= &log;
    if(!num)
    	log0 = curLog;

	c2 = ExecuteLogic(curLog);

#ifdef _FULL_BLIT_RESFRESHES
	EraseBlitLists();
	DrawBlitLists();
#endif

    curLog = prevLog;
   	code = c;
    // release the pointer
	return (c2);
}
/*****************************************************************************/
#ifdef _PRINT_LOG
char zs[1000];
int cmdnum;
#endif
U8 *ExecuteLogic(LOGIC *log)
{
    register unsigned int op;
#ifdef _PRINT_LOG
    int i,l;
    cmdnum = 0;
#endif
	code = log->code+logScan[log->num];

	while(code && (BOOL)(op = *code++)) {
#ifdef _WINDOWS
	if(sndFlag!=-1) {
     	SetFlag(sndFlag);
        sndFlag=-1;
    }                         
    if((vars[vSECONDS]%30)==0)
                SystemUpdate();
    if(++vars[vSECONDS]>=60) {
    	vars[vSECONDS]=0;
        if(++vars[vMINUTES]>=60) {
    		vars[vMINUTES]=0;
        	if(++vars[vHOURS]>=60) {
    			vars[vHOURS]=0;
                vars[vDAYS]++;
            }
        }
    }
    if(sndBuf)
    	StopSound();
#endif
#ifdef _PRINT_LOG
		cmdnum++;
		sprintf(zs,"$%03d:%04X:\t%s ",log->num,(code-log->code)-1,agiCommands[op].name);
        for(i=0;i<agiCommands[op].nParams;i++) {
        	l=strlen(zs);
            if(i){
				sprintf(zs+l,",");
            	l++;}
            if((agiCommands[op].pMask<<i)&0x80) {
				sprintf(zs+l,"x%d(%d) ",code[i],vars[code[i]]);
            } else
				sprintf(zs+l,"%d",code[i]);
        }
        l=strlen(zs);
        sprintf(zs+l,"");
        fprintf(flog,"%s\n",zs);
#endif
		agiCommands[op].func();
	}
	return code;
}
/*****************************************************************************/
void ExecuteGoto()
{
	code += (S16)(bGetW(code)+2);
}
/*****************************************************************************/
void ExecuteIF()
{
	BOOL IS_NOT = FALSE;
    register unsigned int orCnt=0;
	register unsigned int op;
#ifdef _PRINT_LOG
    int i,l;
#endif

	for(;;) {
		if((op = *code++) >= 0xFC) {
			if(op == 0xFC) {
				if(orCnt) {
					orCnt = 0;
					SkipANDFalse();
					return;
				}
				orCnt++;
			} else if(op == 0xFF) {
				code+=2;
				return;
			}
			if(op == 0xFD) {
				IS_NOT = !IS_NOT;
#ifdef _PRINT_LOG
        fprintf(flog,"\t\tNOT\n");
#endif
            }
		} else {
#ifdef _PRINT_LOG
		sprintf(zs,"$%03d:%04X:\t\t%s ",curLog->num,(code-curLog->code)-1,testCommands[op].name);
        for(i=0;i<testCommands[op].nParams;i++) {
        	l=strlen(zs);
            if(i){
				sprintf(zs+l,",");
            	l++;}
            if((testCommands[op].pMask<<i)&0x80) {
				sprintf(zs+l,"x%d(%d) ",code[i],vars[code[i]]);
            } else
				sprintf(zs+l,"%d ",code[i]);
        }
        l=strlen(zs);
#endif
        	testCommands[op].func();
			if(IS_NOT)
            	IF_RESULT	= !IF_RESULT;
			IS_NOT		= FALSE;
#ifdef _PRINT_LOG
        fprintf(flog,"%s\n\t\t\t=%s\n",zs,IF_RESULT?"TRUE":"FALSE");
#endif

			if(IF_RESULT) {
				if(orCnt) {
					orCnt = 0;
					SkipORTrue();
				}
			} else if(!orCnt) {
				SkipANDFalse();
				return;
			}
		}
	}
}
/*****************************************************************************/

void SkipORTrue()
{
	register unsigned int op;
	while((op = *code++) != 0xFC)
		if(op <= 0xFC)
			code += (op == 0xE)?(*code << 1) + 1:testCommands[op].nParams;
}
/*****************************************************************************/

void SkipANDFalse()
{
	register unsigned int op;
	while((op = *code++) != 0xFF)
		if(op < 0xFC)
			code += (op == 0xE)?(*code << 1) + 1:testCommands[op].nParams;

	code += (S16)(bGetW(code)+2);	// address
}
/*****************************************************************************/
U8 *NewRoom(U8 num)
{
	VOBJ *vObj;

	for (vObj=ViewObjs; vObj<&ViewObjs[MAX_VOBJ]; vObj++) {
		vObj->flags			&= ~(oANIMATE|oDRAWN);
		vObj->flags			|= oUPDATE;
		vObj->pCel			= NULL;
		vObj->pView			= NULL;
        vObj->blit			= NULL;
		vObj->stepTime		=
		vObj->stepCount		=
		vObj->cycleCount	=
		vObj->cycleTime		=
		vObj->stepSize		= 1;
	}

    StopSound();
    ClearControllers();
    
    pPView		= pViews;
    pOverlay	= overlays;

	PLAYER_CONTROL	= TRUE;
	VOBJ_BLOCKING	= FALSE;
	horizon			= HORIZON_DEFAULT;

	vars[vROOMPREV]		= vars[vROOMNUM];
	vars[vROOMNUM]		= num;
	vars[vOBJECT]		=
	vars[vOBJBORDER]	= 0;
	vars[vMEMORY]		= 10;
	vars[vEGOVIEWNUM]	= ViewObjs[0].view;

	switch(vars[vEGOBORDER]) {
   		case bdTOP: 	// coming from the top, go to the bottom
			ViewObjs[0].y = PIC_MAXY;
			break;
		case bdRIGHT:	// coming from the right, go to the left
			ViewObjs[0].x = 0;
			break;
		case bdBOTTOM:	// coming from the bottom, go up to the top
			ViewObjs[0].y = horizon;
			break;
		case bdLEFT:	// coming from the left, go to the right
			ViewObjs[0].x = PIC_WIDTH - ViewObjs[0].width;
	}
	vars[vEGOBORDER] = bdNONE;

	SetFlag(fNEWROOM);

	ClearControllers();
	WriteStatusLine();

	return NULL;
}
/*****************************************************************************/
