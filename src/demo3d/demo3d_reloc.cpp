#include "swan/swantypes.h"
#include "swan/gfl/fs/gfl_archive.h"
#include "swan/gfl/core/gfl_heap.h"
#include "swan/data/heapid_def.h"

#include "demo3d_reloc.hpp"

#define DEMO3D_ENGINE_WORK_SIZE ( 152 )

namespace cutscene {
    extern "C" u32 THUMB_BRANCH_Demo3D_CheckIsIDValid(u32 id);
    extern "C" void* THUMB_BRANCH_LINK_Demo3D_InjectSeqLoad(u16 heapID, u32 seqID);
    extern "C" void* THUMB_BRANCH_LINK_Demo3D_InjectRscLoad();
    extern "C" void Demo3D_LoadSeqFile(u32 seqID, u16 heapID);
    extern "C" void* THUMB_BRANCH_Demo3D_GetSeqParamTable(u32 seqID);
    extern "C" void* THUMB_BRANCH_Demo3D_GetCmdSeq(u32 seqID);
    extern "C" void* THUMB_BRANCH_Demo3D_GetExitCmdSeq(u32 seqID);
    extern "C" u32 THUMB_BRANCH_Demo3D_GetRscCount(u32 seqID);

    extern "C" u32 THUMB_BRANCH_Demo3D_CheckIsIDValid(u32 id) {
        return id;
    }

    static Demo3DSeqData* s_loadedSeqData; // This gets freed when the DEMO3D heap is destroyed
    static Demo3DSeqFile* s_loadedDemo3DSeqFile; // This gets freed when the DEMO3D heap is destroyed

    // Modified from Hello007/Plat's code
    extern "C" void* THUMB_BRANCH_LINK_Demo3D_InjectSeqLoad(u16 heapID, u32 seqId) {
        Demo3D_LoadSeqFile(seqId, heapID);
        return GFL_HeapAllocate(heapID, DEMO3D_ENGINE_WORK_SIZE, HEAP_CALLOC_ON, "demo3d_engine.c", 121);
    }

    // Modified from Hello007/Plat's loader code
    extern "C" void Demo3D_LoadSeqFile(u32 seqID, u16 heapID) {
        if (s_loadedSeqData) { // Lazy loading behavior
            return;
        }

        s_loadedDemo3DSeqFile = (Demo3DSeqFile*)GFL_ArcSysReadHeapNewDirect("demo3d_seq_data.narc", seqID, heapID);

        u8* fileOffset = (u8*)s_loadedDemo3DSeqFile;
        Demo3DRelocTbl* relocTable = (Demo3DRelocTbl*)(fileOffset + s_loadedDemo3DSeqFile->relocTableOffset);

        // Updates the offsets from the Demo3D sequence file to be their
        // current addresses in RAM relative to the start of the file in RAM.
        for (int i = 0; i < relocTable->entryCount; i++) {
            *(int*)(fileOffset + relocTable->entries[i]) += (u32)fileOffset;
        }

        s_loadedSeqData = (Demo3DSeqData*)(s_loadedDemo3DSeqFile->seqDataOffset);
    }

    extern "C" void* THUMB_BRANCH_LINK_Demo3D_InjectRscLoad() {
        if (!s_loadedSeqData) {
            return NULL;
        }

        return s_loadedSeqData->resourceTreeOffset;
    }

    extern "C" void* THUMB_BRANCH_Demo3D_GetSeqParamTable(u32 seqID) {
        Demo3D_LoadSeqFile(seqID, HEAPID_DEMO3D);
        return s_loadedSeqData->sceneParamOffset;
    }

    extern "C" void* THUMB_BRANCH_Demo3D_GetCmdSeq(u32 seqID) {
        Demo3D_LoadSeqFile(seqID, HEAPID_DEMO3D);
        return s_loadedSeqData->cmdSeqOffset;
    }

    extern "C" void* THUMB_BRANCH_Demo3D_GetExitCmdSeq(u32 seqID) {
        Demo3D_LoadSeqFile(seqID, HEAPID_DEMO3D);
        return s_loadedSeqData->exitCmdSeqOffset;
    }

    extern "C" u32 THUMB_BRANCH_Demo3D_GetRscCount(u32 seqID) {
        Demo3D_LoadSeqFile(seqID, HEAPID_DEMO3D);
        return s_loadedSeqData->scnRsourceCount;
    }
}