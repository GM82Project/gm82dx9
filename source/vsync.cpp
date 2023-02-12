#include "gm82dx8.h"

int isdwm = 0;
HINSTANCE dwm_dll = 0;
DLL_FUNC DwmIsCompositionEnabled = 0;
DLL_FUNC DwmFlush = 0;
ULONGLONG resolution = 1000000, frequency = 1;

GMREAL __gm82dx8_not_xp() {
    return IsWindowsVistaOrGreater();
}
GMREAL __gm82dx8_dll_init() {
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
    
    //this seems to cause issues with dwm-grab screen recorders
    /*if (IsWindows8OrGreater()) {
        dwm_dll = LoadLibrary(TEXT("dwmapi.dll")); 
        DwmFlush = (DLL_FUNC) GetProcAddress(dwm_dll, "DwmFlush");
        isdwm = 1;
    } else if (IsWindowsVistaOrGreater()) {
        //DWM is possible. let's load it and see if it's enabled:
        dwm_dll = LoadLibrary(TEXT("dwmapi.dll")); 
        
        int enabled;
        DwmIsCompositionEnabled = (DLL_FUNC) GetProcAddress(dwm_dll, "DwmIsCompositionEnabled");        
        (DwmIsCompositionEnabled)(&enabled);        
        if (enabled) {
            //DWM exists and is enabled.
            DwmFlush = (DLL_FUNC) GetProcAddress(dwm_dll, "DwmFlush");
            isdwm = 1;
        }
    }*/
    return 0;    
}
GMREAL __gm82dx8_time_now() {
    ULONGLONG now;
    if (QueryPerformanceCounter((LARGE_INTEGER*)&now)) {
        return (double)(now*resolution/frequency);
    } else {
        return -1.0;
    }
}
GMREAL __gm82dx8_waitvblank() {    
    (*d3d8_device)->GetRasterStatus(0, &raster_status);
    if (raster_status.InVBlank) return 1;
    return 0;    
}
GMREAL __gm82dx8_sleep(double ms) {
    SleepEx((DWORD)ms,TRUE);
    return 0;
}
GMREAL __gm82dx8_sync_dwm() {
    if (isdwm) (DwmFlush)();
    else SleepEx(2,TRUE);
    return 0;
}