#include "ExportAPI.h"

Companion* CreateCompanion() {
    return new Companion();
}

void DisposeCompanion(Companion* pObject) {
    if (pObject != NULL) {
        delete pObject;
        pObject = NULL;
    }
}

void CallRun(Companion* pObject, StreamWorker &worker) {
    if (pObject != NULL) {
        pObject->run(worker);
    }
}

void CallStop(Companion* pObject) {
    if (pObject != NULL) {
        pObject->stop();
    }
}
