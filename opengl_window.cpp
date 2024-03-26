#include <QtGui/QSurfaceFormat>

#include "opengl_window.h"
#include "vertex_buffers.h"

OpenGLWindow::OpenGLWindow()
{
    setTitle("OpenGL ES 2.0, Qt6, C++");
    resize(380, 380);

    QSurfaceFormat surfaceFormat;
    surfaceFormat.setDepthBufferSize(24);
    setFormat(surfaceFormat);
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_pHouse;
    delete m_pNiva;
    delete m_pProgram;
}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_pProgram = new QOpenGLShaderProgram();
    m_pProgram->create();
    m_pProgram->addShaderFromSourceFile(QOpenGLShader::ShaderTypeBit::Vertex,
        ":/assets/shaders/texture.vert");
    m_pProgram->addShaderFromSourceFile(QOpenGLShader::ShaderTypeBit::Fragment,
        ":/assets/shaders/texture.frag");
    m_pProgram->link();

    const QVector3D lightPosition(-20.f, 30.f, 35.f);

    const VertexBuffersData houseBuffers =
        VertexBuffers::init(":/assets/models/house/house.dae");
    m_pHouse = new Model(m_pProgram, houseBuffers,
        ":/assets/models/house/house.jpg", lightPosition);
    m_pHouse->position = QVector3D(0.f, 0.f, -3.f);
    m_pHouse->scale = QVector3D(2.f, 2.f, 2.f);

    const VertexBuffersData nivaBuffers =
        VertexBuffers::init(":/assets/models/niva/niva.dae");
    m_pNiva = new Model(m_pProgram, nivaBuffers,
        ":/assets/models/niva/niva.png", lightPosition);
    m_pNiva->position = QVector3D(0.f, 0.f, 2.f);

    m_viewMatrix.lookAt(QVector3D(-3.f, 3.f, 7.f), QVector3D(0.f, 2.f, 0.f),
        QVector3D(0.f, 1.f, 0.f));
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projMatrix.setToIdentity();
    m_projMatrix.perspective(50.f, w / (float)h, 0.1f, 500.f);
    m_projViewMatrix = m_projMatrix * m_viewMatrix;
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.43f, 0.8f, 0.8f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_pHouse->draw(m_projViewMatrix);
    m_pNiva->draw(m_projViewMatrix);
}

void OpenGLWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    m_pHouse->cleanUp();
    m_pNiva->cleanUp();
}
