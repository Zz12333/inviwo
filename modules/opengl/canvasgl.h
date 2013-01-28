#ifndef IVW_CANVASGL_H
#define IVW_CANVASGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include <inviwo/core/util/canvas.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasGL : public Canvas {
public:
    CanvasGL(ivec2 dimensions);

    virtual void initialize();
    virtual void deinitialize();
    virtual void switchContext();
    virtual void repaint();
    virtual void resize(ivec2 size);
    virtual void update();

};

} // namespace

#endif // IVW_CANVASGL_H
