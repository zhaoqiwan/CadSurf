
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VKGEOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VKGEOM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef VKGEOM_EXPORTS
#define VKGEOM_API __declspec(dllexport)
#else
#define VKGEOM_API __declspec(dllimport)
#endif

/*
// This class is exported from the VKGeom.dll
class VKGEOM_API CVKGeom {
public:
	CVKGeom(void);
	// TODO: add your methods here.
};

extern VKGEOM_API int nVKGeom;

VKGEOM_API int fnVKGeom(void);
*/
