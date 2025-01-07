#pragma once

#include "swan/swantypes.h"

namespace cutscene {
    typedef struct {
        void* sceneParamOffset;
        void* cmdSeqOffset;
        void* exitCmdSeqOffset;
        void* resourceTreeOffset;
        u32   scnRsourceCount;
    } Demo3DSeqData;

    typedef struct {
        /* 0x00 */ u8 magic[8];
        /* 0x08 */ u32 relocTableOffset;
        /* 0x0C */ u32 seqDataOffset;
        /* 0x10 */ u32 pad0;
    } Demo3DSeqFile;

    typedef struct {
        /* 0x00 */ u8 magic[8];
        /* 0x08 */ u32 pad0;
        /* 0x0C */ u32 entryCount;
        /* 0x10 */ u32 entries[];
    } Demo3DRelocTbl;
}