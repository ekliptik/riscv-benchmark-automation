#include "ee_mfcc_f32.h"

const int16_t p_input[FRAME_LEN]
    = { -100, -98,  -108, -119, -136, -122, -127, -113, -103, -77,  -45,  8,
        43,   110,  171,  209,  260,  321,  358,  379,  359,  336,  251,  145,
        6,    -139, -260, -357, -389, -380, -304, -232, -134, -76,  -38,  -30,
        -42,  -63,  -96,  -106, -85,  -50,  -6,   75,   134,  207,  259,  272,
        292,  247,  183,  105,  10,   -78,  -144, -184, -183, -176, -128, -93,
        -65,  -33,  -46,  -64,  -67,  -111, -120, -101, -66,  -8,   54,   145,
        202,  260,  279,  304,  285,  294,  277,  278,  266,  234,  177,  113,
        -8,   -132, -288, -401, -502, -526, -507, -414, -292, -165, -47,  3,
        49,   25,   1,    -38,  -44,  -22,  37,   94,   190,  247,  315,  342,
        314,  271,  171,  80,   -37,  -132, -220, -258, -257, -225, -172, -74,
        5,    93,   168,  190,  171,  152,  94,   58,   51,   60,   101,  147,
        233,  275,  311,  286,  249,  160,  75,   -10,  -46,  -62,  -30,  -8,
        50,   56,   49,   2,    -70,  -154, -240, -283, -270, -187, -75,  65,
        163,  224,  216,  149,  50,   -76,  -160, -194, -179, -125, -18,  52,
        117,  116,  59,   -31,  -128, -231, -286, -294, -249, -148, -10,  170,
        304,  455,  523,  548,  509,  417,  280,  184,  60,   -14,  -29,  -30,
        -7,   -11,  -33,  -95,  -190, -280, -373, -447, -446, -422, -326, -184,
        -17,  182,  363,  502,  611,  623,  616,  526,  442,  334,  250,  190,
        169,  168,  146,  115,  36,   -68,  -218, -369, -478, -552, -549, -476,
        -375, -242, -144, -80,  -59,  -69,  -92,  -104, -99,  -30,  73,   212,
        346,  459,  519,  538,  493,  417,  269,  116,  -33,  -168, -264, -312,
        -346, -355, -380, -421 };

int8_t p_expected[NUM_MFCC_FEATURES]
    = { 87, 101, 96, 97, 99, 99, 100, 101, 100, 99 };
