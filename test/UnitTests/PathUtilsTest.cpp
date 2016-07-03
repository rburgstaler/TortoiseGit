// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2015-2016 - TortoiseGit
// Copyright (C) 2003-2008, 2013-2014 - TortoiseSVN

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
#include "PathUtils.h"

TEST(CPathUtils, UnescapeTest)
{
	CString test(_T("file:///d:/REpos1/uCOS-100/Trunk/name%20with%20spaces/NewTest%20%%20NewTest"));
	CString test2 = CPathUtils::PathUnescape(test);
	EXPECT_STREQ(L"file:///d:/REpos1/uCOS-100/Trunk/name with spaces/NewTest % NewTest", test2);
	CStringA test3 = CPathUtils::PathEscape("file:///d:/REpos1/uCOS-100/Trunk/name with spaces/NewTest % NewTest");
	EXPECT_STREQ("file:///d:/REpos1/uCOS-100/Trunk/name%20with%20spaces/NewTest%20%%20NewTest", test3);
}

TEST(CPathUtils, GetFileNameFromPath)
{
	CString test(L"d:\\test\\filename.ext");
	EXPECT_STREQ(L"filename.ext", CPathUtils::GetFileNameFromPath(test));
	test = L"filename.ext";
	EXPECT_STREQ(L"filename.ext", CPathUtils::GetFileNameFromPath(test));
	test = L"d:/test/filename";
	EXPECT_STREQ(L"filename", CPathUtils::GetFileNameFromPath(test));
	test = L"d:\\test\\filename";
	EXPECT_STREQ(L"filename", CPathUtils::GetFileNameFromPath(test));
	test = L"filename";
	EXPECT_STREQ(L"filename", CPathUtils::GetFileNameFromPath(test));
	test.Empty();
	EXPECT_TRUE(CPathUtils::GetFileNameFromPath(test).IsEmpty());
}

TEST(CPathUtils, ExtTest)
{
	CString test(L"d:\\test\\filename.ext");
	EXPECT_STREQ(L".ext", CPathUtils::GetFileExtFromPath(test));
	test = _T("filename.ext");
	EXPECT_STREQ(L".ext", CPathUtils::GetFileExtFromPath(test));
	test = L"d:\\test\\filename";
	EXPECT_TRUE(CPathUtils::GetFileExtFromPath(test).IsEmpty());
	test = _T("filename");
	EXPECT_TRUE(CPathUtils::GetFileExtFromPath(test).IsEmpty());
	test.Empty();
	EXPECT_TRUE(CPathUtils::GetFileExtFromPath(test).IsEmpty());
}

TEST(CPathUtils, ParseTests)
{
	CString test(_T("test 'd:\\testpath with spaces' test"));
	EXPECT_STREQ(L"d:\\testpath with spaces", CPathUtils::ParsePathInString(test));
	test = _T("d:\\testpath with spaces");
	EXPECT_STREQ(L"d:\\testpath with spaces", CPathUtils::ParsePathInString(test));
}

TEST(CPathUtils, MakeSureDirectoryPathExists)
{
	CAutoTempDir tmpDir;
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir()));
	EXPECT_FALSE(PathFileExists(tmpDir.GetTempDir() + L"\\sub"));
	EXPECT_FALSE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub"));

	EXPECT_TRUE(CPathUtils::MakeSureDirectoryPathExists(tmpDir.GetTempDir() + L"\\sub\\sub\\dir"));
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub"));
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub\\sub"));
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub\\sub\\dir"));

	EXPECT_TRUE(CPathUtils::MakeSureDirectoryPathExists(tmpDir.GetTempDir() + L"\\sub/asub/adir"));
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub"));
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub\\asub"));
	EXPECT_TRUE(PathIsDirectory(tmpDir.GetTempDir() + L"\\sub\\asub\\adir"));
}

TEST(CPathUtils, TrailingPathDelimiter)
{
	CString tPath = _T("C:\\my\\path");
	EXPECT_TRUE(CPathUtils::IncludeTrailingPathDelimiter(tPath).Compare(_T("C:\\my\\path\\")) == 0);
	tPath = _T("C:\\my\\path\\");
	EXPECT_TRUE(CPathUtils::IncludeTrailingPathDelimiter(tPath).Compare(_T("C:\\my\\path\\")) == 0);
	tPath = _T("C:\\my\\path");
	EXPECT_TRUE(CPathUtils::ExcludeTrailingPathDelimiter(tPath).Compare(_T("C:\\my\\path")) == 0);
	tPath = _T("C:\\my\\path\\");
	EXPECT_TRUE(CPathUtils::ExcludeTrailingPathDelimiter(tPath).Compare(_T("C:\\my\\path")) == 0);
}

TEST(CPathUtils, ExpandFileName)
{
	//It should eliminate ..
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("C:\\my\\path\\..\\da\\da\\da")).Compare(_T("C:\\my\\da\\da\\da")) == 0);
	//It should eliminate .
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("C:\\my\\path\\.\\da\\da\\da")).Compare(_T("C:\\my\\path\\da\\da\\da")) == 0);
	//It should not mess with trailing path delimiter if it is there
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("C:\\my\\path\\..\\da\\da\\da\\")).Compare(_T("C:\\my\\da\\da\\da\\")) == 0);
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("C:\\my\\path\\.\\da\\da\\da\\")).Compare(_T("C:\\my\\path\\da\\da\\da\\")) == 0);


	//It should support UNC file paths::: ALL PATHS FROM ABOVE WILL PASS ASSUMING UNC ROOT
	//It should eliminate ..
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("\\\\DACOMPUTER\\my\\path\\..\\da\\da\\da")).Compare(_T("\\\\DACOMPUTER\\my\\da\\da\\da")) == 0);
	//It should eliminate .
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("\\\\DACOMPUTER\\my\\path\\.\\da\\da\\da")).Compare(_T("\\\\DACOMPUTER\\my\\path\\da\\da\\da")) == 0);
	//It should not mess with trailing path delimiter if it is there
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("\\\\DACOMPUTER\\my\\path\\..\\da\\da\\da\\")).Compare(_T("\\\\DACOMPUTER\\my\\da\\da\\da\\")) == 0);
	EXPECT_TRUE(CPathUtils::ExpandFileName(_T("\\\\DACOMPUTER\\my\\path\\.\\da\\da\\da\\")).Compare(_T("\\\\DACOMPUTER\\my\\path\\da\\da\\da\\")) == 0);
}