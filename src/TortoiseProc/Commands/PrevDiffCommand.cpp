// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2008-2011, 2016 - TortoiseGit
// Copyright (C) 2007-2008 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "stdafx.h"
#include "PrevDiffCommand.h"
#include "GitDiff.h"
#include "MessageBox.h"
#include "ChangedDlg.h"
#include "LogDlgHelper.h"
#include "FileDiffDlg.h"

bool PrevDiffCommand::Execute()
{
	//bool bAlternativeTool = !!parser.HasKey(_T("alternative"));
	if (this->orgCmdLinePath.IsDirectory())
	{
		CFileDiffDlg dlg;

		dlg.m_strRev1 = GIT_REV_ZERO;
		dlg.m_strRev2 = _T("HEAD~1");
		dlg.m_sFilter = this->cmdLinePath.GetGitPathString();

		dlg.DoModal();
		return true;
	}

	CLogDataVector revs;
	CLogCache cache;
	revs.m_pLogCache = &cache;
	revs.ParserFromLog(&cmdLinePath, 2, CGit::LOG_INFO_ONLY_HASH, nullptr, CRegDWORD(_T("Software\\TortoiseGit\\LogOrderBy"), CGit::LOG_ORDER_TOPOORDER));

	if (revs.size() != 2)
	{
		CMessageBox::Show(hwndExplorer, IDS_ERR_NOPREVREVISION, IDS_APPNAME, MB_ICONERROR);
		return false;
	}

	CGitDiff diff;
	return !!diff.Diff(&cmdLinePath, &cmdLinePath, GIT_REV_ZERO, revs.GetGitRevAt(1).m_CommitHash.ToString());
}
