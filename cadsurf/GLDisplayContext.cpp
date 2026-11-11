// GLDisplayContext.cpp: implementation of the CGLDisplayContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Line3D.h"
#include <algorithm>
#include <memory>
#include "CadSurf.h"
#include "GLDisplayContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLDisplayContext::CGLDisplayContext()
{
}

CGLDisplayContext::CGLDisplayContext(CDocument* pDoc)
{
	display = new CListOfCGLObject;
	erase = new CListOfCGLObject;
	select = new CListOfCGLObject;
	selListIter.SetList(select);
	selListIter.Init();
	ptrDoc = pDoc;
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

void CGLDisplayContext::Append(CGLObject* aO)
{
	if(aO == NULL)
		return;

	if(display->IsEmpty())
		viewBox = aO->GetBoundingBox();
	else
		viewBox.AddBox(aO->GetBoundingBox());

	display->Append(aO);
}

void CGLDisplayContext::Remove(CGLObject* pGlObj)
{
	delete display->Remove(pGlObj);
	RecomputeBounds();
}

void CGLDisplayContext::Display(const CGLObject* aO)
{
	if(aO == NULL)
		return;
	CGLObject* O = aO->Copy();
	if(display->IsEmpty())
		viewBox = O->GetBoundingBox();
	else
		viewBox.AddBox(O->GetBoundingBox());
	display->Append(O);
	ptrDoc->UpdateAllViews(NULL);
}

void CGLDisplayContext::Erase(const CGLObject* aO)
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
	CGLObject* eO, *sO;
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
	CGLObject* eO, *sO;
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
	CGLObject* eO, *sO;
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

void CGLDisplayContext::Render(const GLDisplayMode& mode, bool picking)
{
	if(IsEmpty())
		return;
	CGLObject* O;

	// Initialize the names stack
	glInitNames();
	glPushName(0);
	//显示实体;
	CListIteratorOfListOfCGLObject listIter(display);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		O = listIter.Current();
		if(O)
		{
			glLoadName(O->GetObjID());
			O->Display(mode);
		}
	}
	//高亮显示选中实体
	if (picking)
		return;
	listIter.SetList(select);
	for(listIter.Init(); listIter.More(); listIter.Next())
	{
		O = listIter.Current();
		if(O)
			O->Hilight(mode);
	}
}

bool CGLDisplayContext::Select(CGLView* aView, const int& x, const int& y)
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
				if(O->IsOfType(GLSTLMESH) || !IsSelected(O))
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

bool CGLDisplayContext::MultiSelect(CGLView* aView, const int& x, const int& y)
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

bool CGLDisplayContext::SweepSelect(CGLView* aView, const CRect& swRect)
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
    return FindDisplayed(selListIter.Current()->GetObjID());
}

CGLObject* CGLDisplayContext::FindDisplayed(unsigned long id) const
{
    CListIteratorOfListOfCGLObject objects(display);
    for (objects.Init(); objects.More(); objects.Next())
    {
        CGLObject* object = objects.Current();
        if (object && object->GetObjID() == id)
            return object;
    }
    return nullptr;
}

CGLObject* CGLDisplayContext::DetachDisplayed(unsigned long id)
{
    CGLObject* object = FindDisplayed(id);
    if (!object)
        return nullptr;

    EmptySelection();
    CGLObject* detached = display->Remove(object);
    RecomputeBounds();
    return detached;
}

std::vector<RemovedObject> CGLDisplayContext::CollectForDeletion(bool all) const
{
    std::vector<RemovedObject> result;
    CListOfCGLObject* lists[] = {display, erase};
    for (int listIndex = 0; listIndex < (all ? 2 : 1); ++listIndex)
    {
        CListIteratorOfListOfCGLObject objects(lists[listIndex]);
        std::size_t index = 0;
        for (objects.Init(); objects.More(); objects.Next(), ++index)
        {
            CGLObject* object = objects.Current();
            if (object && (all || IsSelected(object)))
            {
                RemovedObject entry;
                entry.id = object->GetObjID();
                entry.identity = object;
                entry.index = index;
                entry.erased = listIndex == 1;
                result.push_back(std::move(entry));
            }
        }
    }
    return result;
}

bool CGLDisplayContext::DetachForHistory(std::vector<RemovedObject>& objects)
{
    struct LocatedObject
    {
        CGLObject* object;
        CListOfCGLObject* list;
        std::size_t index;
        bool erased;
    };
    std::vector<LocatedObject> located;
    located.reserve(objects.size());
    for (const RemovedObject& entry : objects)
    {
        if (entry.object)
            return false;

        bool found = false;
        CListOfCGLObject* lists[] = {display, erase};
        for (int listIndex = 0; listIndex < 2 && !found; ++listIndex)
        {
            CListIteratorOfListOfCGLObject iterator(lists[listIndex]);
            std::size_t index = 0;
            for (iterator.Init(); iterator.More(); iterator.Next(), ++index)
            {
                CGLObject* object = iterator.Current();
                if (object == entry.identity && object->GetObjID() == entry.id)
                {
                    located.push_back({object, lists[listIndex], index, listIndex == 1});
                    found = true;
                    break;
                }
            }
        }
        if (!found)
            return false;
    }

    EmptySelection();
    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        objects[i].index = located[i].index;
        objects[i].erased = located[i].erased;
        objects[i].object.reset(located[i].list->Remove(located[i].object));
    }
    RecomputeBounds();
    return true;
}

bool CGLDisplayContext::RestoreFromHistory(std::vector<RemovedObject>& objects)
{
    for (const RemovedObject& entry : objects)
        if (!entry.object || entry.object.get() != entry.identity)
            return false;

    std::sort(objects.begin(), objects.end(),
        [](const RemovedObject& left, const RemovedObject& right)
        {
            return left.erased == right.erased ? left.index < right.index
                                               : left.erased < right.erased;
        });
    try
    {
        for (RemovedObject& entry : objects)
        {
            CListOfCGLObject* list = entry.erased ? erase : display;
            list->InsertAt(entry.object.get(), entry.index);
            entry.object.release();
        }
    }
    catch (const std::bad_alloc&)
    {
        // Restore ownership if allocating a list node fails mid-operation.
        for (RemovedObject& entry : objects)
        {
            if (!entry.object)
            {
                CListOfCGLObject* list = entry.erased ? erase : display;
                entry.object.reset(list->Remove(entry.identity));
            }
        }
        RecomputeBounds();
        return false;
    }
    RecomputeBounds();
    return true;
}

std::vector<CGLObject*> CGLDisplayContext::DisplayedObjectsOfType(GLObjectType type) const
{
    std::vector<CGLObject*> result;
    CListIteratorOfListOfCGLObject objects(display);
    for (objects.Init(); objects.More(); objects.Next())
    {
        CGLObject* object = objects.Current();
        if (object && object->IsOfType(type))
            result.push_back(object);
    }
    return result;
}

namespace
{
bool GetLineEndpoints(const CGLObject* object, CPoint3D& start, CPoint3D& end)
{
    if (!object || !object->IsOfType(GLCURVE))
        return false;

    const std::unique_ptr<CGeometry> geometry(object->Geometry());
    const auto* line = dynamic_cast<const CLine3D*>(geometry.get());
    if (!line)
        return false;

    start = line->StartPoint();
    end = line->EndPoint();
    return true;
}
}

bool CGLDisplayContext::GetStraightLine(unsigned long id, CPoint3D& start, CPoint3D& end) const
{
    return GetLineEndpoints(FindDisplayed(id), start, end);
}

std::vector<CPoint3D> CGLDisplayContext::ReferencePoints() const
{
    std::vector<CPoint3D> points;
    CListIteratorOfListOfCGLObject objects(display);
    for (objects.Init(); objects.More(); objects.Next())
    {
        const CGLObject* object = objects.Current();
        if (!object || object->IsHidden())
            continue;

        if (object->IsOfType(GLPOINT))
        {
            const std::unique_ptr<CGeometry> geometry(object->Geometry());
            if (const auto* point = dynamic_cast<const CPoint3D*>(geometry.get()))
                points.push_back(*point);
        }
        else if (object->IsOfType(GLCURVE))
        {
            CPoint3D start, end;
            if (GetLineEndpoints(object, start, end))
            {
                points.push_back(start);
                points.push_back(end);
            }
        }
    }
    return points;
}

void CGLDisplayContext::RecomputeBounds()
{
    CListIteratorOfListOfCGLObject objects(display);
    objects.Init();
    if (!objects.More())
    {
        viewBox.SetLimits(-1, 1, -1, 1, -1, 1);
        return;
    }

    viewBox = objects.Current()->GetBoundingBox();
    for (objects.Next(); objects.More(); objects.Next())
        viewBox.AddBox(objects.Current()->GetBoundingBox());
}

void CGLDisplayContext::AddToSelection(const CGLObject* object)
{
    if (!IsSelected(object))
        select->Append(object->Copy());
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
