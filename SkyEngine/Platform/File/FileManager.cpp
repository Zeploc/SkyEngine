#include "FileManager.h"

#include <fstream>
#include <windows.h>
#include <shobjidl.h>
#include <filesystem>

#include "PathUtils.h"
#include "Core/Application.h"
#include "Core/StringUtils.h"
#include "Platform/PlatformInterface.h"

bool CFileManager::OpenFile(std::string& OpenedFile, const std::string& Extension, std::string DefaultFolder)
{
	// TODO: Open dialogue (Currently makes file not openable?)
	// use IFileSaveDialog
	// https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
	
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
			COINIT_DISABLE_OLE1DDE);
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	IFileOpenDialog *pFileOpen;
	
	// Create the FileOpenDialog object.
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	if (!DefaultFolder.empty())
	{
		IShellItem *pCurFolder = NULL;
		const std::wstring Wstring = StringUtils::ToWString(DefaultFolder);
		const wchar_t* Folder = Wstring.c_str();
		hr = SHCreateItemFromParsingName(Folder, NULL, IID_PPV_ARGS(&pCurFolder));
		if (SUCCEEDED(hr))
		{
			pFileOpen->SetDefaultFolder(pCurFolder);
			pCurFolder->Release();
		}
	}

	if (!Extension.empty())
	{
		// TODO: Get working
		const std::wstring Wstring = StringUtils::ToWString(Extension);
		const wchar_t* WExtension = Wstring.c_str();
		pFileOpen->SetDefaultExtension(WExtension);
	}
	
	// Show the Open dialog box.
	hr = pFileOpen->Show(NULL);

	// Get the file name from the dialog box.
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	
	IShellItem *pItem;
	hr = pFileOpen->GetResult(&pItem);
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	wchar_t* pszFilePath;
	hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	// Display the file name to the user.
	if (SUCCEEDED(hr))
	{
		std::wstring ws(pszFilePath);
		OpenedFile = std::string(ws.begin(), ws.end());
		// GetPlatformInterface()->DisplayMessageBox("File Path", OpenedFile, MB_OK);
		CoTaskMemFree(pszFilePath);
	}
	pItem->Release();
	pFileOpen->Release();
	
	// OPENFILENAME ofn;
	// ZeroMemory(&ofn, sizeof(ofn));
	// char szFile[100];
	// ofn.lStructSize = sizeof(ofn);
	// ofn.hwndOwner = NULL; //CameraManager::GetInstance()->MainWindow
	// ofn.lpstrFile = szFile;
	// ofn.lpstrFile[0] = '\0';
	// ofn.nMaxFile = sizeof(szFile);
	// ofn.lpstrFilter = "Level (*.slvl)\0*.slvl\0Text\0*.TXT\0";
	// ofn.nFilterIndex = 1;
	// ofn.lpstrFileTitle = NULL;
	// ofn.nMaxFileTitle = 0;
	// ofn.lpstrInitialDir = "Resources/Levels/";
	// ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// if (!GetOpenFileNameA(&ofn))
	// {
	// 	// MessageBox(NULL, ofn.lpstrFile, "File Name", MB_OK);
	// 	return false;
	// }

	/*std::filesystem::path CurrentPath(szFile);
	std::filesystem::path RootResources("D:\\Projects\\OpenGL\\Graphics Combination\\Graphics Combination\\");
	std::filesystem::path relativePath = CurrentPath.lexically_relative(RootResources);
	std::string relative = relativePath.string();
	std::replace(relative.begin(), relative.end(), '\\', '/');*/
	// SceneName == "Example" ? "Resources/Levels/Demo Scene.slvl" : "Resources/Levels/Example.slvl";// = szFile;
	
	// OpenedFile = FileName;
	return true;
}

bool CFileManager::FileExists(const std::string& FilePath)
{
	std::ifstream OpenedFileStream(FilePath);//szFile); //
	return OpenedFileStream.is_open();
}

bool CFileManager::ReadFile(const std::string& FilePath, std::string& FileContents)
{
	std::string line;
	std::ifstream OpenedFileStream(FilePath);//szFile); //
	if (!OpenedFileStream.is_open())
	{
		GetPlatformInterface()->DisplayMessageBox("Failed to open", FilePath, MB_OK);
		return false;
	}

	std::stringstream ss;
	ss << OpenedFileStream.rdbuf();
	OpenedFileStream.close();
	FileContents = ss.str();
	return true;
}

bool CFileManager::SaveFile(const std::string& FilePath, std::string FileContents)
{
	// TODO: Go through parent directories as this one does the last
	// Confirm directory exists
	std::filesystem::create_directory(PathUtils::GetDirectory(FilePath));
	std::ofstream SaveFile;
	SaveFile.open(FilePath);
	if (!SaveFile.is_open())
	{
		GetPlatformInterface()->DisplayMessageBox("Failed to edit", FilePath.c_str(), MB_OK);
		return false;
	}
	SaveFile << FileContents;

	SaveFile.close();
	return true;
}

bool CFileManager::SaveAsFile(std::string& ChosenFilePath, std::string FileExtension, std::string FileName, std::string FileDirectory)
{
	if (FileName.empty())
	{
		FileName = "Untitled";
	}
	if (FileDirectory.empty())
	{
		FileDirectory = "C://";
	}
	//IFileSaveDialog
	
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	char szFile[100];
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = LPWSTR(szFile);
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = LPCWSTR(FileExtension.c_str());
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = LPWSTR(FileName.c_str());
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = LPCWSTR(FileDirectory.c_str());
	ofn.Flags = OFN_PATHMUSTEXIST;// | OFN_FILEMUSTEXIST;
	if (!GetSaveFileNameA(LPOPENFILENAMEA(&ofn)))
	{
		// MessageBox(NULL, ofn.lpstrFile, "File Name", MB_OK);
		return false;
	}

	// TODO: Prompt for overwrite
	
	// GetPlatformInterface()->DisplayMessageBox("Saved file", LPCSTR(ofn.lpstrFile), MB_OK);
	// MessageBoxA(NULL, LPCSTR(ofn.lpstrDefExt), "File Name (WIP)", MB_OK);
	// MessageBoxA(NULL, LPCSTR(ofn.nFileExtension), "File Name (WIP)", MB_OK);

	ChosenFilePath = std::string(LPCSTR(ofn.lpstrFile));
	// std::wstring FilePath();
	return true;	
}

namespace fs = std::filesystem;
TArray<std::string> CFileManager::ListFilesInDirectory(const std::string& DirectoryPath, bool bInRecursive)
{
	TArray<std::string> FoundFiles;
	for (const auto & entry : fs::directory_iterator(DirectoryPath))
	{
		if (entry.is_directory())
		{
			if (bInRecursive)
			{
				TArray<std::string> SubFiles = ListFilesInDirectory(entry.path().string(), bInRecursive);
				for (std::string SubFile : SubFiles)
				{
					FoundFiles.push_back(SubFile);
				}
			}
		}
		else
		{
			std::string FullPath = entry.path().string();
			FoundFiles.push_back(FullPath);
		}
	}
	
	// std::string search_path = DirectoryPath + "*.*";
	// WIN32_FIND_DATA fd; 
	// HANDLE hFind = ::FindFirstFile(LPCWSTR(search_path.c_str()), &fd); 
	// if(hFind != INVALID_HANDLE_VALUE) { 
	// 	do { 
	// 		// read all (real) files in current folder
	// 		// , delete '!' read other 2 default folder . and ..
	// 		if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
	// 			FoundFiles.push_back(std::string(std::begin(fd.cFileName), std::end(fd.cFileName)));
	// 		}
	// 	}while(::FindNextFile(hFind, &fd)); 
	// 	::FindClose(hFind); 
	// }
	return FoundFiles;
}
