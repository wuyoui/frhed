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
 * @file  ReverseDlg.cpp
 *
 * @brief Implementation of the Data reversing dialog.
 *
 */
// ID line follows -- this is updated by SVN
// $Id$

#include "precomp.h"
#include "resource.h"
#include "hexwnd.h"
#include "hexwdlg.h"

INT_PTR ReverseDlg::DlgProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	char buf[128] = {0};
	int iStartOfSelSetting;
	int iEndOfSelSetting;
	int maxb;
	switch (m)
	{
	case WM_INITDIALOG:
		if (bSelected)
		{
			iStartOfSelSetting = iStartOfSelection;
			iEndOfSelSetting = iEndOfSelection;
		}
		else
		{
			iEndOfSelSetting = iStartOfSelSetting = iCurByte;
		}
		_snprintf(buf, RTL_NUMBER_OF(buf) - 1, "x%x", iStartOfSelSetting);
		SetDlgItemText (h, IDC_EDIT1, buf);
		_snprintf(buf, RTL_NUMBER_OF(buf) - 1, "x%x", iEndOfSelSetting);
		SetDlgItemText (h, IDC_EDIT2, buf);
		//Because we are using the Select block dialog template some things need to be changed
		SetWindowText(h, "Reverse bytes");
		#define IDC_STATIC      0xFFFF //Stupid bloody windows
		SetDlgItemText(h, IDC_STATIC, "Reverse bytes between and including");
		SetDlgItemText(h, IDC_STATIC2, "these two offsets (prefix x for hex)");
		return TRUE;

	case WM_COMMAND:
		switch (w)
		{
		case IDOK:
			if (GetDlgItemText(h, IDC_EDIT1, buf, 128) &&
				sscanf(buf, "x%x", &iStartOfSelSetting) == 0 &&
				sscanf(buf, "%d", &iStartOfSelSetting) == 0)
			{
				MessageBox(h, "Start offset not recognized.", "Reverse bytes", MB_ICONERROR);
				return TRUE;
			}
			if (GetDlgItemText(h, IDC_EDIT2, buf, 128) &&
				sscanf(buf, "x%x", &iEndOfSelSetting) == 0 &&
				sscanf(buf, "%d", &iEndOfSelSetting) == 0)
			{
				MessageBox(h, "End offset not recognized.", "Reverse bytes", MB_ICONERROR);
				return TRUE;
			}
			if (iEndOfSelSetting == iStartOfSelSetting)
			{
				MessageBox(h, "Cannot reverse the order of one byte.", "Reverse bytes", MB_ICONERROR);
				return TRUE;
			}
			maxb = DataArray.GetUpperBound();
			if (iStartOfSelSetting < 0 ||
				iStartOfSelSetting > maxb ||
				iEndOfSelSetting < 0 ||
				iEndOfSelSetting > maxb)
			{
				MessageBox(h, "The chosen block extends into non-existant data.\nThe offsets will be shifted to correct positions.", "Reverse bytes", MB_ICONERROR);
			}

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			if (iStartOfSelSetting < 0)
				iStartOfSelSetting = 0;
			if (iStartOfSelSetting > maxb)
				iStartOfSelSetting = maxb;
			if (iEndOfSelSetting < 0)
				iEndOfSelSetting = 0;
			if (iEndOfSelSetting > maxb)
				iEndOfSelSetting = maxb;
			if (iEndOfSelSetting < iStartOfSelSetting)
				swap(iEndOfSelSetting, iStartOfSelSetting);
			if (iStartOfSelSetting != iEndOfSelSetting)
				reverse_bytes(&DataArray[iStartOfSelSetting], &DataArray[iEndOfSelSetting]);
			if (bSelected)
			{
				//If the selection was inside the bit that was reversed, then reverse it too
				if (iStartOfSelSetting <= iStartOfSelection &&
					iStartOfSelSetting <= iEndOfSelection &&
					iEndOfSelSetting >= iStartOfSelection &&
					iEndOfSelSetting >= iEndOfSelection)
				{
					iStartOfSelection = iEndOfSelSetting - iStartOfSelection + iStartOfSelSetting;
					iEndOfSelection = iEndOfSelSetting - iEndOfSelection + iStartOfSelSetting;
				}
				else
				{
					bSelected = FALSE;
				}//If the above is not true deselect - this may change when multiple selections are allowed
			}
			//Or if the current byte was in the reversed bytes reverse it too
			else if (iCurByte >= iStartOfSelSetting &&
				iCurByte <= iEndOfSelSetting)
			{
				iCurByte = iEndOfSelSetting - iCurByte + iStartOfSelSetting;
				iCurNibble = !iCurNibble;
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			repaint();
			// fall through
		case IDCANCEL:
			EndDialog(h, w);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
