#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QDebug>
#include <QOpenGLFunctions_4_3_Core>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    QSurfaceFormat::setDefaultFormat(format);

    QOpenGLContext context;
    if (!context.create())
        qFatal("Failed to create OpenGL context");

    if(!context.isValid()) return -1;

    // Must have a "surface". Depending on the platform this may be a small
    // window, pbuffer surface, or even nothing (in case surfaceless contexts are
    // supported) under the hood.
    QOffscreenSurface surface;
    surface.create();

    if (!context.makeCurrent(&surface))
        qFatal("makeCurrent failed");

    if(!surface.isValid()) return -2;

    QOpenGLFunctions_4_3_Core f;
    if(!f.initializeOpenGLFunctions()) return -3;

    qDebug() << QString::fromLatin1((const char*)f.glGetString(GL_VERSION));


    // FBO Size will set the final image size. Modify above variable to change final render fbo buffer / image size
    const QSize size(1280, 800);
    QOpenGLFramebufferObject fbo(size, QOpenGLFramebufferObject::CombinedDepthStencil);
    if (fbo.isValid()) {
        if (fbo.format().samples() > 0) {
            // we got a framebuffer backed by a multisample renderbuffer
            qDebug() << "Multisample framebuffer";
        } else {
            // we got a non-multisample framebuffer, backed by a texture
            qDebug() << "No multisample framebuffer";
        }
    }

    // ------------------ Begin RENDER OGL SCENE HERE ------------ /
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glViewport(0, 0, 1280, 800);
    glOrtho(0,100,0,100, 0, 1);

    glColor3f(1.0,0.0,1.0);

    glBegin(GL_LINE_STRIP);
        glVertex2f(10.0f,10.0f);
        glVertex2f(90.0f,10.0f);
        glVertex2f(90.0f,90.0f);
        glVertex2f(90.0f,10.0f);
    glEnd();
    
    // ------------------ End RENDER OGL SCENE HERE ------------ /
    
    // Write it to a file.
    context.makeCurrent(&surface);
    fbo.toImage().save("test.png");


    return 0;
}
