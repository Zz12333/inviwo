#ifndef IVW_CAMERAPROPERTYWIDGETQT_H
#define IVW_CAMERAPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API CameraPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    CameraPropertyWidgetQt(CameraProperty* property);
    void updateFromProperty();

public slots:
    void setPropertyValue();

private:
    CameraProperty* property_;
    QSlider* slider_;

    void generateWidget();
};

} // namespace

#endif // IVW_CAMERAPROPERTYWIDGETQT_H