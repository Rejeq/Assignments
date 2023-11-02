#include "PlotView.h"

#include <QGridLayout>

#include <Log.h>
#include <qcustomplot.h>

void PlotView::SetInitRange(QCustomPlot& plot) {
  plot.xAxis->setRange(0.0, 1.0);
  plot.yAxis->setRange(0.0, 1.0);
}

void PlotView::SetGraphCountEx(QCustomPlot& plot, int count) {
  for (int i = plot.graphCount(); i < count; i++)
    plot.addGraph();
}

void PlotView::InitEx(QCustomPlot& plot, const QString& plotName,
                      const QString& xAxisName, const QString& yAxisName) {
  plot.plotLayout()->insertRow(0);
  plot.plotLayout()->addElement(0, 0, new QCPTextElement(&plot, plotName));

  plot.setInteraction(QCP::iRangeDrag, true);
  plot.setInteraction(QCP::iRangeZoom, true);
  plot.setInteraction(QCP::iSelectAxes, true);
  plot.xAxis->setLabel(xAxisName);
  plot.yAxis->setLabel(yAxisName);
  SetGraphCountEx(plot, 1);
  SetInitRange(plot);

  plot.replot();
}

void PlotView::AppendEx(QCustomPlot& plot, int idx, double time, double value) {
  double minRange = 0.0;
  double maxRange = 0.0;

  plot.graph(idx)->addData(time, value);

  minRange = plot.xAxis->range().lower;
  maxRange = plot.xAxis->range().upper;
  plot.xAxis->setRange(std::min(time, minRange), std::max(time, maxRange));

  minRange = plot.yAxis->range().lower;
  maxRange = plot.yAxis->range().upper;
  plot.yAxis->setRange(std::min(value, minRange), std::max(value, maxRange));

  plot.replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
}

void PlotView::ClearEx(QCustomPlot& plot) {
  for (int i = 0; i < plot.graphCount(); i++)
    plot.graph(i)->setData({}, {});
  SetInitRange(plot);

  plot.replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
}

void PlotView::SetGraphInfoEx(QCustomPlot& plot, int idx, const QString& name,
                              const QPen& pen) {
  plot.graph(idx)->setPen(pen);
  plot.graph(idx)->setName(name);
}

PlotView::PlotView(QWidget* parent) : QWidget(parent) {
  QGridLayout* layout = new QGridLayout(this);

  m_height = new QCustomPlot();
  InitEx(*m_height, "График Высоты", "Время (сек)", "Высота (метры)");

  m_velocity = new QCustomPlot();
  InitEx(*m_velocity, "График Скорости", "Время (сек)", "Скорость (м/с)");

  m_fuellEmission = new QCustomPlot();
  InitEx(*m_fuellEmission, "График расхода топлива", "Время (сек)",
         "Расход топлива (кг/с)");

  m_acceleration = new QCustomPlot();
  InitEx(*m_acceleration, "График ускорения", "Время (сек)", "Ускорение (м/с)");

  layout->addWidget(m_height, 0, 0);
  layout->addWidget(m_velocity, 0, 1);
  layout->addWidget(m_fuellEmission, 1, 0);
  layout->addWidget(m_acceleration, 1, 1);
}

PlotView::~PlotView() {}

void PlotView::SetGraphCount(int count) {
  SetGraphCountEx(*m_height, count);
  SetGraphCountEx(*m_velocity, count);
  SetGraphCountEx(*m_fuellEmission, count);
  SetGraphCountEx(*m_acceleration, count);
}

void PlotView::SetGraphInfo(int idx, const QString& name, const QPen& pen) {
  SetGraphInfoEx(*m_height, idx, name, pen);
  SetGraphInfoEx(*m_velocity, idx, name, pen);
  SetGraphInfoEx(*m_fuellEmission, idx, name, pen);
  SetGraphInfoEx(*m_acceleration, idx, name, pen);
}

void PlotView::Append(int idx, double time, const Phys::RocketBase& rocket) {
  double height = rocket.GetHeight();
  AppendEx(*m_height, idx, time, height);

  double velocity = rocket.GetVelocity();
  AppendEx(*m_velocity, idx, time, velocity);

  double fuellEmission = rocket.GetFuelEmission();
  AppendEx(*m_fuellEmission, idx, time, fuellEmission);

  double acceleration = rocket.GetAcceleration();
  AppendEx(*m_acceleration, idx, time, acceleration);
}

void PlotView::Clear() {
  ClearEx(*m_height);
  ClearEx(*m_velocity);
  ClearEx(*m_fuellEmission);
  ClearEx(*m_acceleration);
}
