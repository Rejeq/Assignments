#pragma once

#include <Renderer/Surface.h>

#include <QVector>

class RocketBuilder {
 public:
  using Pos = Ren::Vertex::Pos;

  static QVector<Ren::Surface> Build(const Pos& pos, float w, float h);
  static Ren::Surface BuildBody(const Pos& pos, float w, float h);
  static Ren::Surface BuildHead(const Pos& pos, float w, float h);
  static Ren::Surface BuildTail(const Pos& pos, float topW, float baseW,
                                float h);
  static Ren::Surface BuildWing(const Pos& pos, float w, float h,
                                float leftOffset);
  static Ren::Surface BuildWindow(const Pos& pos, float r,
                                  const Ren::Color& color);
};
