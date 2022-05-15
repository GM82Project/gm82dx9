#include "gm82dx8.h"

GMREAL dx8_transform_xyzt(double rx, double ry, double rz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yxzt(double ry, double rx, double rz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zxyt(double rz, double rx, double ry, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_xzyt(double rx, double rz, double ry, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yzxt(double ry, double rz, double rx, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zyxt(double rz, double ry, double rx, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_xyt(double rx, double ry, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_xzt(double rx, double rz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yzt(double ry, double rz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zyt(double rz, double ry, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zxt(double rz, double rx, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yxt(double ry, double rx, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_xt(double rx, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_yt(double ry, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_zt(double rz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

//now with scaling!

GMREAL dx8_transform_xyzst(double rx, double ry, double rz, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yxzst(double ry, double rx, double rz, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zxyst(double rz, double rx, double ry, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_xzyst(double rx, double rz, double ry, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yzxst(double ry, double rz, double rx, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zyxst(double rz, double ry, double rx, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_xyst(double rx, double ry, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_xzst(double rx, double rz, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yzst(double ry, double rz, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zyst(double rz, double ry, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_zxst(double rz, double rx, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}
GMREAL dx8_transform_yxst(double ry, double rx, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_xst(double rx, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationX(&matrix,(float)rx/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_yst(double ry, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationY(&matrix,(float)ry/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}

GMREAL dx8_transform_zst(double rz, double sx, double sy, double sz, double x, double y, double z) {
    IDirect3DDevice8_GetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixRotationZ(&matrix,(float)rz/-180*M_PI));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixScaling(&matrix,(float)sx,(float)sy,(float)sz));
    D3DXMatrixMultiply(&world_matrix,&world_matrix,D3DXMatrixTranslation(&matrix,(float)x,(float)y,(float)z));    
    IDirect3DDevice8_SetTransform(*d3d8_device,D3DTS_WORLDMATRIX(0),&world_matrix);
    return 0;
}