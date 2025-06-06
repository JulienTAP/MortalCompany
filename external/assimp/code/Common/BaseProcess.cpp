/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2025, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file Implementation of BaseProcess */

#include "BaseProcess.h"
#include "Importer.h"
#include <assimp/BaseImporter.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>

using namespace Assimp;

// ------------------------------------------------------------------------------------------------
// Constructor to be privately used by Importer
BaseProcess::BaseProcess() AI_NO_EXCEPT
        : shared(),
          progress() {
    // empty
}

// ------------------------------------------------------------------------------------------------
void BaseProcess::ExecuteOnScene(Importer *pImp) {
    ai_assert( nullptr != pImp );
    if (pImp == nullptr) {
        return;
    }

    ai_assert(nullptr != pImp->Pimpl()->mScene);
    if (pImp->Pimpl()->mScene == nullptr) {
        return;
    }

    progress = pImp->GetProgressHandler();
    ai_assert(nullptr != progress);
    if (progress == nullptr) {
        return;
    }

    SetupProperties(pImp);

    // catch exceptions thrown inside the PostProcess-Step
    try {
        Execute(pImp->Pimpl()->mScene);
    } catch (const std::exception &err) {

        // extract error description
        pImp->Pimpl()->mErrorString = err.what();
        ASSIMP_LOG_ERROR(pImp->Pimpl()->mErrorString);

        // and kill the partially imported data
        delete pImp->Pimpl()->mScene;
        pImp->Pimpl()->mScene = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
void BaseProcess::SetupProperties(const Importer * /*pImp*/) {
    // the default implementation does nothing
}

// ------------------------------------------------------------------------------------------------
bool BaseProcess::RequireVerboseFormat() const {
    return true;
}
