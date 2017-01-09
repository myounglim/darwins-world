//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include <wx/wfstream.h>
#include "World.h"
#include <algorithm>

enum
{
	ID_SImSTART=1000,
	ID_TURN_TIMER,
	ID_LOAD_ZOMBIE = 2000,
	ID_LOAD_SURVIVOR = 2001,
	ID_RANDOMIZE = 2002,
	ID_RESET = 2003
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
	EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
	EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
	EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
	EVT_MENU(ID_RESET, ZomFrame::OnReset)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
	mSimMenu->Append(ID_LOAD_ZOMBIE, "Load zombie", "Load in a zombie file");
	mSimMenu->Append(ID_LOAD_SURVIVOR, "Load survivor", "Load in a survivor file");
	mSimMenu->Append(ID_RANDOMIZE, "Randomize", "A random configuration");
	mSimMenu->Append(ID_RESET, "Reset", "Reset the simulation");
	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	mSimMenu->Enable(ID_RESET, false);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine
	//zombieMachine.LoadMachine(std::string(""));
	//zombieMachine.BindState(zombieTestState);
	// END TEMP CODE
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
	World::get().mHumanState.clear();
	World::get().mZombieState.clear();
	World::get().mZombieMachine.LoadMachine("");
	World::get().mHumanMachine.LoadMachine("");
	World::get().mHumanMachine.numTurns = 0; 
	mTurnTimer->Stop();
	World::get().mZombieMachine.loadedFile = "";
	World::get().mHumanMachine.loadedFile = "";
	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	mSimMenu->Enable(ID_RESET, false);
	mPanel->PaintNow();
	
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
	//zombieMachine.TakeTurn(zombieTestState);
	// END TEMP CODE
	int size = World::get().mZombieState.size();
	size_t zombieStopper = 0; 
	for (int i = 0; i < size; i++) 
	{
		if (zombieStopper >= World::get().mZombieState.size())
			break;
		try {
			World::get().mZombieMachine.TakeTurn(*World::get().mZombieState[i]);
		}
		catch (std::exception&) {
			wxMessageBox("Zombies can't attack ranged! Pausing the program", "Error", wxOK | wxICON_ERROR);
			mTurnTimer->Stop();
			mPanel->PaintNow();
		}
		zombieStopper++; 
	}

	int humanSize = World::get().mHumanState.size();
	size_t humanStopper = 0; 
	for (int i = 0; i < humanSize; i++) 
	{
		if (humanStopper >= World::get().mHumanState.size())
			break;
		World::get().mHumanMachine.TakeTurn(World::get().mHumanState[i]);
		humanStopper++; 
	}
	mPanel->PaintNow();
	World::get().mHumanMachine.numTurns++; 

	if (World::get().mHumanState.size() == 0)
	{
		mTurnTimer->Stop();
		wxMessageBox(wxT("Zombies won"));
	}
	else if (World::get().mZombieState.size() == 0)
	{
		mTurnTimer->Stop();
		wxMessageBox(wxT("Humans won"));
	}
}

//populates with a single zombie on loading of a file for this implementation
//random will populate with the 20zombies/10humans configuration if pressed
void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open Zom File"), "./data", "",
		"ZOM files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// proceed loading the file chosen by the user;
					// this can be done with e.g. wxWidgets input streams:
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}

	World::get().mZombieMachine.LoadMachine(openFileDialog.GetPath().ToStdString());
	World::get().mZombieMachine.loadedFile = openFileDialog.GetPath().ToStdString();
	
	World::get().mZombieState.push_back(std::make_shared<MachineState>());
	World::get().mZombieMachine.BindState(*World::get().mZombieState[0]);
	mPanel->PaintNow();
}

//populates with a single zombie on loading of a file for this implementation
//random will populate with the 20zombies/10humans configuration if pressed
void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open Zom File"), "./data", "",
		"ZOM files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// proceed loading the file chosen by the user;
					// this can be done with e.g. wxWidgets input streams:
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}

	World::get().mHumanMachine.LoadMachine(openFileDialog.GetPath().ToStdString());
	World::get().mHumanMachine.loadedFile = openFileDialog.GetPath().ToStdString();
	World::get().mHumanState.push_back(MachineState());
	World::get().mHumanState[0].mXPosition = 19; 
	World::get().mHumanMachine.BindState(World::get().mHumanState[0]);

	mSimMenu->Enable(ID_SImSTART, true);
	mSimMenu->Enable(ID_RANDOMIZE, true);
	mSimMenu->Enable(ID_RESET, true);
	mPanel->PaintNow();
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
	World::get().mHumanState.clear();
	World::get().mZombieState.clear();
	std::vector<wxPoint> occupied; 
	//random zombies
	for (int i = 0; i < 20; i++) 
	{
		World::get().mZombieState.push_back(std::make_shared<MachineState>());
		int randNum = rand() % 4; 
		if (randNum == 0)
			World::get().mZombieState[i]->mFacing = MachineState::UP;
		else if (randNum == 1)
			World::get().mZombieState[i]->mFacing = MachineState::RIGHT;
		else if (randNum == 2)
			World::get().mZombieState[i]->mFacing = MachineState::DOWN;
		else
			World::get().mZombieState[i]->mFacing = MachineState::LEFT;

		int randX = rand() % 20;
		int randY = rand() % 20; 
		wxPoint position(randX, randY);
		while (std::find(occupied.begin(), occupied.end(), position) != occupied.end()) {
			randX = rand() % 20;
			randY = rand() % 20;
			position.x = randX; position.y = randY; 
		}
		occupied.push_back(position);

		World::get().mZombieState[i]->mXPosition = randX;
		World::get().mZombieState[i]->mYPosition = randY; 
		World::get().mZombieMachine.BindState(*World::get().mZombieState[i]);
	}

	//random humans 
	for (int i = 0; i < 10; i++)
	{
		World::get().mHumanState.push_back(MachineState());
		int randNum = rand() % 4;
		if (randNum == 0)
			World::get().mHumanState[i].mFacing = MachineState::UP;
		else if (randNum == 1)
			World::get().mHumanState[i].mFacing = MachineState::RIGHT;
		else if (randNum == 2)
			World::get().mHumanState[i].mFacing = MachineState::DOWN;
		else
			World::get().mHumanState[i].mFacing = MachineState::LEFT;

		int randX = rand() % 20;
		int randY = rand() % 20;
		wxPoint position(randX, randY);
		while (std::find(occupied.begin(), occupied.end(), position) != occupied.end()) {
			randX = rand() % 20;
			randY = rand() % 20;
			position.x = randX; position.y = randY;
		}
		occupied.push_back(position);
		World::get().mHumanState[i].mXPosition = randX;
		World::get().mHumanState[i].mYPosition = randY;
		World::get().mHumanMachine.BindState(World::get().mHumanState[i]);
	}
	mPanel->PaintNow();
}

void ZomFrame::OnReset(wxCommandEvent& event)
{
	World::get().mHumanState.clear();
	World::get().mZombieState.clear();
	mTurnTimer->Stop(); 
	World::get().mHumanMachine.numTurns = 0; 
	mPanel->PaintNow();
}
