#include "SceneView.h"

#include <Eigen/Geometry>

#include <Log.h>

#include <QFile>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QTextStream>
#include <QWheelEvent>

#include "LogGl.h"
#include "RocketBuilder.h"

SceneView::SceneView(QWidget* parent) : QOpenGLWidget(parent) {}

SceneView::~SceneView() {}

void SceneView::UpdateRocketPos(const Ren::Vertex::Pos& pos) {
  m_camera.SetPosition(pos);

  m_rocketTransform = Eigen::Translation3f(pos);
  m_transformChanged = true;
}

void SceneView::Zoom(float delta) {
  m_camera.Zoom(delta);
  update();
}

void SceneView::InitDrawers() {
  QString content;

  content = LoadFileContent(":Moon/Shader/Surface.glsl");
  m_rocket.CreateShaderFromMemory(content.toStdString());
  m_floor.CreateShaderFromMemory(content.toStdString());

  content = LoadFileContent(":Moon/Shader/Stars.glsl");
  m_stars.CreateShaderFromMemory(content.toStdString());
  m_stars.SetVerticesCount(12);

  auto rocketSurface = RocketBuilder::Build({0.0f, 0.0f, 1.0f}, 20.0f, 40.0f);
  m_rocket.AddSurface(rocketSurface.begin(), rocketSurface.end());

  // m_rocketTransform = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
  // m_transformChanged = true;

  // Floor - just a rectangle
  auto floorSurface =
      RocketBuilder::BuildBody({0.0f, -100.0f, 2.0f}, 200.0f, 200.0f);
  floorSurface.SetColor(Ren::RgbColor(246, 241, 213, 255));
  m_floor.AddSurface(floorSurface);
}

void SceneView::initializeGL() {
  LOG_DEBUG("SceneView: Init OpenGL");

  // For some reason Qt destroy opengl context when the widget is rearranged in
  // the dock system.
  // In this case all OpenGL data will be destroyed.
  // And initializeGL will be called again
  //
  // Workaround:
  // https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System/issues/281
  connect(context(), &QOpenGLContext::aboutToBeDestroyed, [&]() {
    LOG_DEBUG("SceneView: Destroing OpenGL context");

    makeCurrent();
    m_rocket.Destroy();
    m_floor.Destroy();
    m_stars.Destroy();
  });

  if (!m_isInited) {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
      LOG_ERROR("SceneView: Unable to init glew: {}",
                (const char*)glewGetErrorString(err));
    }

    m_isInited = true;
  }

  InitGlLogging();
  InitDrawers();

  m_camera.LookAtPlane(Ren::Plane::ZY);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_MULTISAMPLE);
}

void SceneView::paintGL() {
  if (m_camera.Update()) {
    m_cameraChanged = true;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  DrawStars();
  DrawFloor();
  DrawRocket();

  m_cameraChanged = false;
  m_transformChanged = false;
}

void SceneView::resizeGL(int w, int h) {
  LOG_TRACE("SceneView: Resizing to: ({}, {})", w, h);

  m_camera.Resize(w / 100.0f, h / 100.0f);
  glViewport(0, 0, w, h);
}

void SceneView::DrawStars() {
  if (IsCameraChanged()) {
    auto& shader = m_stars.GetShader();

    shader.Use();
    shader.Set("u_height", m_camera.GetPos().y() / 10.0f);
    shader.Set("u_resolution", m_camera.GetSize());
  }

  m_stars.Draw();
}

void SceneView::DrawFloor() {
  if (IsCameraChanged()) {
    auto& shader = m_floor.GetShader();
    shader.Use();
    shader.Set("u_projView", m_camera.GetProjView());
  }

  m_floor.Draw();
}

void SceneView::DrawRocket() {

  if (IsCameraChanged() || IsTransformChanged()) {
    Eigen::Matrix4f pvmMat = m_camera.GetProjView() * m_rocketTransform.matrix();

    auto& shader = m_rocket.GetShader();
    shader.Use();
    shader.Set("u_projView", pvmMat);
  }

  m_rocket.Draw();

  // m_particles.Update();
  // if (IsCameraChanged()) {
  //   auto& shader = m_particles->GetShader();
  //   shader.Set("in_modelView", camera.GetModelView());
  //   shader.Set("in_transform",
  //              Eigen::MakeTransform(ctx->world.rocket.GetPos().y() - 2.0f));
  // }
  //
  // m_particles->Draw();
}

void SceneView::wheelEvent(QWheelEvent* event) {
  float delta = 0.0f;

  QPoint numPixels = event->pixelDelta();
  if (!numPixels.isNull()) {
    delta = numPixels.y();
  } else {
    QPoint numDegrees = event->angleDelta();
    if (numDegrees.isNull()) {
      LOG_ERROR("SceneView: Unable to determine mouse wheel delta");
      return;
    }

    delta = numDegrees.y() / 8 / 15;
  }

  LOG_TRACE("SceneView: Zooming with mouse delta: {}", delta);

  Zoom(delta);
  event->accept();
}

QString SceneView::LoadFileContent(const QString& path) const {
  QFile file = QFile(path);
  file.open(QIODevice::ReadOnly);
  QTextStream stream(&file);
  QString content = stream.readAll();
  file.close();
  return content;
}
