#pragma once

#include <Renderer/GL.h>
#include <QOpenglWidget>

#include <Eigen/Geometry>

#include <Renderer/Camera3d.h>
#include <Renderer/ShaderDrawer.h>
#include <Renderer/SurfaceDrawer.h>

class SceneView : public QOpenGLWidget {
  Q_OBJECT

 public:
  SceneView(QWidget* parent = nullptr);
  ~SceneView();

  void UpdateRocketPos(const Ren::Vertex::Pos& pos);
  void Zoom(float delta);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  void wheelEvent(QWheelEvent* event) override;

 private:
  void InitDrawers();

  void DrawStars();
  void DrawRocket();
  void DrawFloor();

  QString LoadFileContent(const QString& path) const;

  bool IsCameraChanged() const { return m_cameraChanged; }
  bool IsTransformChanged() const { return m_transformChanged; }

  Eigen::Transform<float, 3, Eigen::Affine> m_rocketTransform;

  Ren::OrthoCamera3d m_camera;

  Ren::ShaderDrawer m_stars;
  Ren::SurfaceDrawer m_rocket;
  Ren::SurfaceDrawer m_floor;

  bool m_cameraChanged = false;
  bool m_transformChanged = false;
  bool m_isInited = false;
};
