//////////////////////////////////////////////////////////////////////
// GLDisplayContext.cpp: implementation of the CGLDisplayContext class.
//
//////////////////////////////////////////////////////////////////////

#include "GLDisplayContext.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLDisplayContext::CGLDisplayContext()
{
	display = new CListOfCGLObject;
	erase = new CListOfCGLObject;
	select = new CListOfCGLObject;
	selListIter.SetList(select);
	selListIter.Init();
	viewBox.SetLimits(-1, 1, -1, 1, -1, 1);
	mouseSensitivity = 5; //5 pixels sensitivity
}

CGLDisplayContext::~CGLDisplayContext()
{
	if(!display->IsEmpty())
		display->Clear();
	if(!erase->IsEmpty())
		erase->Clear();
	if(!select->IsEmpty())
		select->Clear();
	delete display;
	delete erase;
	delete select;
}

void CGLDisplayContext::Display(CGLObject* aO)
{
	if(aO == NULL)
		return;
	//CGLObject* O = aO->Copy();
	if(display->IsEmpty())
		viewBox = aO->GetBoundingBox();
	else
		viewBox.AddBox(aO->GetBoundingBox());
	display->Append(aO);
}

void CGLDisplayContext::RefreshVolume()
{
    viewBox.SetLimits(-1, 1, -1, 1, -1, 1);
    CListIteratorOfListOfCGLObject listIter(display);
    for(listIter.Init(); listIter.More(); listIter.Next())
    {
	CGLObject* pObj = (CGLObject*)listIter.Current();
	CBoundingBox b = pObj->GetBoundingBox();
	viewBox.AddBox(b);
    }
}

void CGLDisplayContext::Erase(CGLObject* aO)
{
	erase->Append(display->Remove(aO));
}

void CGLDisplayContext::DisplayAll()
{
	CListIteratorOfListOfCGLObject listIter(erase);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		CGLObject* eO = listIter.Current();
		if(display->IsEmpty())
			viewBox = eO->GetBoundingBox();
		else
			viewBox.AddBox(eO->GetBoundingBox());
		display->Append(eO);
		listIter.SetCurrent(0);
	}
	erase->Clear();
}

void CGLDisplayContext::DisplaySelected()
{
	if(!HasSelected())
		return;
	EraseAll();
	CListIteratorOfListOfCGLObject listIterD(erase);
	CListIteratorOfListOfCGLObject listIterSel(select);
	CGLObject* eO = 0, *sO = 0;
	for(listIterSel.Init(); listIterSel.More(); listIterSel.Next())
	{
		sO = listIterSel.Current();
		for(listIterD.Init(); listIterD.More(); listIterD.Next())
		{
			eO = listIterD.Current();
			if(eO->GetObjID() == sO->GetObjID())
				break;
		}
		if(display->IsEmpty())
			viewBox = eO->GetBoundingBox();
		else
			viewBox.AddBox(eO->GetBoundingBox());
		display->Append(erase->Remove(eO));
	}
	EmptySelection();
}

void CGLDisplayContext::EraseAll()
{
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		erase->Append(listIter.Current());
		listIter.SetCurrent(0);
	}
	display->Clear();
}

void CGLDisplayContext::EraseSelected()
{
	if(!HasSelected())
		return;
	CListIteratorOfListOfCGLObject listIterD(display);
	CListIteratorOfListOfCGLObject listIterSel(select);
	CGLObject* eO = 0, *sO = 0;
	for(listIterSel.Init(); listIterSel.More(); listIterSel.Next())
	{
		sO = listIterSel.Current();
		for(listIterD.Init(); listIterD.More(); listIterD.Next())
		{
			eO = listIterD.Current();
			if(eO->GetObjID() == sO->GetObjID())
				break;
		}
		Erase(eO);
	}
	EmptySelection();
	viewBox.SetLimits(-1,1,-1,1,-1,1);
	for(listIterD.Init(); listIterD.More(); listIterD.Next())
		viewBox.AddBox(listIterD.Current()->GetBoundingBox());
}

void CGLDisplayContext::DeleteAll()
{
	if(!display->IsEmpty())
		display->Clear();
	if(!erase->IsEmpty())
		erase->Clear();
	EmptySelection();
	viewBox.SetLimits(-1, 1, -1, 1, -1, 1);
}

void CGLDisplayContext::DeleteSelected()
{
	if(!HasSelected())
		return;
	CListIteratorOfListOfCGLObject listIterD(display);
	CListIteratorOfListOfCGLObject listIterSel(select);
	CGLObject* eO = 0, *sO = 0;
	for(listIterSel.Init(); listIterSel.More(); listIterSel.Next())
	{
		sO = listIterSel.Current();
		for(listIterD.Init(); listIterD.More(); listIterD.Next())
		{
			eO = listIterD.Current();
			if(eO->GetObjID() == sO->GetObjID())
				break;
		}
		delete display->Remove(eO);
	}
	EmptySelection();
	viewBox.SetLimits(-1,1,-1,1,-1,1);
	for(listIterD.Init(); listIterD.More(); listIterD.Next())
		viewBox.AddBox(listIterD.Current()->GetBoundingBox());
}

void CGLDisplayContext::DeletePoints()
{
	EmptySelection();
	CListIteratorOfListOfCGLObject listIterD(display);
	CGLObject* eO = 0;
	for(listIterD.Init(); listIterD.More(); listIterD.Next())
	{
		eO = listIterD.Current();
		if(eO->IsOfType(GLPOINT))
			break;
	}
	delete display->Remove(eO);
}

void CGLDisplayContext::Render(const GLDisplayMode& mode)
{
	if(IsEmpty())
		return;
	CGLObject* O;

	// Initialize the names stack
	//glInitNames();  commented out for application specific use
	//glPushName(0);  commented out for application specific use
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		O = listIter.Current();
		if(O && !O->IsHidden())
		{
			//glLoadName(O->GetObjID());       commented out for application specific use
			O->Display(O->GetDisplayMode());
		}
	}
	listIter.SetList(select);
	for(listIter.Init(); listIter.More(); listIter.Next())

	{
		O = listIter.Current();
		if(O && !O->IsHidden())
			O->Hilight(mode);
	}
}

bool CGLDisplayContext::Select(GtkGLView* aView, const int& x, const int& y)
{
	bool res = false;
	CGLObject* O = 0;
	unsigned long int id = aView->ProcessSelection(x, y, mouseSensitivity);
	if(id)
		res = true;
	else
	{
		EmptySelection();
		return false;
	}
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		O = listIter.Current();
		if(O)
		{
			if(id == O->GetObjID())
			{
				if(!IsSelected(O))
				{
					select->Clear();
					AddToSelection(O);
				}
				else
					RemoveSelection(O);
			}
		}
	}
	return res;
}

bool CGLDisplayContext::MultiSelect(GtkGLView* aView, const int& x, const int& y)
{
	bool res = false;
	CGLObject* O = 0;
	unsigned long int id = aView->ProcessSelection(x, y, mouseSensitivity);
	if(id)
		res = true;
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		O = listIter.Current();
		if(O)
		{
			if(id == O->GetObjID())
			{
				if(!IsSelected(O))
					AddToSelection(O);
				else
					RemoveSelection(O);
			}
		}
	}
	return res;
}

bool CGLDisplayContext::SweepSelect(GtkGLView* aView, const GdkRectangle& swRect)
{
	bool res = false;
	CGLObject* O = 0;
	unsigned long int *id = new unsigned long int[100];
	int no_of_objs = aView->ProcessSweepSelection(swRect, id);
	if(no_of_objs)
		res = true;
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		O = listIter.Current();
		if(O)
		{
			for(int i = 0; i < no_of_objs; i++)
			{
				if(id[i] == O->GetObjID())
					AddToSelection(O);
			}
		}
	}
	delete [] id;
	return res;
}

bool CGLDisplayContext::IsEmpty() const
{
	return (display->IsEmpty() && erase->IsEmpty());
}

bool CGLDisplayContext::IsDisplayed(const CGLObject* O) const
{
	if(O == NULL)
		return false;
	CListIteratorOfListOfCGLObject li(display);

	return li.IsFound(O);
}

bool CGLDisplayContext::IsErased(const CGLObject* O) const
{
	if(O == NULL)
		return false;
	CListIteratorOfListOfCGLObject li(erase);
	return li.IsFound(O);
}

bool CGLDisplayContext::IsSelected(const CGLObject* O) const
{
	bool b = false;
	if(O == NULL)
		return false;
	CListIteratorOfListOfCGLObject li(select);
	for(li.Init(); li.More(); li.Next())
	{
		if(O->GetObjID() == li.Current()->GetObjID())
		{
			b = true;
			break;
		}
	}
	return b;
}

bool CGLDisplayContext::IsAllDisplayed() const
{
	return erase->IsEmpty();
}

bool CGLDisplayContext::IsAllErased() const
{
	return display->IsEmpty();
}

bool CGLDisplayContext::HasSelected() const
{
	return !select->IsEmpty();
}

void CGLDisplayContext::InitSelected() const
{
	selListIter.Init();
}

bool CGLDisplayContext::MoreSelected() const
{
	return selListIter.More();
}

void CGLDisplayContext::NextSelected() const
{
	selListIter.Next();
}

CGLObject* CGLDisplayContext::CurrentSelected() const
{
	CGLObject* sO = selListIter.Current();
	CGLObject* O = 0;
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		if(sO->GetObjID() == listIter.Current()->GetObjID())
		{
			O = listIter.Current();
			break;
		}
	}
	return O;
}

//Private
void CGLDisplayContext::AddToSelection(const CGLObject* O)
{
	select->Append(O->Copy());
}

void CGLDisplayContext::RemoveSelection(const CGLObject* O)
{
	CListIteratorOfListOfCGLObject listIter(select);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		if(O->GetObjID() == listIter.Current()->GetObjID())
		{
			delete select->Remove(listIter.Current());
			break;
		}
	}
}

void CGLDisplayContext::EmptySelection()
{
	if(!select->IsEmpty())
		select->Clear();
}
