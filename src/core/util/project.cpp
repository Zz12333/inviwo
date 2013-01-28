#include <inviwo/core/util/project.h>

namespace inviwo {

const std::string Project::logSource_ = "Project";

Project::Project() {}
Project::~Project() {}

void Project::load(std::string projectName) throw (Exception) {
   /* LogInfo("loading project " + projectName);

    std::fstream fileStream(projectName.c_str(), std::ios_base::in);
    if (fileStream.fail())
        throw SerializationException("Could not open project '" + projectName + "'.");

    XmlDeserializer d(projectName);
    d.setUseAttributes(true);
    NetworkSerializer ser;
    try {
        d.read(fileStream, &ser);
    }
    catch (SerializationException& e) {
        throw SerializationException("Failed to read serialization data stream from workspace file '"
            + projectName + "': " + e.what());
    }
    catch (...) {
        throw SerializationException("Failed to read serialization data stream from workspace file '"
            + projectName + "' (unknown exception).");
    }

    // deserialize workspace from data stream
    try {
        d.deserialize("Workspace", *this);
        //errorList_ = d.getErrors();
        //setFileOrigin(projectName);
    }
    catch (std::exception& e) {
        throw SerializationException("Deserialization from workspace file '" + projectName + "' failed: " + e.what());
    }
    catch (...) {
        throw SerializationException("Deserialization from workspace file '" + projectName + "' failed (unknown exception).");
    }*/
}

ProcessorNetwork* Project::getProcessorNetwork() {
    return processorNetwork_;
}

void Project::serialize(IvwSerializer& /*s*/) const {}
void Project::deserialize(IvwDeserializer& /*s*/) {}

} // namespace
