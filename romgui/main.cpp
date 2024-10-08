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

//---------------------------------------------------------------------------

#include "vcl-shim/vcl-shim.h"
#include <shellapi.h>
#pragma hdrstop

#include "main.h"
#include "addgame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
    CreateControls();

    _tcscpy(szPath,GetCurrentDir().c_str());
    if(!FileExists(ProgramDir+_T("\\gbinjectb.exe")))
    	GetProgramPath();
    int l=_tcslen(szPath);
    if(szPath[l-1]=='\\')
    	szPath[l-1]='\0'; 
    ProgramDir=VclString(GetProgramPath());

    tbInput->Text	= ProgramDir+_T("\\gbagi.bin");
    tbVocab->Text	= ProgramDir+_T("\\vocab.bin");
    tbOutput->Text	= ProgramDir+_T("\\agigames.gba");

	DirDialog = new TDirDialog;

    addGameFirst	=
    addGamePtr 		= NULL;

	FormResize(this);
    UpdateControls();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
	delete DirDialog;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::UpdateControls()
{
	btnBuild->Enabled 	= (tbOutput->Text!=_T("") && listbox->Items->Count > 0);
	btnRemove->Enabled 	= (listbox->Items->Count > 0);

	// Generated;
	this->Update();
	btnBuild->Update();
	btnRemove->Update();
	tbOutput->Update();
	tbInput->Update();
	tbVocab->Update();
	txStatus->Update();
	listbox->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormResize(TObject *Sender)
{
	tbInput->Width	= ClientWidth-tbInput->Left - btnBrowseInp->Width;
	tbVocab->Width 	= ClientWidth-tbVocab->Left - btnBrowseVoc->Width;
	tbOutput->Width	= ClientWidth-tbOutput->Left- btnBrowseOut->Width;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnAddClick(TObject *Sender)
{
	if(listbox->Items->Count >= 16) {
     	ShowMessage(_T("Maximum games added to game list"));
        return;
    }

	// Get the directory which the game is located
    DirDialog->Title = _T("Add Game");
    DirDialog->Caption = _T("Please select the directory which the game you wish to add is located in.");
    DirDialog->MAP_CHECK = TRUE;
    if(!DirDialog->Execute()) {
        return;
    }
	DirDialog->InitialDir = DirDialog->FullPath;

    TFormAddGame *ag = new TFormAddGame(this);
    ag->SetUp(DirDialog->FullPath);
    ag->ShowModal();

    if(ag->okClose) {
    	TAddGameObj *go = new TAddGameObj();
        go->prev = addGamePtr;
        go->next = NULL;
      	if(!addGameFirst) {
          	addGameFirst = go;
        }
      	if(addGamePtr) {
          	addGamePtr->next = go;
        }
        addGamePtr = go;

        memcpy(&go->gameinfo,&ag->gameinfo,sizeof(go->gameinfo));
        go->itemindex = listbox->Items->Add(go->gameinfo.title);

        UpdateControls();
    }

    delete ag;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnBrowseInpClick(TObject *Sender)
{
	if(dlgOpenInp->Execute()) {
     	tbInput->Text = dlgOpenInp->FileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnBrowseVocClick(TObject *Sender)
{
	if(dlgOpenVoc->Execute()) {
     	tbVocab->Text = dlgOpenVoc->FileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnBrowseOutClick(TObject *Sender)
{
	if(dlgSaveOut->Execute()) {
     	tbOutput->Text = dlgSaveOut->FileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnRemoveClick(TObject *Sender)
{
	if(listbox->ItemIndex != -1) {
    	RemoveAddGame(listbox->ItemIndex);
        listbox->Items->Delete(listbox->ItemIndex);
    }
}
//---------------------------------------------------------------------------
TAddGameObj *TFormMain::FindAddGame(int num)
{
 	TAddGameObj *o = addGameFirst;
    while(o) {
     	if(!num--)
        	break;
        o = o->next;
    }
    return o;
}
//---------------------------------------------------------------------------
int TFormMain::RemoveAddGame(int num)
{
	TAddGameObj *o = FindAddGame(num);

    if(o==NULL) return -1;

    if(o->prev)
    	o->prev->next = o->next;
    if(o->next)
    	o->next->prev = o->prev;
    if(o == addGameFirst)
    	addGameFirst = o->next;
    if(o == addGamePtr)
    	addGamePtr = o->next?o->next:o->prev;


    free((void*) o->gameinfo.title );
    free((void*) o->gameinfo.path );

    delete o;

    UpdateControls();

    return 0;
}
//---------------------------------------------------------------------------
void TFormMain::RemoveAddGames()
{
	while(RemoveAddGame(0) != -1)
        listbox->Items->Delete(0);
    addGameFirst	=
    addGamePtr 		= NULL;
}
//---------------------------------------------------------------------------
BOOL TFormMain::PackGames()
{
	int i, l;
	FILE *fin;

	int totalGames = listbox->Items->Count;

	inromName	= _tcsdup(tbInput->Text.c_str());
	outromName	= _tcsdup(tbOutput->Text.c_str());
	vocabName	= _tcsdup(tbVocab->Text.c_str());

	if(FileExists(vocabName) == false || (fin=_tfopen(vocabName,_T("rb")))==NULL) {
 		ShowMessage(_T("Error opening vocab definition file! Please specify the location of the file included with this program by clicking on the \"Browse...\" button."));
		return FALSE;
	}
	fclose(fin);
	if(FileExists(inromName) == false || (fin=_tfopen(inromName,_T("rb")))==NULL) {
 		ShowMessage(_T("Error opening input rom! Please specify the location of the file included with this program by clicking on the \"Browse...\" button."));
		return FALSE;
	}
	if((fout=_tfopen(outromName,_T("wb")))==NULL) {
    	fclose(fin);
 		ShowMessage(_T("Error opening file for output!"));
		return FALSE;
	}

	fseek(fin,0,SEEK_END);
	l=ftell(fin);
	fseek(fin,0,SEEK_SET);
	for(i=0;i<l;i++)
    	fputc(fgetc(fin),fout);
	fclose(fin);
	S32 BASEx0X = (l + AGI_DATA_ALIGNMENT-1) & -AGI_DATA_ALIGNMENT;
	for(i=l;i<BASEx0X;i++)
    	fputc(0xFF,fout);
	U32 BASE80X = BASE800 + BASEx0X;

	offs = BASE80X;

  	fwrite(agiid,IDSIZE,1,fout);
	fputc(totalGames,fout);   
	for(i=IDSIZE+1;i<0x20;i++)
    	fputc(0,fout);
	offs += 0x20;

	giPos = ftell(fout);

	for(i=totalGames*80;i>0;i--)
    	fputc(0,fout);
	offs += totalGames*80;

	GAMEINFO ginm;
	listbox->ItemIndex = 0;
    TAddGameObj *gameobj = addGameFirst;
	while(gameobj) {
		//printf("Packing game: %s...\t\t\t",games[i].title);
        Repaint();
        Update();
		ginm.path = gameobj->gameinfo.path;
		ginm.title = gameobj->gameinfo.title;
		ginm.version = gameobj->gameinfo.version;
		ginm.vID = gameobj->gameinfo.vID;
		txStatus->Caption = _T("Adding Game: ")+VclString(gameobj->gameinfo.title);
		if(!ProcessGame(&ginm)) {
    		FreeGame();
    		fclose(fout);
			ShowMessage(_T("Error adding game: ")+VclString(gameobj->gameinfo.title));
			return FALSE;
		}
    	FreeGame();
		if(gameobj->next)
			listbox->ItemIndex++;
     	gameobj = gameobj->next;
	}

	fclose(fout);

	return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnBuildClick(TObject *Sender)
{
	if(tbOutput->Text == _T("") || tbInput->Text == _T("")) {
		ShowMessage(_T("You must specify an input and output file name!"));
	} else {
	if(listbox->Items->Count == 0) {
		ShowMessage(_T("You must add games to embed in the ROM!"));
	} else {
		Enabled = false;
		if(PackGames()) {
			ShowMessage(_T("ROM Build finished. Enjoy!"));

            tbOutput->Text = _T("");
            RemoveAddGames();
    		UpdateControls();
		}
		txStatus->Caption = _T("");
		delete inromName;
		delete outromName;
		delete vocabName;
		Enabled = true;
	}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tbOutputChange(TObject *Sender)
{
	UpdateControls();	
}
//---------------------------------------------------------------------------
// Get path where the current process (.EXE) is located.
// This may be different from the current directory.
LPCTSTR TFormMain::GetProgramPath()
{
	int i;
	BOOL bInString = FALSE;
	static TCHAR szPath[4096];

	_tcscpy(szPath, GetCommandLine());

	// Extract first argument
	for (i = 0; i < (int)_tcslen(szPath); i++)
	{
		if (szPath[i] == '"')
			bInString = !bInString;

		if (szPath[i] == ' ' && !bInString)
		{
			szPath[i] = 0;
			break;
		}
	}

	// Remove file name
	bool bFoundPath = FALSE;

	for (i = _tcslen(szPath) - 1; i >= 0; i--)
		if (szPath[i] == '\\')
		{
			szPath[i] = 0;
			bFoundPath = TRUE;
			break;
		}

	if (!bFoundPath)
	{
		// Command line does not contain path.
		// Assume working directory.
		GetCurrentDirectory(MAX_PATH, szPath);
	}

	// Remove leading quote, if any
	if (szPath[0] == '"')
		return szPath + 1;
	else
		return szPath;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject *Sender)
{
    tbOutput->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Label3Click(TObject *Sender)
{
	// This URL was originally "http://www.bripro.com", but that is a broken link now.
	// An archive of its last state can be seen at:
	// https://web.archive.org/web/20090512184610/http://www.bripro.com:80/gbagi/index.php
	ShellExecute(NULL, NULL, _T("https://github.com/Davidebyzero/GBAGI"), NULL, NULL, 0);
}
//---------------------------------------------------------------------------

