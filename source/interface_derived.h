#pragma once

//
//  interface_derived.h
//
//  Copyright © 2019 Ravbug. All rights reserved.
//
// This file contains the definition for GUI events and methods
// Define the user interface elements with wxFormBuilder

#include "globals.h"
#include "interface.h"
#include "folder_sizer.hpp"
#include "FolderDisplay.hpp"
#include <thread>
#include <unordered_set>
#include <wx/treebase.h>
#include <wx/clipbrd.h>

using namespace std;

class StructurePtrData;

/**
 Defines the main window and all of its behaviors and members.
 */
class MainFrame : public MainFrameBase
{
public:
	MainFrame(wxWindow* parent = nullptr);
	/**
	Log a message to the console
	@param msg the string to log
	*/
	void Log(const wxString& msg) {
		logCtrl->AppendText(msg + "\n");
		if (!userClosedLog && !browserSplitter->IsSplit()) {
			auto e = wxCommandEvent();
			OnToggleLog(e);
		}
	}
	void OnLog(wxCommandEvent& evt) {
		Log(evt.GetString());
	}
	void PopulateSidebar(DirectoryData*);
	
	void ChangeSelection(DirectoryData*);
	
	FolderDisplay* AddDisplay(DirectoryData* model){
		FolderDisplay* f = new FolderDisplay(scrollView,this,model);
		int count = (int)scrollSizer->GetItemCount();
		scrollSizer->SetCols(++count);
		scrollSizer->Add(f, wxGBPosition( 0, count-1), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 0);
		if (!scrollSizer->IsColGrowable(count-1)){
			scrollSizer->AddGrowableCol(count-1);
		}
		//update the sizer
		scrollSizer->Layout();
		
		//force-update the scrolled window
		wxSize size = scrollView->GetBestVirtualSize();
		scrollView->SetVirtualSize( size );
		
		//scroll to the end of the window
		scrollView->Scroll(size.GetWidth(),0);
		
		currentDisplay.push_back(f);
		return f;
	}
	
	DirectoryData* selected = nullptr;
	
private:
	DirectoryData* folderData = nullptr;
	unordered_set<string> loaded;
	int progIndex = 0;
	wxDataViewItem lastUpdateItem;
	bool userClosedLog = false;

	string GetPathFromDialog(const string&);
	void SizeRootFolder(const string&);
	
	vector<FolderDisplay*> currentDisplay;
	
	void OnExit(wxCommandEvent&);
	void OnAbout(wxCommandEvent&);
	void OnOpenFolder(wxCommandEvent&);
	void OnReloadFolder(wxCommandEvent&);
	void OnUpdateUI(wxCommandEvent&);
	void OnUpdateReload(wxCommandEvent&);
	void OnCopy(wxCommandEvent&);
	void OnToggleSidebar(wxCommandEvent&);
	void OnToggleLog(wxCommandEvent&);
	void OnReveal(wxCommandEvent&);


	void OnSourceCode(wxCommandEvent&){
		wxLaunchDefaultBrowser("https://github.com/ravbug/FatFileFinderCPP/");
	}
	void OnUpdates(wxCommandEvent& event){
		wxLaunchDefaultBrowser("https://github.com/ravbug/FatFileFinderCPP/releases/latest");
	}
	void OnAbort(wxCommandEvent& event) {
		//TODO: find any items with size operaions in progress, and signal them to stop
				//if any were found, then display the below message
		
//			wxMessageBox("File percentage calculations will be incorrect.\nReload an individual item to size it, or Open this folder and resize it to calculate all items.","Stopped Sizing");
	}
	void OnClearLog(wxCommandEvent& event) {
		logCtrl->SetValue("");
	}
	void OnCopyLog(wxCommandEvent& event) {
		//copy values to the clipboard
		if (wxTheClipboard->Open()) {
			wxTheClipboard->SetData(new wxTextDataObject(logCtrl->GetValue()));
			wxTheClipboard->Flush();
			wxTheClipboard->Close();
		}
	}
	void UpdateTitlebar(int prog, const string& size) {
		SetTitle(AppName + " v" + AppVersion + " - Sizing " + to_string(prog) + "% " + folderData->Path + " [" + size + "]");
	}
	wxDECLARE_EVENT_TABLE();
	
};
