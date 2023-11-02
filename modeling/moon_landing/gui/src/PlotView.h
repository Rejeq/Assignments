#pragma once

#include <QWidget>

#include <Physics/Rocket.h>

class QCustomPlot;
class QPaintEvent;

class PlotView : public QWidget {
  Q_OBJECT

 public:
  PlotView(QWidget* parent = nullptr);
  ~PlotView();

  void SetGraphCount(int count);
  void SetGraphInfo(int idx, const QString& name, const QPen& pen);

  // Time in seconds
  void Append(int idx, double time, const Phys::RocketBase& rocket);

  void Clear();

 private:
  void InitEx(QCustomPlot& plot, const QString& plotName,
              const QString& xAxisName, const QString& yAxisName);
  void AppendEx(QCustomPlot& plot, int idx, double time, double value);
  void ClearEx(QCustomPlot& plot);
  void SetInitRange(QCustomPlot& plot);
  void SetGraphCountEx(QCustomPlot& plot, int count);
  void SetGraphInfoEx(QCustomPlot& plot, int idx, const QString& name,
                      const QPen& pen);

  QCustomPlot* m_height = nullptr;
  QCustomPlot* m_velocity = nullptr;
  QCustomPlot* m_fuellEmission = nullptr;
  QCustomPlot* m_acceleration = nullptr;
};
