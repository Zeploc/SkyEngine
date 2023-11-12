#include "FileManager.h"

#include <fstream>
#include <windows.h>
#include <shobjidl.h> 

#include "Core/Application.h"
#include "Platform/PlatformInterface.h"

bool CFileManager::OpenFile(std::string& OpenedFile)
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
