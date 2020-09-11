#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QOpenGLFunctions_3_3_Core>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);

    QOpenGLContext context;
    if (!context.create())
        qFatal("Failed to create OpenGL context");

    // Must have a "surface". Depending on the platform this may be a small
    // window, pbuffer surface, or even nothing (in case surfaceless contexts are
    // supported) under the hood.
    QOffscreenSurface surface;
    surface.create();

    if (!context.makeCurrent(&surface))
        qFatal("makeCurrent failed");

    // FBO Size will set the final image size. Modify above variable to change final render fbo buffer / image size
    const QSize size(1280, 800);
    QOpenGLFramebufferObject fbo(size, QOpenGLFramebufferObject::CombinedDepthStencil);

    // ------------------ Begin RENDER OGL SCENE HERE ------------ /
    glClear(GL_COLOR_BUFFER_BIT
          | GL_DEPTH_BUFFER_BIT
          | GL_STENCIL_BUFFER_BIT);

    glViewport(0,0,100,100);
    glOrtho(0,100,0,100,0,1);

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


    fbo.toImage().save("/Users/pabloelias/Documents/Development/TestOpenGLQt/test.png");


    return 0;
}
