/////////////////////////////////////////////////////////////////////////////
//    License (GPLv2+):
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but
//    WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/////////////////////////////////////////////////////////////////////////////
/** 
 * @file  hexwnd.cpp
 *
 * @brief Implementation of the About dialog.
 *
 */
// ID line follows -- this is updated by SVN
// $Id$

#include "precomp.h"
#include "resource.h"
#include "hexwnd.h"
#include "hexwdlg.h"

/**
 * @brief Initialize the dialog.
 * @param [in] hDlg Handle to the dialog.
 */
BOOL AboutDlg::OnInitDialog(HWND hDlg)
{
	TranslateDialog(hDlg);
	// Set the version information.
	SetDlgItemText(hDlg, IDC_STATIC1,
		"frhed - free hex editor for 32-bit Windows\nVersion "CURRENT_VERSION"."
		SUB_RELEASE_NO"."BUILD_NO"\n(c) Raihan Kibria 2000"
		"\nFill with by Pabs Dec 1999"
		"\nDisk-Access, Code/Decode Extension and some other bits by Gerson Kurz."
		"\nDLL interface by Jochen Neubeck.");
	// Set the email-addresses.
	SetDlgItemText(hDlg, IDC_EDIT1,
		"rkibria@hrz1.hrz.tu-darmstadt.de"
		"\r\nPabs: pabs3@zip.to");
	// Set the homepage URL.
	SetDlgItemText(hDlg, IDC_EDIT2, "http://www.kibria.de");
	// Set the icon.
	if (HWND hwndParent = GetParent(hDlg))
		if (DWORD dwIcon = GetClassLong(hwndParent, GCLP_HICON))
			SendDlgItemMessage(hDlg, IDC_APPICON, STM_SETICON, dwIcon, 0);
	return TRUE;
}

/**
 * @brief Handle dialog commands.
 * @param [in] hDlg Hanle to the dialog.
 * @param [in] wParam The command to handle.
 * @param [in] lParam Optional parameter for the command.
 * @return TRUE if the command was handled, FALSE otherwise.
 */
BOOL AboutDlg::OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDOK:
	case IDCANCEL:
		EndDialog(hDlg, wParam);
		return TRUE;

	case IDC_BUTTON1:
		{
			HINSTANCE hi = ShellExecute(hDlg, "open", "http://www.kibria.de", 0, NULL, SW_SHOWNORMAL);
			if ((UINT)hi <= HINSTANCE_ERROR)
				MessageBox(hDlg, "Could not call browser.", "Go to homepage", MB_ICONERROR);
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @brief Handle dialog messages.
 * @param [in] hDlg Handle to the dialog.
 * @param [in] iMsg The message.
 * @param [in] wParam The command in the message.
 * @param [in] lParam The optional parameter for the command.
 * @return TRUE if the message was handled, FALSE otherwise.
 */
INT_PTR AboutDlg::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return OnInitDialog(hDlg);

	case WM_COMMAND:
		return OnCommand(hDlg, wParam, lParam);
	}
	return FALSE;
}