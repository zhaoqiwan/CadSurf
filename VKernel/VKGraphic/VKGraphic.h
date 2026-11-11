
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VKGRAPHIC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VKGRAPHIC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef VKGRAPHIC_EXPORTS
#define VKGRAPHIC_API __declspec(dllexport)
#else
#define VKGRAPHIC_API __declspec(dllimport)
#endif

/*
// This class is exported from the VKGraphic.dll
class VKGRAPHIC_API CVKGraphic {
public:
	CVKGraphic(void);
	// TODO: add your methods here.
};

extern VKGRAPHIC_API int nVKGraphic;

VKGRAPHIC_API int fnVKGraphic(void);
*/
