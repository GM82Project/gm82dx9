#include "gm82dx8.h"

// Glossary:
//   C: rotation component (either x, y or z)
//   CC: 2 rotation components (different components in any order)
//   CCC: 3 rotation components (different components in any order)
//   S: scaling
//   T: translation

// Define these so we can use lowercase letters for
// D3DXMatrixRotationC (C being X, Y or Z)
#define rotx XMMatrixRotationX
#define roty XMMatrixRotationY
#define rotz XMMatrixRotationZ

// Function Parameter list
#define C_PARM(c) double r ## c
#define CC_PARM(c1, c2) C_PARM(c1), C_PARM(c2)
#define CCC_PARM(c1, c2, c3) CC_PARM(c1, c2), C_PARM(c3)
#define S_PARM double sx, double sy, double sz
#define T_PARM double x, double y, double z

// Matrix multiplication
#define C_MUL(c)                                                    \
    world_matrix = XMMatrixMultiply(world_matrix,                 \
                       rot ## c ((float)r ## c/-180*M_PI))
#define CC_MUL(c1, c2) C_MUL(c1); C_MUL(c2)
#define CCC_MUL(c1, c2, c3) C_MUL(c1); C_MUL(c2); C_MUL(c3)
#define S_MUL                                                                       \
    world_matrix = XMMatrixMultiply(world_matrix,                                 \
                       XMMatrixScaling((float)sx,(float)sy,(float)sz))
#define T_MUL                                                                       \
    world_matrix = XMMatrixMultiply(world_matrix,                                 \
                       XMMatrixTranslation((float)x,(float)y,(float)z))

// Start of function
#define FUNC_START {                                                                \
    (*d3d8_device)->GetTransform(D3DTS_WORLDMATRIX(0),reinterpret_cast<D3DMATRIX*>(&world_matrix))

// End of function
#define FUNC_END                                                                    \
    (*d3d8_device)->SetTransform(D3DTS_WORLDMATRIX(0),reinterpret_cast<D3DMATRIX*>(&world_matrix)); \
    return 0;                                                                       \
}

// Function definition macros
#define CT_FUNC(c)                                      \
    GMREAL dx8_transform_ ## c ## t(C_PARM(c), T_PARM)  \
        FUNC_START; C_MUL(c); T_MUL; FUNC_END
#define CST_FUNC(c)                                             \
    GMREAL dx8_transform_ ## c ## st(C_PARM(c), S_PARM, T_PARM) \
        FUNC_START; C_MUL(c); S_MUL; T_MUL; FUNC_END
#define CCT_FUNC(c1, c2)                                            \
    GMREAL dx8_transform_ ## c1 ## c2 ## t(CC_PARM(c1, c2), T_PARM) \
        FUNC_START; CC_MUL(c1, c2); T_MUL; FUNC_END
#define CCST_FUNC(c1, c2)                                                       \
    GMREAL dx8_transform_ ## c1 ## c2 ## st(CC_PARM(c1, c2), S_PARM, T_PARM)    \
        FUNC_START; CC_MUL(c1, c2); S_MUL; T_MUL; FUNC_END
#define CCCT_FUNC(c1, c2, c3)                                                   \
    GMREAL dx8_transform_ ## c1 ## c2 ## c3 ## t(CCC_PARM(c1, c2, c3), T_PARM)  \
        FUNC_START; CCC_MUL(c1, c2, c3); T_MUL; FUNC_END
#define CCCST_FUNC(c1, c2, c3)                                                          \
    GMREAL dx8_transform_ ## c1 ## c2 ## c3 ## st(CCC_PARM(c1, c2, c3), S_PARM, T_PARM) \
        FUNC_START; CCC_MUL(c1, c2, c3); S_MUL; T_MUL; FUNC_END

// Actual function definitions
CCCT_FUNC(x, y, z)
CCCT_FUNC(y, x, z)
CCCT_FUNC(z, x, y)
CCCT_FUNC(x, z, y)
CCCT_FUNC(y, z, x)
CCCT_FUNC(z, y, x)
CCT_FUNC(x, y)
CCT_FUNC(x, z)
CCT_FUNC(y, z)
CCT_FUNC(z, y)
CCT_FUNC(z, x)
CCT_FUNC(y, x)
CT_FUNC(x)
CT_FUNC(y)
CT_FUNC(z)

// "now with scaling!" ~ renex, 2022
CCCST_FUNC(x, y, z)
CCCST_FUNC(y, x, z)
CCCST_FUNC(z, x, y)
CCCST_FUNC(x, z, y)
CCCST_FUNC(y, z, x)
CCCST_FUNC(z, y, x)
CCST_FUNC(x, y)
CCST_FUNC(x, z)
CCST_FUNC(y, z)
CCST_FUNC(z, y)
CCST_FUNC(z, x)
CCST_FUNC(y, x)
CST_FUNC(x)
CST_FUNC(y)
CST_FUNC(z)
