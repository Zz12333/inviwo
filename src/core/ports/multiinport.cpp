/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Daniel J�nsson, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/ports/multiinport.h>

namespace inviwo {

MultiInport::MultiInport(std::string identifier)
    : Inport(identifier)
    , inports_(new InportVec())
    , vectorInports_(new InportVec()) {
}

void MultiInport::setProcessorHelper(Port* port, Processor* processor) {
    port->setProcessor(processor);
}

MultiInport::~MultiInport() {
    InportVec::iterator it = inports_->begin();
    InportVec::iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        delete *it;

    delete inports_;
    inports_ = NULL;

    it = vectorInports_->begin();
    endIt = vectorInports_->end();

    for (; it != endIt; ++it)
        delete *it;

    delete vectorInports_;
    vectorInports_ = NULL;
}

PropertyOwner::InvalidationLevel MultiInport::getInvalidationLevel() const {
    InportVec::const_iterator it = inports_->begin();
    InportVec::const_iterator endIt = inports_->end();
    PropertyOwner::InvalidationLevel maxInvalidationLevel(PropertyOwner::VALID);

    for (; it != endIt; ++it)
        maxInvalidationLevel = std::max(maxInvalidationLevel, (*it)->getInvalidationLevel());

    it = vectorInports_->begin();
    endIt = vectorInports_->end();
    for (; it != endIt; ++it)
        maxInvalidationLevel = std::max(maxInvalidationLevel, (*it)->getInvalidationLevel());

    return maxInvalidationLevel;
}

void MultiInport::setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {
    InportVec::iterator it = inports_->begin();
    InportVec::iterator endIt = inports_->end();
    for (; it != endIt; ++it)
        (*it)->setInvalidationLevel(invalidationLevel);

    it = vectorInports_->begin();
    endIt = vectorInports_->end();
    for (; it != endIt; ++it)
        (*it)->setInvalidationLevel(invalidationLevel);

    setChanged();
}


void MultiInport::setChanged(bool changed /*= true*/) {
    InportVec::const_iterator it = inports_->begin();
    InportVec::const_iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        (*it)->setChanged(changed);
}

bool MultiInport::isChanged() {
    InportVec::const_iterator it = inports_->begin();
    InportVec::const_iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        if((*it)->isChanged())
            return true;

    it = vectorInports_->begin();
    endIt = vectorInports_->end();
    for (; it != endIt; ++it)
        if((*it)->isChanged())
            return true;

    return false;
}

bool MultiInport::isConnectedTo(Outport* outport) const {
    InportVec::const_iterator it = inports_->begin();
    InportVec::const_iterator endIt = inports_->end();

    for (; it != endIt; ++it) {
        if ((*it)->isConnectedTo(outport))
            return true;
    }
    it = vectorInports_->begin();
    endIt = vectorInports_->end();
    for (; it != endIt; ++it) {
        if ((*it)->isConnectedTo(outport))
            return true;
    }

    return false;
}




std::vector<Outport*> MultiInport::getConnectedOutports() const {
    InportVec::const_iterator it = inports_->begin();
    InportVec::const_iterator endIt = inports_->end();
    std::vector<Outport*> connectedOutports;

    for (; it != endIt; ++it) {
        std::vector<Outport*> inportConnectedOutports = (*it)->getConnectedOutports();
        std::vector<Outport*>::const_iterator outportIt = inportConnectedOutports.begin();
        std::vector<Outport*>::const_iterator outportEndIt = inportConnectedOutports.end();

        for (; outportIt != outportEndIt; ++outportIt)
            connectedOutports.push_back(*outportIt);
    }

    it = vectorInports_->begin();
    endIt = vectorInports_->end();
    for (; it != endIt; ++it) {
        std::vector<Outport*> inportConnectedOutports = (*it)->getConnectedOutports();
        std::vector<Outport*>::const_iterator outportIt = inportConnectedOutports.begin();
        std::vector<Outport*>::const_iterator outportEndIt = inportConnectedOutports.end();

        for (; outportIt != outportEndIt; ++outportIt)
            connectedOutports.push_back(*outportIt);
    }


    return connectedOutports;
}

size_t MultiInport::getNumConnectedOutports() const {
    return getConnectedOutports().size();
}

void MultiInport::disconnectFrom(Outport* outport) {
    InportVec::iterator it;

    for (it = inports_->begin(); it != inports_->end(); ++it) {
        // Find connected port
        if ((*it)->isConnectedTo(outport)) {
            Inport* inport = *it;
            inports_->erase(it);
            inport->disconnectFrom(outport);
            delete inport;
            break;
        }
    }
    for (it = vectorInports_->begin(); it != vectorInports_->end(); ++it) {
        // Find connected port
        if ((*it)->isConnectedTo(outport)) {
            Inport* inport = *it;
            vectorInports_->erase(it);
            inport->disconnectFrom(outport);
            delete inport;
            break;
        }
    }
}

Outport* MultiInport::getConnectedOutport() const {
    if (isConnected()) {
        if (inports_->size() > 0) {
            return (*(inports_->begin()))->getConnectedOutport();
        } else {
            return (*(vectorInports_->begin()))->getConnectedOutport();
        }
    } else {
        return NULL;
    }
}

void MultiInport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    InportVec::iterator it = inports_->begin();
    InportVec::iterator endIt = inports_->end();

    for (; it != endIt; ++it)
        (*it)->invalidate(invalidationLevel);

    it = vectorInports_->begin();
    endIt = vectorInports_->end();
    for (; it != endIt; ++it)
        (*it)->invalidate(invalidationLevel);
}



} // namespace
