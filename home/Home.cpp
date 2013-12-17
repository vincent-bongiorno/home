#include "Home.h"
#include "MainWindow.h"

//#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
//# import "OpenUDID.h"
//#elif (TARGET_OS_MAC && !TARGET_OS_IPHONE)
//# import <Foundation/NSPathUtilities.h>
//#endif

NGL_APP_CREATE(Home);


/***********
 * Globals *
 ***********/
#pragma Globals
nglPath* gTempDirectory = NULL;
nglPath* gUserDocumentsDirectory = NULL;
nglPath* gLibraryDirectory = NULL;
nglPath* gDataDirectory = NULL;
nglPath* gPrefFilePath = NULL;
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
nglPath* gPurchasesDirectory = NULL;
#endif
char* gBundlePath = NULL;


/****************
 * Feature Keys *
 ****************/

void BM2GetFeatureKey(uint32 ID, nglString& rKey)
{
  rKey.CFormat(_T("bm2lic.%#08x"), ID);
}

/*************
 * BeatMaker *
 *************/
#pragma -
#pragma Home

Home::Home()
: mpWindow(NULL)
{
  App->GetLog().SetLevel("font", 0);
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
	// On startup, set the default orientation to Landscape right
//  UIApplication* pApp = [UIApplication sharedApplication];
//  pApp.statusBarOrientation = (UIInterfaceOrientation) UIDeviceOrientationLandscapeLeft;
#endif
  
  mpWindow = NULL;

}

Home::~Home()
{
}

void Home::OnWillExit()
{
  NGL_DEBUG(NGL_OUT(_T("[Home] OnWillExit !\n"));)
}

void Home::OnExit (int Code)
{
  NGL_DEBUG(NGL_OUT(_T("[Home] OnExit !\n"));)
  exit(0);
}

void Home::OnMemoryWarning()
{
	if (!mpWindow)
	{
  NGL_DEBUG(NGL_OUT(_T("[Home] /!\\ MEMORY WARNING /!\\ (No Window to display message)\n")); )
		return;
	}
}

void Home::OnDeactivation()
{
  // Override so Quit() is not called (iOS 4 multi-tasking)
}

void Home::OnOpenFile(const nglPath& rPath)
{
}

void Home::OnOpenURL(const nglString& rURL)
{
//  if (mpWindow)
//    mpWindow->OnOpenURL(rURL);
}

void Home::OnInit()
{
	nglString ver;
	ver.Format(_T("[Home v%d.%d.%d]\n"), HOME_VERSION_MAJ, HOME_VERSION_MIN, HOME_VERSION_REV);
  NGL_OUT(_T("%s"), ver.GetChars());

//	NGL_DEBUG(NGL_OUT(_T("[Startup] Available Memory=%.3F\n"), ial::GetAvailableMemory());)
//  nuiWidget::SetSelfClippingDefault(false);

  // Accept NGL default options
  nuiRenderer Renderer = eOpenGL;
  ParseDefaultArgs();

#if USE_APP_CONSOLE
  GLConsole* pConsole = new GLConsole();
  App->SetConsole(pConsole);
#endif
  
  GetLog().UseConsole(true);
	
  // Manual
  if ( (GetArgCount() == 1) &&
       ((!GetArg(0).Compare(_T("-h"))) || (!GetArg(0).Compare(_T("--help")))) )
  {
    Quit (0);
    return;
  }

// Create volumes and set application paths
//  SetPathsAndVolumes();
  
  // Window configuration
  uint Width = 0, Height = 0;
  uint Angle = 270;
  bool IsFullScreen = false;
  bool DebugObject = false;
  bool PartialRedraw = true;
  
  nuiMainWindow::SetRenderer(Renderer);

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)

//  ial::iOSDeviceFamily f;
//  ial::iOSDeviceType t;
//  ial::GetiOSDevice(f, t);
  
//  if (t == eiOSDeviceTypeiPad3 && SYSTEM_VERSION_LESS_THAN(@"6.0"))
//  {
//    NGL_OUT(_T("*** iPad 3 detected, disabling partial redraw (temp fix)\n"));
//    PartialRedraw = false;
//  }
  
  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  ContextInfo.CopyOnSwap = PartialRedraw;
 
  nglWindowInfo Info;

  Info.Flags = IsFullScreen ? nglWindow::FullScreen : 0;
  Info.Flags |= nglWindow::NoResize;
  Info.Width = Width;
  Info.Height = Height;
  Info.Rotate = Angle;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = _T("Home");
  Info.XPos = 0;
  Info.YPos = 0;

  MainWindow* pMainWin = new MainWindow(ContextInfo, Info);
  pMainWin->Acquire();
  
//  pMainWin->EnableAutoRotation(false);
  pMainWin->SetRotation(Angle);
  mpWindow = pMainWin;
  if ((!pMainWin) || (pMainWin->GetError()))
  {
    if (pMainWin) {
      NGL_DEBUG(NGL_OUT(_T("[Home] Error: cannot create window (%s)\n"), pMainWin->GetErrorStr());)
    }
    Quit (1);
    return;
  }
  mpWindow->SetState(nglWindow::eMaximize);
  mpWindow->EnablePartialRedraw(PartialRedraw);

#else // Desktop flavor
  //IsFullScreen = true;
  if (IsFullScreen)
  {
    nglVideoMode current_mode;
    
    Width = current_mode.GetWidth();
    Height = current_mode.GetHeight();
  }
  else
  {
    Width = 1024;
    Height = 768;
  }
  
  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  nglWindowInfo Info;
    
  NSString* pAppName = [[NSBundle mainBundle] objectForInfoDictionaryKey: (NSString *) kCFBundleNameKey];
  NSString* pVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey: (NSString *) kCFBundleVersionKey];
  nglString version([pVersion UTF8String]);
  nglString title([pAppName UTF8String]);
//  title += _T(" ");
//  title += version;
  
  Info.Flags = nglWindow::NoResize | (IsFullScreen ? nglWindow::FullScreen : 0);
  Info.Width = Width;
  Info.Height = Height;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = title;
  Info.XPos = 0;
  Info.YPos = 0;
  
  mpWindow = new MainWindow(ContextInfo, Info);  
  if ((!mpWindow) || (mpWindow->GetError()))
  {
    if (mpWindow) 
      NGL_OUT(_T("Error: cannot create window (%s)\n"), mpWindow->GetErrorStr());
    Quit (1);
    return;
  }
  mpWindow->Acquire();

  //mpWindow->DBG_SetMouseOverInfo(true);
  //mpWindow->DBG_SetMouseOverObject(true);
  mpWindow->SetState(nglWindow::eShow);  
	mpWindow->EnablePartialRedraw(true);
  mpWindow->SetDebug(0);
  mpWindow->SetDebugMode(false);

#endif  
  
  nuiAnimation::SetFrameRate(60.0);
}


#pragma mark -

MainWindow* GetMainWindow()
{
	return ((Home*) App)->GetMainWindow();
}
