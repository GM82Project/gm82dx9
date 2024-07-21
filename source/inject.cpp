#include "gm82dx9.h"

extern IDirect3DTexture9 *white_pixel = nullptr;
extern D3DPRESENT_PARAMETERS* present_params;

D3DCAPS9 d3d_caps;
D3DPRESENT_PARAMETERS d3d_parameters = {
        .BackBufferCount = 1,
        .MultiSampleType = D3DMULTISAMPLE_NONE,
        .SwapEffect = D3DSWAPEFFECT_COPY,
        .Windowed = 1,
        .EnableAutoDepthStencil = TRUE,
        .AutoDepthStencilFormat = D3DFMT_D24S8,
        .PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE,
};

HRESULT WINAPI CheckDeviceMultiSampleType(IDirect3D9 *d3d, UINT Adapter,
                                          D3DDEVTYPE DeviceType,
                                          D3DFORMAT SurfaceFormat,
                                          BOOL Windowed,
                                          D3DMULTISAMPLE_TYPE MultiSampleType) {
    return d3d->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, nullptr);
}

HRESULT WINAPI GetDisplayMode(IDirect3DDevice9 *dev,
                              D3DDISPLAYMODE *pMode
) {
    return dev->GetDisplayMode(0, pMode);
}

HRESULT WINAPI CreateImageSurface(IDirect3DDevice9 *dev,
                                  UINT Width,
                                  UINT Height,
                                  D3DFORMAT Format,
                                  IDirect3DSurface9 **ppSurface
) {
    return dev->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SCRATCH, ppSurface, nullptr);
}

HRESULT WINAPI GetBackBuffer(IDirect3DDevice9 *dev,
                             UINT BackBuffer,
                             D3DBACKBUFFER_TYPE Type,
                             IDirect3DSurface9 **ppBackBuffer
) {
    return dev->GetBackBuffer(0, BackBuffer, Type, ppBackBuffer);
}

HRESULT WINAPI CreateVertexBuffer(IDirect3DDevice9 *dev,
                                  UINT Length,
                                  DWORD Usage,
                                  DWORD FVF,
                                  D3DPOOL Pool,
                                  IDirect3DVertexBuffer9 **ppVertexBuffer
) {
    return dev->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, nullptr);
}

HRESULT WINAPI SetStreamSource(IDirect3DDevice9 *dev,
                               UINT StreamNumber,
                               IDirect3DVertexBuffer9 *pStreamData,
                               UINT Stride
) {
    return dev->SetStreamSource(StreamNumber, pStreamData, 0, Stride);
}

HRESULT WINAPI CreateDepthStencilSurface(IDirect3DDevice9 *dev,
                                         UINT Width,
                                         UINT Height,
                                         D3DFORMAT Format,
                                         D3DMULTISAMPLE_TYPE MultiSample,
                                         IDirect3DSurface9 **ppSurface
) {
    return dev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, 0, FALSE, ppSurface, nullptr);
}

HRESULT
WINAPI SetRenderTarget(IDirect3DDevice9 *dev, IDirect3DSurface9 *pRenderTarget, IDirect3DSurface9 *pNewZStencil) {
    HRESULT hr = dev->SetRenderTarget(0, pRenderTarget);
    if (SUCCEEDED(hr)) hr = dev->SetDepthStencilSurface(pNewZStencil);
    return hr;
}

HRESULT WINAPI GetRenderTarget(IDirect3DDevice9 *dev, IDirect3DSurface9 **ppRenderTarget) {
    return dev->GetRenderTarget(0, ppRenderTarget);
}

HRESULT WINAPI CopyRects(IDirect3DDevice9 *dev,
                         IDirect3DSurface9 *pSourceSurface,
                         CONST RECT *pSourceRectsArray,
                         UINT cRects,
                         IDirect3DSurface9 *pDestinationSurface,
                         CONST POINT *pDestPointsArray
) {
    RECT destRect;
    destRect.left = pDestPointsArray->x;
    destRect.top = pDestPointsArray->y;
    destRect.right = destRect.left + (pSourceRectsArray->right - pSourceRectsArray->left);
    destRect.bottom = destRect.top + (pSourceRectsArray->bottom - pSourceRectsArray->top);
    // TODO AddDirtyRect if needed
    return D3DXLoadSurfaceFromSurface(pDestinationSurface, nullptr, &destRect, pSourceSurface, nullptr,
                                      pSourceRectsArray, D3DX_FILTER_NONE, 0);
}

HRESULT WINAPI
D3DXGetErrorStringA(
        HRESULT hr,
        LPSTR pBuffer,
        UINT BufferLen) {
    const wchar_t *wstr = DXGetErrorStringW(hr);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, pBuffer, BufferLen, nullptr, nullptr);
    return S_OK;
}

HRESULT WINAPI screen_refresh(IDirect3DDevice9 *dev, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestOverride, const RGNDATA *pDirtyRegion) {
    dev->EndScene();
    auto res = dev->Present(pSourceRect, pDestRect, hDestOverride, pDirtyRegion);
    dev->BeginScene();
    return res;
}

void WINAPI regain_device() {
    // force exclusive fullscreen off
    d3d_parameters.Windowed = TRUE;
    d3d_parameters.FullScreen_RefreshRateInHz = 0;
    (*runner_display_reset)();
}

short old_cw = 0;
short new_cw = 0;

HRESULT WINAPI CreateDevice(IDirect3D9 *d3d9, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags,
                            D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface
) {
    // _control87 doesn't seem to let us reset the control word, so we're going asm
    if (old_cw == 0) {
        _asm fnstcw [old_cw];
        new_cw = old_cw | 0x3f;
        _asm fldcw [new_cw];
    }
    
    present_params = pPresentationParameters;
	
	// get window size from first room
	int first_room_id = **(int**)0x8452d4;
	char *room_ptr = (*(char***)0x686a4c)[first_room_id];
	int desired_width = 0, desired_height = 0;
	if (room_ptr[0x40]) {
		// views
		for (int i = 0; i < 8; i++) {
			int *view_ptr = *(int**)(room_ptr + 0x44 + i * 4);
			int port_width = view_ptr[6] + view_ptr[8];
			int port_height = view_ptr[7] + view_ptr[9];
			if (port_width > desired_width)
				desired_width = port_width;
			if (port_height > desired_height)
				desired_height = port_height;
		}
	} else {
		// no views
		desired_width = ((int*)room_ptr)[3];
		desired_height = ((int*)room_ptr)[4];
	}
	if (desired_width != 0 && desired_height != 0 && desired_width < present_params->BackBufferWidth && desired_height < present_params->BackBufferHeight) {
		present_params->BackBufferWidth = desired_width;
		present_params->BackBufferHeight = desired_height;
	}
    
    auto res = d3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    return res;
}

#define CW_INJ_START(name)                        \
    __declspec(naked) void name##_inj() { __asm { \
        __asm fldcw [old_cw]
#define CW_INJ_END() \
        __asm fldcw [new_cw] \
        __asm ret 0xc        \
        }                    \
    }
#define CW_INJ_ENDD()        \
        __asm fldcw [new_cw] \
        __asm ret 0x8        \
        }                    \
    }
#define CW_INJ_END2()            \
            __asm fldcw [new_cw] \
            __asm ret 0x18       \
        }                        \
    }


CW_INJ_START(sqrt)
    _asm fld tbyte ptr [esp + 4]
    _asm fsqrt
CW_INJ_END()

CW_INJ_START(ln)
    fld tbyte ptr [esp + 4]
    fldln2
    fxch
    fyl2x
CW_INJ_END()

CW_INJ_START(log2)
    fld1
    fld tbyte ptr [esp + 4]
    fyl2x
CW_INJ_END()

CW_INJ_START(log10)
    fldlg2
    fld tbyte ptr [esp + 4]
    fyl2x
CW_INJ_END()

CW_INJ_START(arcsin)
    fld qword ptr [esp + 4]
    fld1
    fadd st(0), st(1)
    fld1
    fsub st(0), st(2)
    fmulp st(1), st(0)
    fsqrt
    fpatan
CW_INJ_ENDD()

CW_INJ_START(arccos)
    fld qword ptr [esp + 4]
    fld1
    fadd st(0), st(1)
    fld1
    fsub st(0), st(2)
    fmulp st(1), st(0)
    fsqrt
    fxch
    fpatan
CW_INJ_ENDD()

CW_INJ_START(arctan)
    fld tbyte ptr [esp + 4]
    fld1
    fpatan
CW_INJ_END()

CW_INJ_START(arctan2)
    fld tbyte ptr [esp + 0x10]
    fld tbyte ptr [esp + 4]
    fpatan
CW_INJ_END2()

CW_INJ_START(logn)
    fld1
    fld tbyte ptr [esp + 4]
    fyl2x
    fld1
    fld tbyte ptr [esp + 0x10]
    fyl2x
    fdivp st(1), st(0)
CW_INJ_END2()

CW_INJ_START(exp)
    __asm push dword ptr [esp + 0xc]
    __asm push dword ptr [esp + 0xc]
    __asm push dword ptr [esp + 0xc]
    __asm mov eax, 0x404844
    __asm call eax
CW_INJ_END()

CW_INJ_START(power)
    __asm push dword ptr [esp + 0x10]
    __asm push dword ptr [esp + 0x10]
    __asm push dword ptr [esp + 0x10]
    __asm push dword ptr [esp + 0x10]
    __asm mov eax, 0x4103d8
    __asm call eax
    __asm fldcw [new_cw]
    __asm ret 0x10
}}

uint8_t white_pixel_tga[] = {
    0, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 0,
    32, 0, 255, 255, 255, 255
};
void create_white_pixel() {
    D3DXCreateTextureFromFileInMemory(Device, white_pixel_tga, 22, &white_pixel);
}

HRESULT WINAPI SetNullTexture(IDirect3DDevice9 *dev, DWORD Stage, IDirect3DBaseTexture9 *pTexture) {
    return dev->SetTexture(0, white_pixel);
}

const uint8_t reset_patch[] = {
        // note: 0xdc = sizeof(DEVMODEW), 0x44 = offset(dmSize), 0x46 = offset(dmDriverExtra),
        // 0xa0 = offset(dmBitsPerPel)

        // sub esp, 0xdc
        0x81, 0xec, 0xdc, 0x00, 0x00, 0x00,

        // get registry settings
        // mov dword ptr [esp+0x44], 0xdc
        0xc7, 0x44, 0x24, 0x44, 0xdc, 0x00, 0x00, 0x00,
        // push esp
        0x54,
        // push -2
        0x6a, 0xfe,
        // push 0
        0x6a, 0x00,
        // call EnumDisplaySettingsW
        0xe8, 0xf0, 0xf8, 0xde, 0xff,

        // get current settings
        // sub esp, 0xdc
        0x81, 0xec, 0xdc, 0x00, 0x00, 0x00,
        // mov word ptr [esp+0x44], 0xdc
        0xc7, 0x44, 0x24, 0x44, 0xdc, 0x00, 0x00, 0x00,
        // push esp
        0x54,
        // push -1
        0x6a, 0xff,
        // push 0
        0x6a, 0x00,
        // call EnumDisplaySettingsW
        0xe8, 0xd8, 0xf8, 0xde, 0xff,

        // compare blocks with
        // lea eax, [esp+0xa8]
        0x8d, 0x84, 0x24, 0xa8, 0x00, 0x00, 0x00,
        // lea edx, [esp+0x184]
        0x8d, 0x94, 0x24, 0x84, 0x01, 0x00, 0x00,
        // xor ecx, ecx
        0x33, 0xc9,
        // mov cl, 0x14
        0xb1, 0x14,
        // call CompareMem
        0xe8, 0xb5, 0x68, 0xdf, 0xff,

        // are they the same?
        // test al, al
        0x84, 0xc0,
        // jne 2f
        0x75, 0x09,

        // restore display settings
        // push 0 (x2)
        0x6a, 0x00, 0x6a, 0x00,
        // call ChangeDisplaySettingsW
        0xe8, 0x18, 0xf7, 0xde, 0xff,

        // exit
        // add esp, 0x1b8
        0x81, 0xc4, 0xb8, 0x01, 0x00, 0x00,
        // ret
        0xc3,
};

HINSTANCE my_handle;

void WINAPI last_resort_impl(HANDLE hLibModule) {
    if (hLibModule == my_handle && Device == nullptr) {
        HANDLE proc = GetCurrentProcess();
        WriteProcessMemory(proc, (void*)(0x61ede0), reset_patch, sizeof(reset_patch), nullptr);
        int ptr = 0x61ede0 - (0x561bb0 + 5);
        WriteProcessMemory(proc, (void*)(0x561bb0 + 1), &ptr, 4, nullptr);
        FlushInstructionCache(proc, (void*)0x61ede0, sizeof(reset_patch));
        FlushInstructionCache(proc, (void*)0x561bb0, 5);
    }
}

__declspec(naked) void last_resort() {
    _asm {
        push [esp+4]
        call last_resort_impl
        mov eax, 0x40dd98
        jmp eax
    }
}

BOOL WINAPI DllMain(
        _In_ HINSTANCE hinstDLL,
        _In_ DWORD fdwReason,
        _In_ LPVOID lpvReserved
) {
    if (fdwReason == DLL_PROCESS_DETACH) {
        HANDLE proc = GetCurrentProcess();
        int ptr = 0x40dd98 - (0x5795c5 + 5);
        WriteProcessMemory(proc, (void*)(0x5795c5 + 1), &ptr, 4, nullptr);
        FlushInstructionCache(proc, (void*)0x5795c5, 5);
        return TRUE;
    }
    if (fdwReason != DLL_PROCESS_ATTACH) return TRUE;
    my_handle = hinstDLL;

    HANDLE proc = GetCurrentProcess();

    void *ptr;
    uint16_t offset;

    // SDK version
    ptr = (void *) (D3D_SDK_VERSION);
    WriteProcessMemory(proc, (void *) (0x61ee6b + 1), &ptr, 4, nullptr);

    // Direct3DCreate9
    ptr = (char *) (&Direct3DCreate9) - (0x510fe0);
    WriteProcessMemory(proc, (void *) (0x510fdb + 1), &ptr, 4, nullptr);

    // D3DPRESENT_PARAMETERS setup
    ptr = &d3d_parameters;
    WriteProcessMemory(proc, (void *) (0x61edfd + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61faa1 + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61fad0 + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61fb15 + 1), &ptr, 4, nullptr);
    uint8_t short_jmp[] = {0xeb, 0x26};
    WriteProcessMemory(proc, (void *) (0x61eece), short_jmp, 2, nullptr);
    uint8_t nops[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    offset = 0x18;
    WriteProcessMemory(proc, (void *) (0x61ef11 + 2), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ef09), nops, 3, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ef19), nops, 7, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ef25), nops, 3, nullptr);
    // set AutoDepthStencilFormat on fail
    offset = 0x28;
    WriteProcessMemory(proc, (void *) (0x61efb2 + 2), &offset, 1, nullptr);
    // set SwapEffect on fail
    offset = 0x18;
    WriteProcessMemory(proc, (void *) (0x61f015 + 2), &offset, 1, nullptr);

    // D3DCAPS setup
    ptr = &d3d_caps;
    WriteProcessMemory(proc, (void *) (0x61f0df + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61fb3a + 1), &ptr, 4, nullptr);
    ptr = &d3d_caps.MaxPrimitiveCount;
    WriteProcessMemory(proc, (void *) (0x61f0ef + 1), &ptr, 4, nullptr);
    ptr = &d3d_caps.RasterCaps;
    WriteProcessMemory(proc, (void *) (0x61e860 + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ea1f + 2), &ptr, 4, nullptr);

    // D3DPRESENT_PARAMETERS on reset
    ptr = &d3d_parameters.SwapEffect;
    WriteProcessMemory(proc, (void *) (0x61fa79 + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61fb10 + 1), &ptr, 4, nullptr);
    ptr = &d3d_parameters.MultiSampleType;
    WriteProcessMemory(proc, (void *) (0x61fa83 + 1), &ptr, 4, nullptr);
    WriteProcessMemory(proc, (void *) (0x61fb06 + 1), &ptr, 4, nullptr);

    // CheckDeviceMultiSampleType
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&CheckDeviceMultiSampleType) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x61f112);
    PATCH(0x61f134);
    PATCH(0x61f156);
    PATCH(0x61f178);
    PATCH(0x61fa58);
#undef PATCH

    // CreateDevice
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&CreateDevice) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x61ef94);
    PATCH(0x61eff7);
    PATCH(0x61f05a);
    PATCH(0x61f099);
#undef PATCH

    // CreateVertexBuffer
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&CreateVertexBuffer) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x5687cd);
#undef PATCH

    // CreateDepthStencilSurface
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&CreateDepthStencilSurface) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x56b44f);
    PATCH(0x56b483);
#undef PATCH

    // SetRenderTarget
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&SetRenderTarget) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x56b771);
    PATCH(0x56b835);
#undef PATCH

    // CopyRects
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&CopyRects) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x56bce3);
    PATCH(0x56bfe9);
    PATCH(0x620209);
#undef PATCH

    // GetDisplayMode
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&GetDisplayMode) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x620158);
#undef PATCH

    // CreateImageSurface
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&CreateImageSurface) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x56bc73);
    PATCH(0x620186);
#undef PATCH

    // GetBackBuffer
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&GetBackBuffer) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x6201b2);
#undef PATCH

    // screen_refresh
#define PATCH(a) \
                 offset = 0xe8; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 1, nullptr); \
                 ptr = ((char*)(&screen_refresh) - (a + 5)); \
                 WriteProcessMemory(proc, (void*)(a + 1), &ptr, 4, nullptr)
    PATCH(0x6200c2);
#undef PATCH

    // SetStreamSource
#define PATCH(a) \
                 offset = 0xe890; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 2, nullptr); \
                 ptr = ((char*)(&SetStreamSource) - (a + 6)); \
                 WriteProcessMemory(proc, (void*)(a + 2), &ptr, 4, nullptr)
    PATCH(0x568b6c);
    PATCH(0x568bc2);
    PATCH(0x568c17);
#undef PATCH

    // GetRenderTarget
#define PATCH(a) \
                 offset = 0xe890; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 2, nullptr); \
                 ptr = ((char*)(&GetRenderTarget) - (a + 6)); \
                 WriteProcessMemory(proc, (void*)(a + 2), &ptr, 4, nullptr)
    PATCH(0x56b712);
#undef PATCH

    // SetTexture(0, NULL)
#define PATCH(a) \
                 offset = 0xe890; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 2, nullptr); \
                 ptr = ((char*)(&SetNullTexture) - (a + 6)); \
                 WriteProcessMemory(proc, (void*)(a + 2), &ptr, 4, nullptr)
    PATCH(0x5662e5); // SetTexture
    PATCH(0x566376); // SetTexture
    PATCH(0x56642c); // SetTexture
    PATCH(0x56658d); // SetTexture
    PATCH(0x56665e); // SetTexture
    PATCH(0x5667ff); // SetTexture
    PATCH(0x5668bd); // SetTexture
    PATCH(0x5669ca); // SetTexture
    PATCH(0x566b83); // SetTexture
    PATCH(0x566d86); // SetTexture
    PATCH(0x566f7e); // SetTexture
    PATCH(0x567448); // SetTexture
    PATCH(0x6212ed); // SetTexture
#undef PATCH

    // initialize white pixel texture
    ptr = ((char*)(&create_white_pixel) - (0x627e5a + 5));
    WriteProcessMemory(proc, (void*)(0x627e5a + 1), &ptr, 4, nullptr);

#define PATCH_SIMPLE(a, off) \
        offset = off;        \
        WriteProcessMemory(proc, (void*)(a + 2), &offset, 1, nullptr)
#define PATCH_DOUBLE(a, off) \
        offset = off;        \
        WriteProcessMemory(proc, (void*)(a + 2), &offset, 2, nullptr)
    PATCH_SIMPLE(0x61faae, 0x40); // Reset
    PATCH_SIMPLE(0x61fadd, 0x40); // Reset
    PATCH_SIMPLE(0x61fb22, 0x40); // Reset
    PATCH_SIMPLE(0x563aae, 0xac); // Clear
    PATCH_SIMPLE(0x566266, 0xac); // Clear
    PATCH_SIMPLE(0x56629b, 0xac); // Clear
    PATCH_SIMPLE(0x61f1a6, 0xac); // Clear
    PATCH_SIMPLE(0x61fb61, 0xac); // Clear
    PATCH_SIMPLE(0x61fc86, 0xbc); // SetViewport
    PATCH_SIMPLE(0x56475e, 0xc4); // SetMaterial
    PATCH_SIMPLE(0x56484f, 0xcc); // SetLight
    PATCH_SIMPLE(0x564941, 0xcc); // SetLight
    PATCH_SIMPLE(0x564971, 0xd4); // LightEnable

    PATCH_SIMPLE(0x563c33, 0xb0); // SetTransform
    PATCH_SIMPLE(0x563cef, 0xb0); // SetTransform
    PATCH_SIMPLE(0x563d52, 0xb0); // SetTransform
    PATCH_SIMPLE(0x563e66, 0xb0); // SetTransform
    PATCH_SIMPLE(0x563ea9, 0xb0); // SetTransform
    PATCH_SIMPLE(0x563fbb, 0xb0); // SetTransform
    PATCH_SIMPLE(0x564002, 0xb0); // SetTransform
    PATCH_SIMPLE(0x56403e, 0xb0); // SetTransform
    PATCH_SIMPLE(0x56408c, 0xb0); // SetTransform
    PATCH_SIMPLE(0x5640dc, 0xb0); // SetTransform
    PATCH_SIMPLE(0x564128, 0xb0); // SetTransform
    PATCH_SIMPLE(0x564184, 0xb0); // SetTransform
    PATCH_SIMPLE(0x5641e0, 0xb0); // SetTransform
    PATCH_SIMPLE(0x564266, 0xb0); // SetTransform
    PATCH_SIMPLE(0x5642f6, 0xb0); // SetTransform
    PATCH_SIMPLE(0x564376, 0xb0); // SetTransform
    PATCH_SIMPLE(0x5643f2, 0xb0); // SetTransform
    PATCH_SIMPLE(0x56447e, 0xb0); // SetTransform
    PATCH_SIMPLE(0x56450a, 0xb0); // SetTransform
    PATCH_SIMPLE(0x5645c9, 0xb0); // SetTransform
    PATCH_SIMPLE(0x56466d, 0xb0); // SetTransform
    PATCH_SIMPLE(0x5646a9, 0xb0); // SetTransform
    PATCH_SIMPLE(0x568b11, 0xb0); // SetTransform
    PATCH_SIMPLE(0x568c45, 0xb0); // SetTransform
    PATCH_SIMPLE(0x61fda9, 0xb0); // SetTransform
    PATCH_SIMPLE(0x61fdee, 0xb0); // SetTransform
    PATCH_SIMPLE(0x61fed9, 0xb0); // SetTransform
    PATCH_SIMPLE(0x61ff1a, 0xb0); // SetTransform

    PATCH_SIMPLE(0x5642c8, 0xb4); // GetTransform
    PATCH_SIMPLE(0x564348, 0xb4); // GetTransform
    PATCH_SIMPLE(0x5643c4, 0xb4); // GetTransform
    PATCH_SIMPLE(0x564450, 0xb4); // GetTransform
    PATCH_SIMPLE(0x5644dc, 0xb4); // GetTransform
    PATCH_SIMPLE(0x56459b, 0xb4); // GetTransform
    PATCH_SIMPLE(0x564636, 0xb4); // GetTransform
    PATCH_SIMPLE(0x568acc, 0xb4); // GetTransform

    PATCH_SIMPLE(0x563b73, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x564793, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e8d1, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e8e3, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e8f5, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e907, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e91b, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e92d, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e951, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e967, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e98a, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e99d, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e9de, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61e9f8, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ea19, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ea37, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ea6c, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ea84, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ea9c, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61eab0, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61eb23, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61eb39, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61eb63, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61eb77, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ed41, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x61ed56, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x63a576, 0xe4); // SetRenderState
    PATCH_SIMPLE(0x63a58c, 0xe4); // SetRenderState

    PATCH_DOUBLE(0x61eba5, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ebb9, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ebcf, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ebe3, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ecd1, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ece5, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ecfb, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x61ed0f, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x621425, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x62143b, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x621585, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x62159b, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x6216ee, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x621704, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x622540, 0x114); // SetSamplerState
    PATCH_DOUBLE(0x622556, 0x114); // SetSamplerState

    offset = D3DSAMP_MAGFILTER;
    WriteProcessMemory(proc, (void *) (0x61eb99 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ebc3 + 1), &offset, 1, nullptr);
    offset = D3DSAMP_MINFILTER;
    WriteProcessMemory(proc, (void *) (0x61ebad + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ebd7 + 1), &offset, 1, nullptr);
    offset = D3DSAMP_ADDRESSU;
    WriteProcessMemory(proc, (void *) (0x61ecc5 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ecef + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x621417 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x621577 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x6216e0 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x622532 + 1), &offset, 1, nullptr);
    offset = D3DSAMP_ADDRESSV;
    WriteProcessMemory(proc, (void *) (0x61ecd9 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x61ed03 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x62142d + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x62158d + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x6216f6 + 1), &offset, 1, nullptr);
    WriteProcessMemory(proc, (void *) (0x622548 + 1), &offset, 1, nullptr);

    PATCH_DOUBLE(0x61ec15, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61ec29, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61ec3d, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61ec51, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61ec65, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61ec79, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61ec8f, 0x10c); // SetTextureStageState
    PATCH_DOUBLE(0x61eca3, 0x10c); // SetTextureStageState

    PATCH_SIMPLE(0x61ff50, 0xa4); // BeginScene
    PATCH_SIMPLE(0x61ff77, 0xa8); // EndScene
    PATCH_SIMPLE(0x61ffff, 0x44); // Present

    // SetTexture with actual texture
    PATCH_DOUBLE(0x6212d7, 0x104); // SetTexture
    PATCH_DOUBLE(0x621451, 0x104); // SetTexture
    PATCH_DOUBLE(0x6215b1, 0x104); // SetTexture
    PATCH_DOUBLE(0x62171a, 0x104); // SetTexture
    PATCH_DOUBLE(0x622575, 0x104); // SetTexture

    PATCH_DOUBLE(0x568b87, 0x144); // DrawPrimitive
    PATCH_DOUBLE(0x568bdc, 0x144); // DrawPrimitive
    PATCH_DOUBLE(0x568c2a, 0x144); // DrawPrimitive

    PATCH_DOUBLE(0x565041, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566312, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x5663a3, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566455, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x5665b6, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566687, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566828, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x5668f3, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x56690c, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566a02, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566a1b, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566bb9, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566bd2, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566dbe, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566dd7, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566fcc, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x566fed, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x567499, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x5674bd, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x56797b, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x62147d, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x6215dd, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x621746, 0x14c); // DrawPrimitiveUP
    PATCH_DOUBLE(0x6227b5, 0x14c); // DrawPrimitiveUP

    // SetVertexShader
#define PATCH(a) \
                 offset = 0xe890; \
                 WriteProcessMemory(proc, (void*)(a), &offset, 2, nullptr); \
                 ptr = ((char*)(&SetVertexShader) - (a + 6)); \
                 WriteProcessMemory(proc, (void*)(a + 2), &ptr, 4, nullptr)
    PATCH(0x565028);
    PATCH(0x5662f7);
    PATCH(0x566388);
    PATCH(0x56643e);
    PATCH(0x56659f);
    PATCH(0x566670);
    PATCH(0x566811);
    PATCH(0x5668cf);
    PATCH(0x5669dc);
    PATCH(0x566b95);
    PATCH(0x566d98);
    PATCH(0x566f90);
    PATCH(0x56745a);
    PATCH(0x567962);
    PATCH(0x568b28);
    PATCH(0x621466);
    PATCH(0x6215c6);
    PATCH(0x62172f);
    PATCH(0x62258a);
#undef PATCH

    PATCH_SIMPLE(0x621329, 0x48); // GetSurfaceLevel
    PATCH_SIMPLE(0x56b741, 0xa0); // GetDepthStencilSurface

    PATCH_SIMPLE(0x6202a4, 0x34); // LockRect
    PATCH_SIMPLE(0x56bd0a, 0x34); // LockRect
    PATCH_SIMPLE(0x6202f9, 0x38); // UnlockRect
    PATCH_SIMPLE(0x56bd75, 0x38); // UnlockRect

    // overwrite d3dx
    *(void **) (0x68fde8) = &D3DXGetErrorStringA;
    *(void **) (0x68fdec) = &D3DXCheckTextureRequirements;
    *(void **) (0x68fdf0) = &D3DXCreateTexture;
    *(void **) (0x68fdf4) = &D3DXCreateTextureFromFileInMemoryEx;
#define PATCH_D3DX(a, f) \
        offset = 0xb890; \
        WriteProcessMemory(proc, (void*)(a), &offset, 2, nullptr); \
        ptr = &f;        \
        WriteProcessMemory(proc, (void*)(a+2), &ptr, 4, nullptr);
    PATCH_D3DX(0x53122f, D3DXCheckTextureRequirements);
    PATCH_D3DX(0x531241, D3DXCreateTexture);
    PATCH_D3DX(0x531253, D3DXCreateTextureFromFileInMemoryEx);
    PATCH_D3DX(0x531277, D3DXGetErrorStringA);

#define PATCH(addr, func) \
        ptr = (char*)(&func) - (addr + 5); \
        WriteProcessMemory(proc, (void*)(addr + 1), &ptr, 4, nullptr);

    PATCH(0x620012, regain_device)
    PATCH(0x5795c5, last_resort)

    // maths
    PATCH(0x633d70, sqrt_inj)
    PATCH(0x633e3c, exp_inj)
    PATCH(0x633ed4, ln_inj)
    PATCH(0x633f68, log2_inj)
    PATCH(0x634000, log10_inj)
    PATCH(0x634110, arcsin_inj)
    PATCH(0x6341a8, arccos_inj)
    PATCH(0x634244, arctan_inj)
    PATCH(0x6342ea, arctan2_inj)
    PATCH(0x63440e, power_inj)
    PATCH(0x6344b6, logn_inj)

    FlushInstructionCache(proc, nullptr, 0);

    return TRUE;
}
