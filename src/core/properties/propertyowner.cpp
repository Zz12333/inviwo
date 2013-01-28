#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

PropertyOwner::PropertyOwner()
    : invalid_(true) {}

PropertyOwner::~PropertyOwner() {
    properties_.clear();
}

void PropertyOwner::addProperty(Property* property) {
    // TODO: check if property with same name has been added before
    properties_.push_back(property);
    property->setOwner(this);
}

void PropertyOwner::addProperty(Property& property) {
    addProperty(&property);
}

Property* PropertyOwner::getPropertyByIdentifier(std::string identifier) {
    for (size_t i=0; i<properties_.size(); i++)
        if ((properties_[i]->getIdentifier()).compare(identifier) == 0)
            return properties_[i];
    return 0;
}

void PropertyOwner::invalidate() {
    invalid_ = true;
}

void PropertyOwner::setValid() {
    invalid_ = false;
}

bool PropertyOwner::isValid() {
    return (invalid_ == false);
}

void PropertyOwner::serialize(IvwSerializer& s) const {

    std::map<std::string, Property*> propertyMap;
    for (std::vector<Property*>::const_iterator it = properties_.begin(); it != properties_.end(); ++it) {
        propertyMap[(*it)->getIdentifier()] = *it; 
    }
    s.serialize("Properties", propertyMap, "Property" );
}

void PropertyOwner::deserialize(IvwDeserializer& d) {

    /* 1) Vector desrialization does not allow
    *     specification of comparision attribute string.
    *  2) But Map deserialization does allow 
    *     specification of comparision attribute string.
    *     (eg. "identifier" in this case).
    *  3) Hence map deserialization is preffered here.
    *  4) TODO: Vector can be made to behave like Map.
    *           But then it necessitates passing of two extra arguments.
    *           And they are list of attribute values, comparision attribute string.
    *           eg., list of identifier for each property and "identifier"
    *                                                 
    */

    std::map<std::string, Property*> propertyMap;
    for (std::vector<Property*>::const_iterator it = properties_.begin(); it != properties_.end(); ++it) {
        propertyMap[(*it)->getIdentifier()] = *it; 
    }
    d.deserialize("Properties", propertyMap, "Property", "identifier") ;
}


} // namespace
