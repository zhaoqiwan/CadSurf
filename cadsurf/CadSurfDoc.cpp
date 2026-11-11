// CadSurfDoc.cpp : implementation of the CCadSurfDoc class
//

#include "stdafx.h"
#include "CadSurf.h"

#include "CadSurfDoc.h"
#include "GLStlMesh.h"

#include <algorithm>
#include <fstream>
#include <new>
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCadSurfDoc

IMPLEMENT_DYNCREATE(CCadSurfDoc, CDocument)

BEGIN_MESSAGE_MAP(CCadSurfDoc, CDocument)
	//{{AFX_MSG_MAP(CCadSurfDoc)
	ON_COMMAND(ID_FILE_IMPORT_STL, OnImportStl)
	ON_COMMAND(ID_POINTS, OnPoints)
	ON_COMMAND(ID_LINES, OnLines)
	ON_COMMAND(ID_CIRCLES, OnCircles)
	ON_COMMAND(ID_ELLIPSE, OnEllipse)
	ON_COMMAND(ID_PARABOLA, OnParabola)
	ON_COMMAND(ID_HYPERBOLA, OnHyperbola)
	ON_COMMAND(ID_BEZIER, OnBezier)
	ON_COMMAND(ID_BSPLINE, OnBspline)
	ON_COMMAND(ID_PLANE, OnPlane)
	ON_COMMAND(ID_CYLINDER, OnCylinder)
	ON_COMMAND(ID_CONE, OnCone)
	ON_COMMAND(ID_SPHERE, OnSphere)
	ON_COMMAND(ID_TORUS, OnTorus)
	ON_COMMAND(ID_EXTSURF, OnExtsurf)
	ON_COMMAND(ID_REVSURF, OnRevsurf)
	ON_COMMAND(ID_RULEDSURF, OnRuledsurf)
	ON_COMMAND(ID_PIPE, OnPipe)
	ON_COMMAND(ID_TRANSLATE, OnTranslate)
	ON_COMMAND(ID_ROTATE, OnRotate)
	ON_COMMAND(ID_SCALE, OnScale)
	ON_COMMAND(ID_REFLECT, OnReflect)
	ON_COMMAND(ID_EDIT_DELETEALL, OnEditDeleteall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETEALL, OnUpdateEditDeleteall)
	ON_COMMAND(ID_EDIT_DELETESELECTED, OnEditDeleteselected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETESELECTED, OnUpdateEditDeleteselected)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_HELIX, OnHelix)
	ON_COMMAND(ID_OFFSETSURFACE, OnOffsetsurface)
	ON_COMMAND(ID_OFFSETCURVE, OnOffsetcurve)
	ON_COMMAND(ID_USERSURFACE, OnUsersurface)
	ON_UPDATE_COMMAND_UI(ID_USERSURFACE, OnUpdateUsersurface)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCadSurfDoc construction/destruction

CCadSurfDoc::CCadSurfDoc()
{
	// TODO: add one-time construction code here
	dContext = new CGLDisplayContext(this);
}

CCadSurfDoc::~CCadSurfDoc()
{
	delete dContext;
}

BOOL CCadSurfDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	undoStack.clear();
	redoStack.clear();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCadSurfDoc serialization

void CCadSurfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		
	}
	else
	{
		// TODO: add loading code here		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCadSurfDoc diagnostics

#ifdef _DEBUG
void CCadSurfDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCadSurfDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCadSurfDoc commands

void CCadSurfDoc::OnImportStl()
{
    CFileDialog dialog(TRUE, _T("stl"), nullptr,
                       OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
                       _T("STL files (*.stl)|*.stl||"));
    if (dialog.DoModal() != IDOK)
        return;

    std::ifstream input(dialog.GetPathName().GetString(), std::ios::binary);
    if (!input)
    {
        AfxMessageBox(_T("The STL file could not be opened."), MB_ICONERROR);
        return;
    }

    try
    {
        CWaitCursor wait;
        StlMesh mesh;
        std::string error;
        if (!ReadBinaryStl(input, mesh, error))
        {
            AfxMessageBox(CString(error.c_str()), MB_ICONERROR);
            return;
        }

        const auto count = static_cast<unsigned long>(mesh.triangles.size());
        CGLStlMesh object(std::make_shared<StlMesh>(std::move(mesh)));

        // Put every imported part on Z=0 and append it to the right of the
        // displayed STL row. STL files carry geometry but no assembly position.
        const auto existing = dContext->DisplayedObjectsOfType(GLSTLMESH);
        const CBoundingBox initial = object.GetBoundingBox();
        const double width = initial.XMax() - initial.XMin();
        const double depth = initial.YMax() - initial.YMin();
        double maximumSpan = (std::max)(width, depth);
        double rightmost = 0.0;
        bool hasExisting = false;
        for (const CGLObject* current : existing)
        {
            const CBoundingBox bounds = current->GetBoundingBox();
            maximumSpan = (std::max)(maximumSpan,
                (std::max)(bounds.XMax() - bounds.XMin(),
                           bounds.YMax() - bounds.YMin()));
            if (!hasExisting || bounds.XMax() > rightmost)
                rightmost = bounds.XMax();
            hasExisting = true;
        }

        const double gap = (std::max)(1.0, maximumSpan * 0.1);
        const StlPosition offset = {{
            hasExisting ? rightmost + gap - initial.XMin()
                        : -(initial.XMin() + initial.XMax()) * 0.5,
            -(initial.YMin() + initial.YMax()) * 0.5,
            -initial.ZMin()
        }};
        StlTransform placement;
        CBoundingBox placedBounds;
        if (!object.PrepareTransform(StlTransform::Translation(offset),
                                     placement, placedBounds))
        {
            AfxMessageBox(_T("The STL placement is outside the supported coordinate range."),
                          MB_ICONWARNING);
            return;
        }
        object.SetTransform(placement, placedBounds);
        dContext->Display(&object);
        RecordStlImport(object.GetObjID());

        // Keep every document view wide enough to show the newly extended row.
        POSITION viewPosition = GetFirstViewPosition();
        while (viewPosition)
        {
            auto* cadView = DYNAMIC_DOWNCAST(CCadSurfView, GetNextView(viewPosition));
            if (cadView && cadView->GetView())
                cadView->GetView()->FitAll();
        }

        if (auto* frame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd()))
        {
            CString message;
            message.Format(_T("Imported %s (%lu triangles)"), dialog.GetFileName().GetString(), count);
            frame->SetMessageText(message);
        }
    }
    catch (const std::bad_alloc&)
    {
        AfxMessageBox(_T("There is not enough memory to import this STL file."), MB_ICONERROR);
    }
}

std::vector<StlObjectState> CCadSurfDoc::CaptureStlStates(
    const std::vector<unsigned long>& objectIds) const
{
    std::vector<StlObjectState> states;
    states.reserve(objectIds.size());
    for (unsigned long id : objectIds)
    {
        const auto* mesh = dynamic_cast<const CGLStlMesh*>(dContext->FindDisplayed(id));
        if (!mesh)
            continue;

        StlObjectState state;
        state.objectId = id;
        state.transform = mesh->GetTransform();
        state.bounds = mesh->GetBoundingBox();
        states.push_back(state);
    }
    return states;
}

void CCadSurfDoc::RecordStlTransform(LPCTSTR name,
    std::vector<StlObjectState> before, std::vector<StlObjectState> after)
{
    if (before.empty() || before.size() != after.size())
        return;

    StlHistoryCommand command;
    command.kind = StlHistoryKind::Transform;
    command.name = name;
    command.before = std::move(before);
    command.after = std::move(after);
    PushStlHistory(std::move(command));
}

void CCadSurfDoc::RecordStlImport(unsigned long objectId)
{
    StlHistoryCommand command;
    command.kind = StlHistoryKind::Import;
    command.name = _T("Import STL");
    command.importedObjectId = objectId;
    PushStlHistory(std::move(command));
}

void CCadSurfDoc::PushStlHistory(StlHistoryCommand command)
{
    redoStack.clear();
    undoStack.push_back(std::move(command));
    if (undoStack.size() > MaxStlHistory)
        undoStack.erase(undoStack.begin());
    SetModifiedFlag(TRUE);
}

bool CCadSurfDoc::RestoreStlStates(const std::vector<StlObjectState>& states)
{
    std::vector<CGLStlMesh*> meshes;
    meshes.reserve(states.size());
    for (const StlObjectState& state : states)
    {
        auto* mesh = dynamic_cast<CGLStlMesh*>(dContext->FindDisplayed(state.objectId));
        if (!mesh)
            return false;
        meshes.push_back(mesh);
    }

    for (size_t index = 0; index < states.size(); ++index)
        meshes[index]->SetTransform(states[index].transform, states[index].bounds);
    return true;
}

void CCadSurfDoc::RefreshAfterHistory()
{
    dContext->RecomputeBounds();
    POSITION viewPosition = GetFirstViewPosition();
    while (viewPosition)
    {
        auto* cadView = DYNAMIC_DOWNCAST(CCadSurfView, GetNextView(viewPosition));
        if (cadView && cadView->GetView())
            cadView->GetView()->FitAll();
    }
    UpdateAllViews(nullptr);
}

void CCadSurfDoc::SetHistoryStatus(LPCTSTR verb, LPCTSTR name)
{
    if (auto* frame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd()))
    {
        CString message;
        message.Format(_T("%s: %s"), verb, name);
        frame->SetMessageText(message);
    }
}

void CCadSurfDoc::OnEditUndo()
{
    if (undoStack.empty())
        return;

    StlHistoryCommand command = std::move(undoStack.back());
    undoStack.pop_back();
    bool restored = false;
    if (command.kind == StlHistoryKind::Transform)
    {
        restored = RestoreStlStates(command.before);
    }
    else
    {
        CGLObject* detached = dContext->DetachDisplayed(command.importedObjectId);
        if (detached)
        {
            command.detachedObject.reset(detached);
            restored = true;
        }
    }

    if (!restored)
    {
        undoStack.push_back(std::move(command));
        AfxMessageBox(_T("Undo could not find one of the STL parts used by this action."),
                      MB_ICONWARNING);
        return;
    }

    const CString actionName = command.name;
    redoStack.push_back(std::move(command));
    RefreshAfterHistory();
    SetHistoryStatus(_T("Undo"), actionName);
    SetModifiedFlag(TRUE);
}

void CCadSurfDoc::OnEditRedo()
{
    if (redoStack.empty())
        return;

    StlHistoryCommand command = std::move(redoStack.back());
    redoStack.pop_back();
    bool restored = false;
    if (command.kind == StlHistoryKind::Transform)
    {
        restored = RestoreStlStates(command.after);
    }
    else if (command.detachedObject)
    {
        dContext->Append(command.detachedObject.release());
        restored = true;
    }

    if (!restored)
    {
        redoStack.push_back(std::move(command));
        AfxMessageBox(_T("Redo could not restore one of the STL parts used by this action."),
                      MB_ICONWARNING);
        return;
    }

    const CString actionName = command.name;
    undoStack.push_back(std::move(command));
    RefreshAfterHistory();
    SetHistoryStatus(_T("Redo"), actionName);
    SetModifiedFlag(TRUE);
}

void CCadSurfDoc::OnUpdateEditUndo(CCmdUI* command)
{
    command->Enable(!undoStack.empty());
    CString label = _T("&Undo\tCtrl+Z");
    if (!undoStack.empty())
        label.Format(_T("&Undo %s\tCtrl+Z"), static_cast<LPCTSTR>(undoStack.back().name));
    command->SetText(label);
}

void CCadSurfDoc::OnUpdateEditRedo(CCmdUI* command)
{
    command->Enable(!redoStack.empty());
    CString label = _T("&Redo\tCtrl+Y");
    if (!redoStack.empty())
        label.Format(_T("&Redo %s\tCtrl+Y"), static_cast<LPCTSTR>(redoStack.back().name));
    command->SetText(label);
}

void CCadSurfDoc::OnPoints() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P1(100,100,100);
	CPoint3D P3(-200,200,100);
	CPoint3D P2(200,-200,-100);

	CGLPoint *gP1 = new CGLPoint(P1);
	CGLPoint *gP2 = new CGLPoint(P2);
	CGLPoint *gP3 = new CGLPoint(P3);

	dContext->Display(gP1);
	dContext->Display(gP2);
	dContext->Display(gP3);

	CString str;

	str.Format("P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, P1+1);
	str.Format("P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P2+1);
	str.Format("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, P3+1);

	dContext->Display(myFont1);
	dContext->Display(myFont2);
	dContext->Display(myFont3);

	delete gP1;
	delete gP2;
	delete gP3;

	delete myFont1;
	delete myFont2;
	delete myFont3;
}

void CCadSurfDoc::OnLines() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P1(-100, 100, -100);
	CPoint3D P2(100, 300, 100);
	CLine3D L1(P1, P2);
	CVector3D V(1,0,1);

	CPoint3D P3(100, -200, 100);
	CLine3D L2(P3, V);

	CGLCurve* lin1 = new CGLCurve(&L1);
	CGLCurve* lin2 = new CGLCurve(&L2);

	dContext->Display(lin1);
	dContext->Display(lin2);	

	CGLPoint* gP1 = new CGLPoint(P1);
	CGLPoint* gP2 = new CGLPoint(P2);
	CGLPoint* gP3 = new CGLPoint(P3);

	dContext->Display(gP1);
	dContext->Display(gP2);
	dContext->Display(gP3);

	CString str;

	str.Format("From P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, P1);
	str.Format("To P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P2);
	P1.Translate(-100,-100,-100);
	CVector3D D = L2.Direction();
	str.Format("Infinite Line Thru P3 Along Direction(%0.3lf,%0.3lf,%0.3lf)",D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, L2.PointAtPara(0.499));
	str.Format("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
	CGLFont *myFont4 = new CGLFont((LPCTSTR)str, P3);

	dContext->Display(myFont1);
	dContext->Display(myFont2);
	dContext->Display(myFont3);
	dContext->Display(myFont4);

	delete lin1;
	delete lin2;

	delete gP1;
	delete gP2;
	delete gP3;

	delete myFont1;
	delete myFont2;
	delete myFont3;
	delete myFont4;
}

void CCadSurfDoc::OnCircles() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double Rad = 100;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CCircle3D C(P, Rad);
	CGLCurve* gC = new CGLCurve(&C);
	dContext->Display(gC);

	CString str;
	str.Format("Circle Of Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetRadius(),P.GetX(), P.GetY(), P.GetZ());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CPoint3D P1(-200, 100, 0);
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);
	CAxisSystem Ax(P1, CVector3D(1,0,1));
	CCircle3D C1(Ax, Rad);
	CGLCurve* gC1 = new CGLCurve(&C1);
	dContext->Display(gC1);

	str.Format("Radius %0.3lf ",C1.GetRadius());
	CPoint3D R = P1;
	R.Translate(R, C1.PointAtPara(0));
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, R);
	dContext->Display(myFont1);
	CVector3D D = Ax.GetDirection();
	str.Format("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont2);

	CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
	dContext->Display(gAx);

	delete gP;
	delete gC;
	delete gP1;
	delete gC1;
	delete gAx;

	delete myFont;
	delete myFont1;
	delete myFont2;
}

void CCadSurfDoc::OnEllipse() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double maj = 100, min = 50;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CEllipse3D C(P, maj, min);
	CGLCurve* gC = new CGLCurve(&C);
	dContext->Display(gC);

	CString str;
	str.Format("Ellipse Of Maj Radius %0.3lf And Min Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetMajorRadius(), C.GetMinorRadius(),P.GetX(), P.GetY(), P.GetZ());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CPoint3D P1(-200, 100, 0);
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);

	CAxisSystem Ax(P1, CVector3D(1,1,0));
	CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
	dContext->Display(gAx);

	CEllipse3D C1(Ax, maj, min);
	CGLCurve* gC1 = new CGLCurve(&C1);
	dContext->Display(gC1);

	str.Format("Maj Radius %0.3lf",C1.GetMajorRadius());
	CPoint3D R1 = P1;
	R1.Translate(R1, C1.PointAtPara(0));
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, R1);
	dContext->Display(myFont1);
	str.Format("Min Radius %0.3lf ",C1.GetMinorRadius());
	CPoint3D R2 = P1;
	R2.Translate(R2, C1.PointAtPara(PI/2));
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, R2);
	dContext->Display(myFont2);
	CVector3D D = Ax.GetDirection();
	str.Format("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont3);

	delete gP;
	delete gC;
	delete gP1;
	delete gC1;
	delete gAx;

	delete myFont;
	delete myFont1;
	delete myFont2;
	delete myFont3;
}

void CCadSurfDoc::OnParabola() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem Ax(CPoint3D(200,0,0), CVector3D(0,0,1), CVector3D(1,0,0));
	CParabola3D P(Ax, 50);
	P.SetYRange(-10, 10);

	CAxisSystem Ax1(CPoint3D(-100,0,0), CVector3D(0,0,1), CVector3D(-1,0,0));
	CParabola3D P1(Ax1, 60);
	P1.SetYRange(-10, 10);

	CGLCurve* gC = new CGLCurve(&P);
	dContext->Display(gC);

	CGLCurve* gC1 = new CGLCurve(&P1);
	dContext->Display(gC1);

	CString str;
	str.Format("Parabola Of Focal Length %0.3lf",P.GetFocalLength());
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, CPoint3D(200,0,0));
	dContext->Display(myFont1);

	str.Format("Parabola Of Focal Length %0.3lf",P1.GetFocalLength());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P1.GetLocation().GetPosition());
	dContext->Display(myFont2);

	delete gC;
	delete gC1;

	delete myFont1;
	delete myFont2;
}

void CCadSurfDoc::OnHyperbola() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem Ax(CPoint3D(0,0,0), CVector3D(0,0,1), CVector3D(1,0,0));
	CHyperbola3D H(Ax, 100, 50);

	CHyperbola3D H1 = H.GetConjugateBranch1();
	CHyperbola3D H2 = H.GetConjugateBranch2();
	CHyperbola3D H3 = H.GetOtherBranch();

	CGLCurve* gC = new CGLCurve(&H);
	dContext->Display(gC);

	CGLCurve* gC1 = new CGLCurve(&H1);
	dContext->Display(gC1);

	CGLCurve* gC2 = new CGLCurve(&H2);
	dContext->Display(gC2);

	CGLCurve* gC3 = new CGLCurve(&H3);
	dContext->Display(gC3);

	CString str;
	CPoint3D P = H1.GetLocation().GetPosition();
	P.Translate(0,-20,0);
	str.Format("Hyperbola Of Maj Radius %0.3lf And Min Radius %0.3lf",H.GetMajorRadius(), H.GetMinorRadius());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	str.Format("First Conjugate Branch");
	CPoint3D P1 = H1.GetLocation().GetPosition();
	P1.Translate(0,200,0);
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont1);

	str.Format("Second Conjugate Branch");
	CPoint3D P2 = H2.GetLocation().GetPosition();
	P2.Translate(0,-200,0);
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P2);
	dContext->Display(myFont2);

	str.Format("Other Branch");
	CPoint3D P3 = H3.GetLocation().GetPosition();
	P3.Translate(-300,0,0);
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, P3);
	dContext->Display(myFont3);

	delete gC;
	delete gC1;
	delete gC2;
	delete gC3;

	delete myFont;
	delete myFont1;
	delete myFont2;
	delete myFont3;
}

void CCadSurfDoc::OnBezier() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CListOfCPoint3D *myCPList = new CListOfCPoint3D;
	CPoint3D P1(-450,-200,-00);
	CPoint3D P2(-350,300,-00);
	CPoint3D P3(-200,-300,-00);
	CPoint3D P4(-200,400,-00);
	CPoint3D P5(200,-300,-00);
	CPoint3D P6(300,400,-00);
	CPoint3D P7(350,-200,-00);
	CPoint3D P8(400,300,-00);
	myCPList->Append(P1);
	myCPList->Append(P2);
	myCPList->Append(P3);
	myCPList->Append(P4);
	myCPList->Append(P5);
	myCPList->Append(P6);
	myCPList->Append(P7);
	myCPList->Append(P8);

	CGLPoint *gP1 = new CGLPoint(P1);
	CGLPoint *gP2 = new CGLPoint(P2);
	CGLPoint *gP3 = new CGLPoint(P3);
	CGLPoint *gP4 = new CGLPoint(P4);
	CGLPoint *gP5 = new CGLPoint(P5);
	CGLPoint *gP6 = new CGLPoint(P6);
	CGLPoint *gP7 = new CGLPoint(P7);
	CGLPoint *gP8 = new CGLPoint(P8);

	dContext->Display(gP1);
	dContext->Display(gP2);
	dContext->Display(gP3);
	dContext->Display(gP4);
	dContext->Display(gP5);
	dContext->Display(gP6);
	dContext->Display(gP7);
	dContext->Display(gP8);

	CBezierCurve B(myCPList);
	CGLCurve* gC = new CGLCurve(&B);
	dContext->Display(gC);

	CString str;
	str.Format("P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, P1);
	str.Format("P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P2);
	str.Format("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, P3);
	str.Format("P4(%0.3lf,%0.3lf,%0.3lf)",P4.GetX(), P4.GetY(), P4.GetZ());
	CGLFont *myFont4 = new CGLFont((LPCTSTR)str, P4);
	str.Format("P5(%0.3lf,%0.3lf,%0.3lf)",P5.GetX(), P5.GetY(), P5.GetZ());
	CGLFont *myFont5 = new CGLFont((LPCTSTR)str, P5);
	str.Format("P6(%0.3lf,%0.3lf,%0.3lf)",P6.GetX(), P6.GetY(), P6.GetZ());
	CGLFont *myFont6 = new CGLFont((LPCTSTR)str, P6);
	str.Format("P7(%0.3lf,%0.3lf,%0.3lf)",P7.GetX(), P7.GetY(), P7.GetZ());
	CGLFont *myFont7 = new CGLFont((LPCTSTR)str, P7);
	str.Format("P8(%0.3lf,%0.3lf,%0.3lf)",P8.GetX(), P8.GetY(), P8.GetZ());
	CGLFont *myFont8 = new CGLFont((LPCTSTR)str, P8);

	dContext->Display(myFont1);
	dContext->Display(myFont2);
	dContext->Display(myFont3);
	dContext->Display(myFont4);
	dContext->Display(myFont5);
	dContext->Display(myFont6);
	dContext->Display(myFont7);
	dContext->Display(myFont8);

	myCPList->Clear();
	delete myCPList;

	delete gP1;	delete gP2;	delete gP3;	delete gP4;
	delete gP5;	delete gP6;	delete gP7;	delete gP8;

	delete gC;

	delete myFont1;	delete myFont2;	delete myFont3; delete myFont4;
	delete myFont5;	delete myFont6;	delete myFont7; delete myFont8;

}

void CCadSurfDoc::OnBspline() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CListOfCPoint3D *myCPList = new CListOfCPoint3D;
	CPoint3D P1(-450,-200,-00);
	CPoint3D P2(-350,300,-00);
	CPoint3D P3(-200,-300,-00);
	CPoint3D P4(-200,400,-00);
	CPoint3D P5(200,-300,-00);
	CPoint3D P6(300,400,-00);
	CPoint3D P7(350,-200,-00);
	CPoint3D P8(400,300,-00);
	myCPList->Append(P1);
	myCPList->Append(P2);
	myCPList->Append(P3);
	myCPList->Append(P4);
	myCPList->Append(P5);
	myCPList->Append(P6);
	myCPList->Append(P7);
	myCPList->Append(P8);

	CGLPoint *gP1 = new CGLPoint(P1);
	CGLPoint *gP2 = new CGLPoint(P2);
	CGLPoint *gP3 = new CGLPoint(P3);
	CGLPoint *gP4 = new CGLPoint(P4);
	CGLPoint *gP5 = new CGLPoint(P5);
	CGLPoint *gP6 = new CGLPoint(P6);
	CGLPoint *gP7 = new CGLPoint(P7);
	CGLPoint *gP8 = new CGLPoint(P8);

	dContext->Display(gP1);
	dContext->Display(gP2);
	dContext->Display(gP3);
	dContext->Display(gP4);
	dContext->Display(gP5);
	dContext->Display(gP6);
	dContext->Display(gP7);
	dContext->Display(gP8);

	CBSplineCurve B(myCPList);
	CGLCurve* gC = new CGLCurve(&B);
	dContext->Display(gC);

	CString str;
	str.Format("P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, P1);
	str.Format("P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P2);
	str.Format("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, P3);
	str.Format("P4(%0.3lf,%0.3lf,%0.3lf)",P4.GetX(), P4.GetY(), P4.GetZ());
	CGLFont *myFont4 = new CGLFont((LPCTSTR)str, P4);
	str.Format("P5(%0.3lf,%0.3lf,%0.3lf)",P5.GetX(), P5.GetY(), P5.GetZ());
	CGLFont *myFont5 = new CGLFont((LPCTSTR)str, P5);
	str.Format("P6(%0.3lf,%0.3lf,%0.3lf)",P6.GetX(), P6.GetY(), P6.GetZ());
	CGLFont *myFont6 = new CGLFont((LPCTSTR)str, P6);
	str.Format("P7(%0.3lf,%0.3lf,%0.3lf)",P7.GetX(), P7.GetY(), P7.GetZ());
	CGLFont *myFont7 = new CGLFont((LPCTSTR)str, P7);
	str.Format("P8(%0.3lf,%0.3lf,%0.3lf)",P8.GetX(), P8.GetY(), P8.GetZ());
	CGLFont *myFont8 = new CGLFont((LPCTSTR)str, P8);

	dContext->Display(myFont1);
	dContext->Display(myFont2);
	dContext->Display(myFont3);
	dContext->Display(myFont4);
	dContext->Display(myFont5);
	dContext->Display(myFont6);
	dContext->Display(myFont7);
	dContext->Display(myFont8);

	myCPList->Clear();
	delete myCPList;

	delete gP1;	delete gP2;	delete gP3;	delete gP4;
	delete gP5;	delete gP6;	delete gP7;	delete gP8;

	delete gC;

	delete myFont1;	delete myFont2;	delete myFont3; delete myFont4;
	delete myFont5;	delete myFont6;	delete myFont7; delete myFont8;
}

void CCadSurfDoc::OnHelix() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem Ax(CPoint3D(0,0,0), CVector3D(0,0,1), CVector3D(1,0,0));
	CHelix H(Ax, 100, 0, 20*PI, 20);

	
	CGLCurve* gC = new CGLCurve(&H);
	gC->SetSmooth(5000);
	dContext->Display(gC);

	CString str;
	CPoint3D P = H.GetLocation().GetPosition();
	str.Format("Helix Of Radius %0.3lf, Start Angle %0.3lf, End Angle %0.3lf And Lead %0.3lf",H.GetRadius(), H.FirstParameter()*180.0/PI, H.LastParameter()*180.0/PI, H.Lead());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	delete gC;
	delete myFont;
}

void CCadSurfDoc::OnOffsetcurve() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem Ax(CPoint3D(200,0,0), CVector3D(0,0,1), CVector3D(1,0,0));
	CParabola3D P(Ax, 50);
	P.SetYRange(-10, 10);

	CGLCurve* gC = new CGLCurve(&P);
	dContext->Display(gC);

	CString str;
	str.Format("Parabola Of Focal Length %0.3lf",P.GetFocalLength());
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, CPoint3D(200,0,0));
	dContext->Display(myFont1);

	COffsetCurve OC(P, 150, CPlane::XOY());
	CGLCurve* gC1 = new CGLCurve(&OC);
	gC1->SetColor(0,255,0);
	dContext->Display(gC1);

	str.Format("Offset Curve At Distance 150, Ref Plane XOY, Along Curve Normal");
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, OC.PointAtPara(0.5));
	dContext->Display(myFont2);

	COffsetCurve OC1(P, 150, CPlane::XOY(), false);
	CGLCurve* gC2 = new CGLCurve(&OC1);
	gC2->SetColor(255,0,0);
	dContext->Display(gC2);

	str.Format("Offset Curve At Distance 150, Ref Plane XOY,  Away From Curve Normal");
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, OC1.PointAtPara(0.5));
	dContext->Display(myFont3);


	delete gC;
	delete gC1;
	delete gC2;

	delete myFont1;
	delete myFont2;
	delete myFont3;
}

void CCadSurfDoc::OnPlane() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPlane Pln1(1,0,1,-200);
	CGLPlane* gPln1 = new CGLPlane(Pln1);
	gPln1->SetLight(BRONZE);
	dContext->Display(gPln1);

	CPlane Pln2 = CPlane::ZOX();
	CGLPlane* gPln2 = new CGLPlane(Pln2);
	dContext->Display(gPln2);

	CPoint3D P1(-300, -100, 50), P2(20, 200, -100), P3(200, 200, 100);
	CGLPoint *gP1 = new CGLPoint(P1);
	CGLPoint *gP2 = new CGLPoint(P2);
	CGLPoint *gP3 = new CGLPoint(P3);
	dContext->Display(gP1);
	dContext->Display(gP2);
	dContext->Display(gP3);

	CPlane Pln3(P1, P2, P3);
	CGLPlane* gPln3 = new CGLPlane(Pln3, 1200, 800);
	gPln3->SetLight(PEWTER);
	dContext->Display(gPln3);

	CPoint3D P5(-1000, 0, -500); CVector3D X(1,0,0), Y(0,1,1);
	CPlane Pln4(P5, X, Y);
	CGLPlane* gPln4 = new CGLPlane(Pln4);
	gPln4->SetLight(GOLD);
	dContext->Display(gPln4);

	delete gPln1; delete gPln2; delete gPln3; delete gPln4;
	delete gP1; delete gP2; delete gP3;
}

void CCadSurfDoc::OnCylinder() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double Rad = 80, Ht = 200;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CCylinder C(CAxisSystem(CPoint3D(200,0,0), CVector3D(0,0,1)), Ht, Rad, 0, PI);
	CGLSurface* gS = new CGLSurface(&C);
	dContext->Display(gS);

	CString str;
	str.Format("Cylinder Of Radius %0.3lf And Height %0.3lf At P(%0.3lf,%0.3lf,%0.3lf) U(0,PI)",C.GetRadius(), C.GetHeight(), P.GetX(), P.GetY(), P.GetZ());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CPoint3D P1(-300, 100, 0);
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);
	CAxisSystem Ax(P1, CVector3D(1,0,1));
	CCircle3D C1(Ax, Rad);
	CCylinder Cyl(C1, Ht);
	CGLCurve* gC1 = new CGLCurve(&C1);
	CGLSurface* gS1 = new CGLSurface(&Cyl);
	dContext->Display(gC1);
	dContext->Display(gS1);

	str.Format("Base Circle Of Radius %0.3lf ",C1.GetRadius());
	CPoint3D R = P1;
	R.Translate(R, C1.PointAtPara(0));
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, R);
	dContext->Display(myFont1);
	CVector3D D = Ax.GetDirection();
	str.Format("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont2);

	CPoint3D HP = P1;
	COneAxis ax(HP, D);
	HP.Translate(ax, Ht);
	str.Format("Height %0.3lf", Cyl.GetHeight());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, HP);
	dContext->Display(myFont3);

	CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
	dContext->Display(gAx);

	delete myFont; delete myFont1;	delete myFont2;	delete myFont3;
	delete gP; delete gS; delete gP1; delete gC1; delete gS1; delete gAx;
}

void CCadSurfDoc::OnCone() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double Rad = 100, Ht = 200;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CCone C(CAxisSystem(CPoint3D(200,0,0), CVector3D(0,0,1)), Ht, Rad);
	CGLSurface* gS = new CGLSurface(&C);
	dContext->Display(gS);

	CString str;
	str.Format("Cone Of Radius %0.3lf And Height %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetBaseRadius(), C.GetHeight(), P.GetX(), P.GetY(), P.GetZ());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CPoint3D P1(-200, -300, 0);
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);
	CAxisSystem Ax(P1, CVector3D(0,-1,1));
	CCircle3D C1(Ax, Rad);
	CCone Con(C1, Ht);
	CGLCurve* gC1 = new CGLCurve(&C1);
	CGLSurface* gS1 = new CGLSurface(&Con);
	dContext->Display(gC1);
	dContext->Display(gS1);

	str.Format("Base Circle Of Radius %0.3lf ",C1.GetRadius());
	CPoint3D R = P1;
	R.Translate(R, C1.PointAtPara(0));
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, R);
	dContext->Display(myFont1);
	CVector3D D = Ax.GetDirection();
	str.Format("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont2);

	CCone C2(CAxisSystem(CPoint3D(100,300,0), CVector3D(0,0,1)), Ht, Rad, Rad-50, 0, PI*1.5);
	CGLSurface* gS2 = new CGLSurface(&C2);
	dContext->Display(gS2);

	CPoint3D P2 = C2.GetLocation().GetPosition();
	str.Format("Cone Of Base Radius %0.3lf, Top Radius %0.3lf And Height %0.3lf At P(%0.3lf,%0.3lf,%0.3lf) U(0, 1.5*PI)",C2.GetBaseRadius(), C2.GetTopRadius(), C2.GetHeight(), P2.GetX(), P2.GetY(), P2.GetZ());
	CGLFont *myFont4 = new CGLFont((LPCTSTR)str, P2);
	dContext->Display(myFont4);

	CPoint3D HP = P1;
	COneAxis ax(HP, D);
	HP.Translate(ax, Ht);
	str.Format("Height %0.3lf", Con.GetHeight());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, HP);
	dContext->Display(myFont3);

	CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
	dContext->Display(gAx);

	delete myFont; delete myFont1;	delete myFont2;	delete myFont3; delete myFont4;
	delete gP; delete gS; delete gP1; delete gC1; delete gS1; delete gS2; delete gAx;
}

void CCadSurfDoc::OnSphere() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double Rad = 100;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CAxisSystem Ax1(P, CVector3D(0,0,1));
	CSphere S(Ax1, Rad, 0, PI, -PI/3, PI/3.5);
	CGLSurface* gS = new CGLSurface(&S);
	dContext->Display(gS);

	CString str;
	str.Format("Sphere Of Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf) U(0,PI) V(-PI/3, PI/3.5)",S.GetRadius(),P.GetX(), P.GetY(), P.GetZ());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CPoint3D P1(-200, 100, 0);
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);
	CAxisSystem Ax(P1, CVector3D(1,0,1));
	CSphere S1(Ax, Rad);
	CGLSurface* gS1 = new CGLSurface(&S1);
	dContext->Display(gS1);

	str.Format("Radius %0.3lf ",S1.GetRadius());
	CPoint3D R = P1;
	R.Translate(R, S1.PointAtPara(0, 0));
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, R);
	dContext->Display(myFont1);
	CVector3D D = Ax.GetDirection();
	str.Format("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont2);

	CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
	dContext->Display(gAx);

	delete myFont; delete myFont1;	delete myFont2;
	delete gP; delete gS; delete gP1; delete gS1; delete gAx;

}

void CCadSurfDoc::OnTorus() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double maj = 100, min = 50;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CAxisSystem Ax1(P, CVector3D(0,0,1));
	CTorus S(Ax1, maj, min, 0, PI*1.5, PI/2);
	CGLSurface* gS = new CGLSurface(&S);
	dContext->Display(gS);

	CString str;
	str.Format("Torus Of Maj Radius %0.3lf, Min Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf) U(0,PI*1.5) V(PI/2, PI*2)",S.GetMajorRadius(),S.GetMinorRadius(),P.GetX(), P.GetY(), P.GetZ());
	CGLFont *myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CPoint3D P1(-200, 100, 0);
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);
	CAxisSystem Ax(P1, CVector3D(1,0,1));
	CTorus S1(Ax, maj, min);
	CGLSurface* gS1 = new CGLSurface(&S1);
	dContext->Display(gS1);

	str.Format("Maj Radius %0.3lf ",S1.GetMajorRadius());
	CPoint3D R = P1;
	R.Translate(R, S1.PointAtPara(PI/2, 0));
	CGLFont *myFont1 = new CGLFont((LPCTSTR)str, R);
	dContext->Display(myFont1);

	str.Format("Min Radius %0.3lf ",S1.GetMinorRadius());
	CPoint3D R1 = P1;
	R1.Translate(R1, S1.PointAtPara(0, 0));
	CGLFont *myFont2 = new CGLFont((LPCTSTR)str, R1);
	dContext->Display(myFont2);

	CVector3D D = Ax.GetDirection();
	str.Format("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
	CGLFont *myFont3 = new CGLFont((LPCTSTR)str, P1);
	dContext->Display(myFont3);

	CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
	dContext->Display(gAx);

	delete myFont; delete myFont1;	delete myFont2;	delete myFont3;
	delete gP; delete gS; delete gP1; delete gS1; delete gAx;
}

void CCadSurfDoc::OnExtsurf() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem sAx(CPoint3D(-500, -800, 500), CVector3D(1, 0.5, 2));
	CParabola3D Par(sAx, 50);
	Par.SetYRange(-15, 15);
	CGLCurve* C1 = new CGLCurve(&Par);
	dContext->Display(C1);

	CString str;
	str.Format("Base Curve (Parabola)");
	CGLFont* myFont1 = new CGLFont((LPCTSTR)str, Par.PointAtPara(0));
	dContext->Display(myFont1);

	CAxisSystem sAx1(CPoint3D(500, -800, -100), CVector3D(-1, 5, -2));
	CEllipse3D El(sAx1, 200, 100);
	CGLCurve* C2 = new CGLCurve(&El);
	dContext->Display(C2);

	str.Format("Base Curve (Ellipse)");
	CGLFont* myFont2 = new CGLFont((LPCTSTR)str, El.PointAtPara((El.LastParameter()-El.FirstParameter())/2));
	dContext->Display(myFont2);
	
	CVector3D Dir(0,0,1);
	CExtrudedSurface E(&Par, Dir, -200, 100);
	CGLSurface* S = new CGLSurface(&E);
	dContext->Display(S);

	COneAxis Ax(Par.PointAtPara(0), Dir);
	CGLOneAxis* gAx = new CGLOneAxis(Ax, 200, 20);
	dContext->Display(gAx);

	CVector3D Dir1(-1,2,1);
	CExtrudedSurface E1(&El, Dir1, -200, 100);
	CGLSurface* S1 = new CGLSurface(&E1);
	dContext->Display(S1);

	COneAxis Ax1(El.PointAtPara(PI), Dir1);
	CGLOneAxis* gAx1 = new CGLOneAxis(Ax1, 200, 20);
	dContext->Display(gAx1);

	delete C1; delete C2; delete S; delete S1; delete gAx; delete gAx1;
	delete myFont1; delete myFont2;

}

void CCadSurfDoc::OnRevsurf() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CListOfCPoint3D *myCPList = new CListOfCPoint3D;
	CPoint3D P1(442, 642, 0);
	CPoint3D P2(208, 598, 0);
	CPoint3D P3(132, 552, 0);
	CPoint3D P4(146, 458, 0);
	CPoint3D P5(462, 320, 0);
	CPoint3D P6(686, 48, 0);
	CPoint3D P7(486, -186, 0);
	CPoint3D P8(152, -176, 0);
	CPoint3D P9(0, -176, 0);
	myCPList->Append(P1);
	myCPList->Append(P2);
	myCPList->Append(P3);
	myCPList->Append(P4);
	myCPList->Append(P5);
	myCPList->Append(P6);
	myCPList->Append(P7);
	myCPList->Append(P8);
	myCPList->Append(P9);

	CBezierCurve B(myCPList);
	B.Scale(P9, 1.5);
	CGLCurve* C1 = new CGLCurve(&B);
	dContext->Display(C1);

	CPoint3D P = B.PointAtPara(0.5);
	CString str;
	str.Format("Base Curve (Bezier)");
	CGLFont* myFont = new CGLFont((LPCTSTR)str, P);
	dContext->Display(myFont);

	CVector3D Dir(0,1,0);
	COneAxis Ax(P9, Dir);
	CRevolvedSurface S1(&B, Ax, 0, 2*PI);
	CGLSurface* gS = new CGLSurface(&S1);
	dContext->Display(gS);

	CGLOneAxis* gAx = new CGLOneAxis(Ax, 500, 50);
	dContext->Display(gAx);

	str.Format("Axis Of Revolution");
	CPoint3D Pa = Ax.GetPosition();
	Pa.Translate(Ax, 500);
	CGLFont* myFont1 = new CGLFont((LPCTSTR)str, Pa);
	dContext->Display(myFont1);

	myCPList->Clear();
	delete myCPList;

	CAxisSystem sAx(CPoint3D(-1500, -800, 500), CVector3D(1, 0.5, 2));
	CParabola3D Par(sAx, 50);
	Par.SetYRange(-15, 15);
	CGLCurve* C2 = new CGLCurve(&Par);
	dContext->Display(C2);

	str.Format("Base Curve (Parabola)");
	CGLFont* myFont2 = new CGLFont((LPCTSTR)str, Par.PointAtPara(10));
	dContext->Display(myFont2);

	COneAxis Ax2(sAx.GetPosition()-50, sAx.GetYDirection());
	CGLOneAxis* gAx1 = new CGLOneAxis(Ax2, 500, 50);
	dContext->Display(gAx1);

	str.Format("Axis Of Revolution");
	Pa = Ax2.GetPosition();
	Pa.Translate(Ax2, 500);
	CGLFont* myFont3 = new CGLFont((LPCTSTR)str, Pa);
	dContext->Display(myFont3);

	CRevolvedSurface S2(&Par, Ax2, 0, PI);
	CGLSurface* gS2 = new CGLSurface(&S2);
	gS2->SetLight(GOLD);
	dContext->Display(gS2);

	delete C1; delete C2; delete gS; delete gS2; delete gAx; delete gAx1;
	delete myFont; delete myFont1; delete myFont2; delete myFont3;
}

void CCadSurfDoc::OnRuledsurf() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CListOfCPoint3D *myCPList = new CListOfCPoint3D;
	CPoint3D P1(-450,-200,0);
	CPoint3D P2(-350,300,0);
	CPoint3D P3(-200,-300,0);
	CPoint3D P4(-200,400,0);
	CPoint3D P5(200,-300,0);
	CPoint3D P6(300,400,0);
	CPoint3D P7(350,-200,0);
	CPoint3D P8(400,300,0);
	myCPList->Append(P1);
	myCPList->Append(P2);
	myCPList->Append(P3);
	myCPList->Append(P4);
	myCPList->Append(P5);
	myCPList->Append(P6);
	myCPList->Append(P7);
	myCPList->Append(P8);

	CBezierCurve B1(myCPList);
	B1.Translate(0,100,300);

	CBSplineCurve B2(myCPList);
	B2.Translate(0,100,-300);
	
	CGLCurve* gC1 = new CGLCurve(&B1);
	CGLCurve* gC2 = new CGLCurve(&B2);

	dContext->Display(gC1);
	dContext->Display(gC2);

	CString str;
	str.Format("Rail Curve 1 (Bezier)");
	CGLFont* myFont1 = new CGLFont((LPCTSTR)str, B1.PointAtPara((B1.LastParameter()-B1.FirstParameter())/2));
	dContext->Display(myFont1);

	str.Format("Rail Curve 2 (B-Spline)");
	CGLFont* myFont2 = new CGLFont((LPCTSTR)str, B2.PointAtPara((B2.LastParameter()-B2.FirstParameter())/2));
	dContext->Display(myFont2);

	CRuledSurface R(&B1, &B2);
	CGLSurface* S = new CGLSurface(&R);
	dContext->Display(S);

	myCPList->Clear();
	delete myCPList;

	CAxisSystem Ax1(CPoint3D(0,-500,200), CVector3D(0,0,1));
	CCircle3D C(Ax1, 200);
	C.Reverse();
	CGLCurve* gC3 = new CGLCurve(&C);
	dContext->Display(gC3);

	CAxisSystem Ax2(CPoint3D(0,-500,-200), CVector3D(0,0,1));
	CEllipse3D E(Ax2, 160, 70);
	CGLCurve* gC4 = new CGLCurve(&E);
	dContext->Display(gC4);

	str.Format("Rail Curve 1 (Circle)");
	CGLFont* myFont3 = new CGLFont((LPCTSTR)str, C.PointAtPara((C.LastParameter()-C.FirstParameter())/2));
	dContext->Display(myFont3);

	str.Format("Rail Curve 2 (Ellipse)");
	CGLFont* myFont4 = new CGLFont((LPCTSTR)str, E.PointAtPara((E.LastParameter()-E.FirstParameter())/2));
	dContext->Display(myFont4);

	CRuledSurface R1(&C, &E);
	CGLSurface* S1 = new CGLSurface(&R1);
	dContext->Display(S1);

	delete gC1; delete gC2; delete gC3; delete gC4; delete S; delete S1;
	delete myFont1; delete myFont2; delete myFont3; delete myFont4;
}

void CCadSurfDoc::OnPipe() 
{
	// TODO: Add your command handler code here
	CString str;
	str.Format("Works Incorrectly If BSpline Or Ellipse Used As Spine!\nI Couldn't Get Exact Mathematical Formulae For Swept Pipe Surfaces\nI Have Applied My Own Logic!");
	MessageBox(0, str, "CadSurf", MB_OK | MB_ICONINFORMATION);	

	dContext->DeleteAll();

	::CWaitCursor aCur;
	CListOfCPoint3D *myCPList = new CListOfCPoint3D;
	CPoint3D P1(-450,-300,-00);
	CPoint3D P2(-350,300,-00);
	CPoint3D P3(-200,-300,-00);
	CPoint3D P4(-200,400,-00);
	CPoint3D P5(200,-300,-00);
	CPoint3D P6(300,400,-00);
	CPoint3D P7(350,-300,-00);
	CPoint3D P8(400,300,-00);
	myCPList->Append(P1);
	myCPList->Append(P2);
	myCPList->Append(P3);
	myCPList->Append(P4);
	myCPList->Append(P5);
	myCPList->Append(P6);
	myCPList->Append(P7);
	myCPList->Append(P8);

	CAxisSystem Ax(CPoint3D(0,-500,0), CVector3D(0,0,1), CVector3D(1,0,0));
	CParabola3D P(Ax, 60);
	P.SetYRange(-10, 10);

	CGLCurve* gC = new CGLCurve(&P);
	dContext->Display(gC);

	CBezierCurve B(myCPList);
	CGLCurve* gC1 = new CGLCurve(&B);
	dContext->Display(gC1);

	CPipeSurface S(&B, 50); //circular section along bezier
	CGLSurface* gS = new CGLSurface(&S);
	dContext->Display(gS);

	str.Format("Circular Section, Spine Curve Bezier");
	CGLFont* myFont2 = new CGLFont((LPCTSTR)str, B.PointAtPara(0.5));
	dContext->Display(myFont2);

	str.Format("Elliptical Section, Spine Curve Parabola");
	CGLFont* myFont1 = new CGLFont((LPCTSTR)str, P.PointAtPara(0));
	dContext->Display(myFont1);

	CPipeSurface S1(&P, 80, 50); //elliptical section along parabola
	CGLSurface* gS1 = new CGLSurface(&S1);
	dContext->Display(gS1);

	myCPList->Clear();
	delete myCPList;

	delete gC; delete gC1; delete gS; delete gS1;
	delete myFont1; delete myFont2;
}

//Offset Surface
void CCadSurfDoc::OnOffsetsurface() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	
	CAxisSystem Ax(CPoint3D(0,200, -300), CVector3D(0,0,1));
	CParabola3D Par(Ax, 150);
	Par.SetYRange(-5, 5);

	CGLCurve* gC = new CGLCurve(&Par);
	
	dContext->Display(gC);
	

	CExtrudedSurface E(&Par, CVector3D(0,0,1), -300, 300);
	CGLSurface* S = new CGLSurface(&E);
	dContext->Display(S);

	COffsetSurface O1(E, 300);
	CGLSurface* S1 = new CGLSurface(&O1);
	S1->SetLight(GOLD);
	dContext->Display(S1);

	COffsetSurface O2(E, 250, false);
	CGLSurface* S2 = new CGLSurface(&O2);
	S2->SetLight(COPPER);
	dContext->Display(S2);

	CString str;
	str.Format("Original Surface(Extruded)");
	CGLFont* myFont = new CGLFont((LPCTSTR)str, E.PointAtPara(0.5, 0.5));
	dContext->Display(myFont);

	str.Format("Offset Surface (Along Original Direction)");
	CGLFont* myFont1 = new CGLFont((LPCTSTR)str, O1.PointAtPara(0.5, 0.5));
	dContext->Display(myFont1);

	str.Format("Offset Surface (Away From Original Direction)");
	CGLFont* myFont2 = new CGLFont((LPCTSTR)str, O2.PointAtPara(0.5, 0.5));
	dContext->Display(myFont2);

	delete gC; delete S; delete S1; delete S2;
	delete myFont; delete myFont1; delete myFont2;
}


// User Defined surface creation
#include "UserSurface.h"
void CCadSurfDoc::OnUsersurface() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	
	CUserSurface *us = new CUserSurface();
	us->Scale(CPoint3D::Origin(), 100);
	CGLSurface* S = new CGLSurface(us);
	S->SetLight(BRONZE);
	dContext->Display(S);

	CString str;
	str.Format("User Defined Surface \"Klein Bottle Variant 2\"");
	CGLFont* myFont = new CGLFont((LPCTSTR)str, us->PointAtPara(0.5, 0.5));
	dContext->Display(myFont);	

	delete us; delete S; delete myFont;
}

void CCadSurfDoc::OnUpdateUsersurface(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

//Transformations

void CCadSurfDoc::OnTranslate() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double Rad = 100, Ht = 200;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CCone C(CAxisSystem(CPoint3D(200,0,0), CVector3D(0,0,1)), Rad, Ht);
	CGLSurface* gS = new CGLSurface(&C);
	dContext->Display(gS);

	CCone tC = C;
	tC.Translate(0,300,0);
	CGLSurface* gSt = new CGLSurface(&tC);
	dContext->Display(gSt);

	COneAxis Ax(CPoint3D(200,0,0), CVector3D(0,300,0));
	CGLOneAxis* gAx = new CGLOneAxis(Ax, 270, 30);
	dContext->Display(gAx);

	CPoint3D P1(-200,-300,-100);
	
	CGLPoint* gP1 = new CGLPoint(P1);
	dContext->Display(gP1);

	CCylinder C1(CAxisSystem(P1, CVector3D(1,0,1)), Rad, Ht);
	CGLSurface* gS1 = new CGLSurface(&C1);
	dContext->Display(gS1);

	CCylinder tC1 = C1;
	tC1.Translate(COneAxis::OX(), 400);
	CGLSurface* gSt1 = new CGLSurface(&tC1);
	dContext->Display(gSt1);

	COneAxis Ax1(P1, CVector3D(1,0,0));
	CGLOneAxis* gAx1 = new CGLOneAxis(Ax1, 460, 40);
	dContext->Display(gAx1);

	delete gP; delete gS; delete gSt; delete gAx;
	delete gP1; delete gS1; delete gSt1; delete gAx1;
}

void CCadSurfDoc::OnRotate() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CPoint3D P(200,0,0);
	double maj = 100, min = 50;

	CGLPoint* gP = new CGLPoint(P);
	dContext->Display(gP);

	CAxisSystem Ax1(P, CVector3D(0,0,1));
	CTorus S(Ax1, maj, min);
	CGLSurface* gS = new CGLSurface(&S);
	dContext->Display(gS);

	COneAxis Ax(CPoint3D(-200, 00, 0), CVector3D(1,1,1));
	CGLOneAxis* gAx = new CGLOneAxis(Ax);
	dContext->Display(gAx);

	CTorus tS = S;
	tS.Rotate(Ax, PI/2);
	CGLSurface* gS1 = new CGLSurface(&tS);
	dContext->Display(gS1);

	delete gP; delete gS; delete gAx; delete gS1;
}

void CCadSurfDoc::OnScale() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem Ax1(CPoint3D(0,-500,200), CVector3D(0,0,1));
	CCircle3D C(Ax1, 200);
	C.Reverse();

	CAxisSystem Ax2(CPoint3D(0,-500,-200), CVector3D(0,0,1));
	CEllipse3D E(Ax2, 160, 70);

	CRuledSurface R1(&C, &E);
	CGLSurface* S1 = new CGLSurface(&R1);
	dContext->Display(S1);

	CRuledSurface* sR = static_cast<CRuledSurface*>(R1.Copy());
	sR->Scale(CPoint3D::Origin(), -2.5);
	CGLSurface* S2 = new CGLSurface(sR);
	dContext->Display(S2);

	CRuledSurface* sR1 = static_cast<CRuledSurface*>(R1.Copy());
	sR1->Scale(CPoint3D::Origin(), 2.5);
	CGLSurface* S3 = new CGLSurface(sR1);
	dContext->Display(S3);

	delete S1; delete S2; delete sR; delete S3; delete sR1;
}

void CCadSurfDoc::OnReflect() 
{
	// TODO: Add your command handler code here
	dContext->DeleteAll();

	::CWaitCursor aCur;
	CAxisSystem Ax1(CPoint3D(400,500,-200), CVector3D(0,0,1));
	CCircle3D C(Ax1, 200);
	C.Reverse();

	CAxisSystem Ax2(CPoint3D(400,500,-400), CVector3D(0,0,1));
	CEllipse3D E(Ax2, 160, 70);

	CRuledSurface R1(&C, &E);
	CGLSurface* S1 = new CGLSurface(&R1);
	dContext->Display(S1);

	CString str;
	str.Format("Original Surface");
	CGLFont* myFont1 = new CGLFont((LPCTSTR)str, R1.PointAtPara(0.5, 0.5));
	dContext->Display(myFont1);

	CRuledSurface* sR = static_cast<CRuledSurface*>(R1.Copy());
	sR->Mirror(CPoint3D::Origin());
	CGLSurface* S2 = new CGLSurface(sR);
	S2->SetLight(GOLD);
	dContext->Display(S2);

	str.Format("Mirrored @ Point Origin");
	CGLFont* myFont2 = new CGLFont((LPCTSTR)str, sR->PointAtPara(0.5, 0.5));
	dContext->Display(myFont2);

	CRuledSurface* sR1 = static_cast<CRuledSurface*>(R1.Copy());
	sR1->Mirror(COneAxis::OY());
	CGLSurface* S3 = new CGLSurface(sR1);
	S3->SetLight(COPPER);
	dContext->Display(S3);

	str.Format("Mirrored @ Axis OY");
	CGLFont* myFont3 = new CGLFont((LPCTSTR)str, sR1->PointAtPara(0.5, 0.5));
	dContext->Display(myFont3);

	CRuledSurface* sR2 = static_cast<CRuledSurface*>(R1.Copy());
	sR2->Mirror(CPlane::XOY());
	CGLSurface* S4 = new CGLSurface(sR2);
	S4->SetLight(BRONZE);
	dContext->Display(S4);

	str.Format("Mirrored @ Plane XOY");
	CGLFont* myFont4 = new CGLFont((LPCTSTR)str, sR2->PointAtPara(0.5, 0.5));
	dContext->Display(myFont4);
	
	delete S1; delete sR; delete S2; delete sR1; delete S3; delete sR2; delete S4;
	delete myFont1; delete myFont2; delete myFont3; delete myFont4;
}

void CCadSurfDoc::OnEditDeleteall() 
{
	// TODO: Add your command handler code here
	undoStack.clear();
	redoStack.clear();
	dContext->DeleteAll();	
}

void CCadSurfDoc::OnUpdateEditDeleteall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!dContext->IsEmpty());
}

void CCadSurfDoc::OnEditDeleteselected() 
{
	// TODO: Add your command handler code here
	// Deletion history is intentionally outside the first STL undo/redo scope.
	// Drop transform/import history so it never refers to a deleted object.
	undoStack.clear();
	redoStack.clear();
	dContext->DeleteSelected();
}

void CCadSurfDoc::OnUpdateEditDeleteselected(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(dContext->HasSelected());
}

void CCadSurfDoc::UpdateViews()
{
	UpdateAllViews(NULL);
}


