#ifndef IVW_PROCESSORNETWORK_H
#define IVW_PROCESSORNETWORK_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/network/portconnection.h>
#include <inviwo/core/network/processorlink.h>
#include <inviwo/core/processors/processor.h>

#include <string>
using namespace std;

namespace inviwo {

class IVW_CORE_API ProcessorNetwork : public IvwSerializable {

public:

    // FIXME: temporary method supporting non serialization-based network creation
    void connectPorts(Port* sourcePort, Port* destPort);
    void disconnectPorts(Port* sourcePort, Port* destPort);
    
    void addLink(Processor* sourceProcessor, Processor* destProcessor);
    void removeLink(Processor* sourceProcessor, Processor* destProcessor);

    ProcessorNetwork();
    virtual ~ProcessorNetwork();

    void addProcessor(Processor* processor);
    void removeProcessor(Processor* processor);

    inline void modified() { isModified_ = true; }
    void setModified(bool isModified) { isModified_ = isModified; }
    bool isModified() { return isModified_; }

    std::vector<Processor*> getProcessors() const { return processors_; }
    Processor* getProcessorByName(std::string name) const;
    template<class T> std::vector<T*> getProcessorsByType() const;

    std::vector<PortConnection*> getPortConnections() const { return portConnections_; }

    std::vector<ProcessorLink*> getProcessorLinks() const { return processorLinks_; }
    ProcessorLink* getProcessorLink(Processor* sourceProcessor, Processor* destProcessor);

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:

    bool isModified_;

    std::vector<Processor*> processors_;
    std::vector<PortConnection*> portConnections_;
    std::vector<ProcessorLink*> processorLinks_;

};

template<class T>
std::vector<T*> ProcessorNetwork::getProcessorsByType() const {
    std::vector<T*> processors;
    for (size_t i=0; i<processors_.size(); i++) {
        T* processor = dynamic_cast<T*>(processors_[i]);
        if (processor) processors.push_back(processor);
    }
    return processors;
}

} // namespace

#endif // IVW_PROCESSORNETWORK_H
