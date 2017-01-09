//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include "World.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	// (x, y, width, height)
	dc.DrawRectangle(10, 10, 600, 600);
	
	for (int i = 1; i < 20; i++) {
		dc.DrawLine(10, 10 + 30 * i, 600 + 10, 10 + 30 * i);
	}

	for (int i = 1; i < 20; i++) {
		dc.DrawLine(10 + 30 * i, 10, 10 + 30 * i, 600 + 10);
	}

	dc.SetPen(*wxRED_PEN);
	dc.SetTextForeground(*wxRED);
	dc.DrawText("Zombies", 625, 10);
	dc.DrawText("Program: " , 625, 25);
	dc.DrawText(World::get().mZombieMachine.loadedFile, 675, 25);
	dc.DrawText("Alive: ", 625, 40);
	dc.DrawText(std::to_string(World::get().mZombieState.size()), 670, 40);

	dc.SetPen(*wxGREEN_PEN);
	dc.SetTextForeground(*wxGREEN);
	dc.DrawText("Humans", 625, 100);
	dc.DrawText("Program: ", 625, 115);
	dc.DrawText(World::get().mHumanMachine.loadedFile, 675, 115);
	dc.DrawText("Alive: ", 625, 130);
	dc.DrawText(std::to_string(World::get().mHumanState.size()), 670, 130);

	dc.SetPen(*wxBLACK_PEN);
	dc.SetTextForeground(*wxBLACK);
	dc.DrawText("Month: ", 625, 200);
	dc.DrawText(std::to_string(World::get().mHumanMachine.numTurns), 675, 200);

	//drawing the zombies 
	dc.SetBrush(*wxRED_BRUSH);
	int zombieSize = World::get().mZombieState.size();
	const int offset = 10;
	const int squareDim = 30; 
	for (int i = 0; i < zombieSize; i++) 
	{
		int currentX = World::get().mZombieState[i]->mXPosition;
		int currentY = World::get().mZombieState[i]->mYPosition;
		wxPoint startingPoint, secondPoint, endingPoint; 
		if (World::get().mZombieState[i]->mFacing == MachineState::UP) {
			startingPoint.x = currentX * squareDim + offset; startingPoint.y = currentY * 30 + offset + squareDim;
			secondPoint.x = currentX * squareDim + offset + 15; secondPoint.y = currentY * squareDim + offset;
			endingPoint.x = currentX * squareDim + offset + squareDim; endingPoint.y = currentY * squareDim + offset + squareDim;
		}
		else if (World::get().mZombieState[i]->mFacing == MachineState::RIGHT) {
			startingPoint.x = currentX * squareDim + offset; startingPoint.y = currentY * squareDim + offset;
			secondPoint.x = currentX * squareDim + offset + squareDim; secondPoint.y = currentY * squareDim + offset + 15;
			endingPoint.x = currentX * squareDim + offset; endingPoint.y = currentY * squareDim + offset + squareDim;
		}
		else if (World::get().mZombieState[i]->mFacing == MachineState::DOWN) {
			startingPoint.x = currentX * squareDim + offset; startingPoint.y = currentY * squareDim + offset;
			secondPoint.x = currentX * squareDim + offset + 15; secondPoint.y = currentY * squareDim + offset + squareDim;
			endingPoint.x = currentX * squareDim + offset + squareDim; endingPoint.y = currentY * squareDim + offset;
		}
		//MachineState::LEFT
		else {	
			startingPoint.x = currentX * 30 + offset + 30; startingPoint.y = currentY * 30 + offset;
			secondPoint.x = currentX * 30 + offset; secondPoint.y = currentY * 30 + offset + 15;
			endingPoint.x = currentX * 30 + offset + 30; endingPoint.y = currentY * 30 + offset + 30;
		}
		
		wxPoint points[] = { startingPoint, secondPoint, endingPoint };
		dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
	}

	//drawing the humans
	dc.SetBrush(*wxGREEN_BRUSH);
	int humanSize = World::get().mHumanState.size();
	for (int i = 0; i < humanSize; i++) 
	{
		int currentX = World::get().mHumanState[i].mXPosition;
		int currentY = World::get().mHumanState[i].mYPosition;
		wxPoint startingPoint, secondPoint, endingPoint;
		if (World::get().mHumanState[i].mFacing == MachineState::UP) {
			startingPoint.x = currentX * squareDim + offset; startingPoint.y = currentY * 30 + offset + squareDim;
			secondPoint.x = currentX * squareDim + offset + 15; secondPoint.y = currentY * squareDim + offset;
			endingPoint.x = currentX * squareDim + offset + squareDim; endingPoint.y = currentY * squareDim + offset + squareDim;
		}
		else if (World::get().mHumanState[i].mFacing == MachineState::RIGHT) {
			startingPoint.x = currentX * squareDim + offset; startingPoint.y = currentY * squareDim + offset;
			secondPoint.x = currentX * squareDim + offset + squareDim; secondPoint.y = currentY * squareDim + offset + 15;
			endingPoint.x = currentX * squareDim + offset; endingPoint.y = currentY * squareDim + offset + squareDim;
		}
		else if (World::get().mHumanState[i].mFacing == MachineState::DOWN) {
			startingPoint.x = currentX * squareDim + offset; startingPoint.y = currentY * squareDim + offset;
			secondPoint.x = currentX * squareDim + offset + 15; secondPoint.y = currentY * squareDim + offset + squareDim;
			endingPoint.x = currentX * squareDim + offset + squareDim; endingPoint.y = currentY * squareDim + offset;
		}
		//MachineState::LEFT
		else {
			startingPoint.x = currentX * 30 + offset + 30; startingPoint.y = currentY * 30 + offset;
			secondPoint.x = currentX * 30 + offset; secondPoint.y = currentY * 30 + offset + 15;
			endingPoint.x = currentX * 30 + offset + 30; endingPoint.y = currentY * 30 + offset + 30;
		}

		wxPoint points[] = { startingPoint, secondPoint, endingPoint };
		dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
	}
}
