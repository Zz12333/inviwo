/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2017-2019 Inviwo Foundation
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
 *********************************************************************************/

#ifndef IVW_INPUTSELECTOR_H
#define IVW_INPUTSELECTOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/processortraits.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/datastructures/datatraits.h>

namespace inviwo {

/** \docpage{org.inviwo.InputSelector, Input Selector}
 * ![](org.inviwo.InputSelector.png?classIdentifier=org.inviwo.InputSelector)
 * Allows to select one particular input from all given ones.
 *
 * ### Inports
 *   * __inport__  all available inputs
 *
 * ### Outports
 *   * __outport__ selected input
 *
 * ### Properties
 *   * __selected inport__  name of selected port
 */

/**
 * \class InputSelector
 * \brief processor for selecting one of n connected inputs
 */
template <typename Inport, typename Outport>
class InputSelector : public Processor {
public:
    InputSelector();
    virtual ~InputSelector() = default;

    virtual const ProcessorInfo getProcessorInfo() const override;

    virtual void process() override;

private:
    void portSettings();

    Inport inport_;
    Outport outport_;

    OptionPropertyInt selectedPort_;

    void updateOptions();
    bool updatedNedded_ = true;
};

template <typename Inport, typename Outport>
const ProcessorInfo InputSelector<Inport, Outport>::getProcessorInfo() const {
    return ProcessorTraits<InputSelector<Inport, Outport>>::getProcessorInfo();
}

template <typename Inport, typename Outport>
InputSelector<Inport, Outport>::InputSelector()
    : Processor()
    , inport_("inport")
    , outport_("outport")
    , selectedPort_("selectedPort", "Select Inport") {
    portSettings();

    addPort(inport_);
    addPort(outport_);

    inport_.onConnect([&]() { updatedNedded_ = true; });

    inport_.onChange([&]() {
        if (selectedPort_.size() != inport_.getConnectedOutports().size()) updatedNedded_ = true;
    });

    addProperty(selectedPort_);

    selectedPort_.setSerializationMode(PropertySerializationMode::All);

    setAllPropertiesCurrentStateAsDefault();
}

template <typename Inport, typename Outport>
void InputSelector<Inport, Outport>::updateOptions() {
    std::vector<OptionPropertyIntOption> options;

    for (auto port : inport_.getConnectedOutports()) {
        auto id = port->getProcessor()->getIdentifier();
        auto dispName = port->getProcessor()->getDisplayName();
        options.emplace_back(id, dispName, static_cast<int>(options.size()));
    }
    selectedPort_.replaceOptions(options);
    selectedPort_.setCurrentStateAsDefault();
    updatedNedded_ = false;
}

template <typename Inport, typename Outport>
void InputSelector<Inport, Outport>::process() {
    if (updatedNedded_) updateOptions();
    outport_.setData(inport_.getVectorData().at(selectedPort_.get()));
}

template <>
void InputSelector<ImageMultiInport, ImageOutport>::portSettings();

template <typename Inport, typename Outport>
void InputSelector<Inport, Outport>::portSettings() {}

template <typename Inport, typename Outport>
struct ProcessorTraits<InputSelector<Inport, Outport>> {
    static ProcessorInfo getProcessorInfo() {
        using DataType = typename Inport::type;
        return {
            DataTraits<DataType>::classIdentifier() + ".InputSelector",  // Class identifier
            DataTraits<DataType>::dataName() + " Input Selector",        // Display name
            "Data Selector",                                             // Category
            CodeState::Stable,                                           // Code state
            Tags::CPU,                                                   // Tags
        };
    }
};

}  // namespace inviwo

#endif  // IVW_INPUTSELECTOR_H
