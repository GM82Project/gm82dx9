#include "gm82dx8.h"


HRESULT WINAPI SetVertexShader(IDirect3DDevice9 *dev, DWORD fvf) {
    return dev->SetFVF(fvf);
}