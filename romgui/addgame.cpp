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
#pragma hdrstop

#include "addgame.h"
#include "main.h"

#if defined(_MSC_VER) 
#define strdup _strdup
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAddGame::TFormAddGame(TComponent* Owner)
	: TForm(Owner)
{
	CreateControls();

	okClose = FALSE;
}
//---------------------------------------------------------------------------
void TFormAddGame::UpdateControls()
{
	// Generated
	this->Update();
	dropGames->Update();
	dropVersion->Update();
	tbId->Update();
	tbName->Update();
	tbPath->Update();
	btnAutodetect->Update();
}
//---------------------------------------------------------------------------
void TFormAddGame::SetUp()
{
	tbPath->Text = U2T(gameinfo.path);
	tbName->Text = U2T(gameinfo.title);
	tbId->Text = U2T(gameinfo.vID);

	dropGames->Items->Clear();
	dropVersion->Items->Clear();

	std::vector<VclString> flags;
	if ((gameinfo.flags && AMIGA) != 0) {
		flags.push_back(_T("Amiga"));
	}

	if ((gameinfo.flags && SINGLE_DIR) != 0) {
		if ((gameinfo.flags && SINGLE_DIR_DIRS) != 0) {
			flags.push_back(_T("Dirs"));
		}
		else {
			flags.push_back(_T("Dir"));
		}
	}
	else {
		flags.push_back(_T("Dirfiles"));
	}

	if ((gameinfo.flags && PACKED_DIRS) != 0) {
		flags.push_back(_T("Packed"));
	}

	VclString version;
	for (const VclString& flag : flags) {
		if (!version.empty()) version += _T(", ");
		version += flag;
	}

	dropGames->Items->Add(_T("<Other>"));
	dropVersion->Items->Add(VclString(_T("Autodetected: ")) + version);

	dropGames->ItemIndex = 0;
	dropVersion->ItemIndex	= 0;
	tbId->Enabled = false;
	dropGames->Enabled = false;
	dropVersion->Enabled = false;
	btnAutodetect->Enabled = false;

	UpdateControls();
}
//---------------------------------------------------------------------------
void __fastcall TFormAddGame::btnCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAddGame::btnOKClick(TObject *Sender)
{
	okClose = TRUE;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAddGame::dropVersionChange(TObject *Sender)
{
	UpdateControls();
}
//---------------------------------------------------------------------------
void __fastcall TFormAddGame::btnAutodetectClick(TObject *Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAddGame::dropGamesChange(TObject *Sender)
{
}
//---------------------------------------------------------------------------
