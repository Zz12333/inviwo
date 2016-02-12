/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2015 Inviwo Foundation
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

#ifndef IVW_CAMERAPROPERTY_H
#define IVW_CAMERAPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/camera.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/interaction/events/eventlistener.h>
#include <inviwo/core/interaction/trackballobject.h>
#include <inviwo/core/properties/eventproperty.h>

namespace inviwo {

template <unsigned int N>
class SpatialEntity;

class Inport;

/**
* \class CameraProperty
*
* Besides giving access to a perspective camera in the GUI
* it also enables linking individual camera properties.
* @see PerspectiveCamera
*/
class IVW_CORE_API CameraProperty : public CompositeProperty, public TrackballObject {
public:
    InviwoPropertyInfo();

    CameraProperty(std::string identifier, std::string displayName,
                   vec3 eye = vec3(0.0f, 0.0f, -2.0f), vec3 center = vec3(0.0f),
                   vec3 lookUp = vec3(0.0f, 1.0f, 0.0f), Inport* inport = nullptr,
                   InvalidationLevel invalidationLevel = InvalidationLevel::InvalidResources,
                   PropertySemantics semantics = PropertySemantics::Default);

    CameraProperty(const CameraProperty& rhs);
    CameraProperty& operator=(const CameraProperty& that);

    // virtual operator Camera&() { return value_; }; // Do not allow user to get
    // non-const reference since no notification mechanism exist.
    virtual operator const Camera&() const;

    virtual CameraProperty* clone() const override;
    virtual ~CameraProperty() = default;

    virtual Camera& get();
    virtual const Camera& get() const;
    virtual void set(const Property* srcProperty) override;

    /**
     * Reset camera position, direction to default state.
     */
    void resetCamera();

    virtual const vec3& getLookFrom() const override;
    virtual void setLookFrom(vec3 lookFrom) override;
    virtual const vec3& getLookTo() const override;
    virtual void setLookTo(vec3 lookTo) override;
    virtual const vec3& getLookUp() const override;
    virtual void setLookUp(vec3 lookUp) override;

    vec3 getLookRight() const;

    void setAspectRatio(float aspectRatio);
    float getAspectRatio() const;

    virtual void setLook(vec3 lookFrom, vec3 lookTo, vec3 lookUp) override;

    virtual float getNearPlaneDist() const override;
    virtual float getFarPlaneDist() const override;

    void setNearPlaneDist(float v);
    void setFarPlaneDist(float v);

    virtual vec3 getLookFromMinValue() const override;
    virtual vec3 getLookFromMaxValue() const override;

    virtual vec3 getLookToMinValue() const override;
    virtual vec3 getLookToMaxValue() const override;

    /**
     * \brief Convert from normalized device coordinates (xyz in [-1 1]) to world coordinates.
     *
     * @param ndcCoords Coordinates in [-1 1]
     * @return World space position
     */
    virtual vec3 getWorldPosFromNormalizedDeviceCoords(const vec3& ndcCoords) const override;

    /**
    * \brief Convert from normalized device coordinates (xyz in [-1 1]) to clip coordinates.
    *
    * @param ndcCoords xyz clip-coordinates in [-1 1]^3, and the clip w-coordinate used for
    *perspective division.
    * @return Clip space position
    */
    vec4 getClipPosFromNormalizedDeviceCoords(const vec3& ndcCoords) const;

    virtual vec3 getNormalizedDeviceFromNormalizedScreenAtFocusPointDepth(
        const vec2& normalizedScreenCoord) const override;

    const mat4& viewMatrix() const;
    const mat4& projectionMatrix() const;
    const mat4& inverseViewMatrix() const;
    const mat4& inverseProjectionMatrix() const;

    void invokeEvent(Event* event) override;

    void setInport(Inport* inport);

    /** 
     * \brief Translates and scales camera to match new data and fit new object into view.
     *
     * @note requires that adjustCameraOnDataChange_ is true
     * and that an inport has been supplied during construction.
     * @param const mat4 & prevDataToWorldMatrix Matrix of previous object
     * @param const mat4 & newDataToWorldMatrix Matrix of new object
     */
    void adjustCameraToData(const mat4& prevDataToWorldMatrix, const mat4& newDataToWorldMatrix);
    /** 
     * \brief Reset the camera adjustment matrix to currently set inport data.
     */
    void resetAdjustCameraToData();
    void inportChanged();

    void dblClick(Event *event);

private:
    void changeCamera(std::unique_ptr<Camera> newCamera);

    void updatePropertyFromValue();
    OptionPropertyString cameraType_;
    // These properties enable linking of individual
    // camera properties but requires them to be synced
    // with the camera

    FloatVec3Property lookFrom_;
    FloatVec3Property lookTo_;
    FloatVec3Property lookUp_;

    FloatProperty aspectRatio_;
    FloatProperty nearPlane_;
    FloatProperty farPlane_;

    EventProperty mouseDblClick_;

    BoolProperty adjustCameraOnDataChange_;

    std::unique_ptr<Camera> camera_;

    Inport* inport_;  ///< Allows the camera to be positioned relative to new data (VolumeInport,
                      ///MeshInport)
    const SpatialEntity<3>* data_;  //< non-owning reference;
    mat4 prevDataToWorldMatrix_; //< Data-to-world matrix of object currently being viewed
};

}  // namespace

#endif  // IVW_CAMERAPROPERTY_H
