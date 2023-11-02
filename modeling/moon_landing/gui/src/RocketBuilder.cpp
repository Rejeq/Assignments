#include "RocketBuilder.h"

#include <vector>

QVector<Ren::Surface> RocketBuilder::Build(const Pos& pos, float w, float h) {
  QVector<Ren::Surface> rocket;

  float bodyW = w;
  float bodyH = h;
  const Pos& bodyPos = pos;

  rocket.append(BuildBody(bodyPos, bodyW, bodyH));

  float headW = bodyW;
  float headH = bodyH / 4.0f;
  Pos headPos = bodyPos + Pos(0.0f, (bodyH + headH) / 2.0f, 0.0f);

  rocket.append(BuildHead(headPos, headW, headH));

  float tailH = bodyH / 4.0f;
  float tailBaseW = bodyW - 0.2f;
  float tailTopW = tailBaseW / 3.0f;
  Pos tailPos = bodyPos - Pos(0.0f, (bodyH + tailH) / 2.0f, 0.0f);

  rocket.append(BuildTail(tailPos, tailTopW, tailBaseW, tailH));

  float wingW = bodyW / 4.0f;
  float wingLeftH = bodyH / 15.0f;
  float wingRightH = bodyH / 3.0f;

  Pos wingLeftPos = bodyPos + Pos(-(bodyW + wingW) / 2.0f, -bodyH / 4.0f, 0.0f);
  Pos wingRightPos = bodyPos + Pos((bodyW + wingW) / 2.0f, -bodyH / 4.0f, 0.0f);

  rocket.append(BuildWing(wingLeftPos, wingW, wingLeftH, wingRightH));
  rocket.append(BuildWing(wingRightPos, -wingW, wingLeftH, wingRightH));

  float windowLargeR = bodyW / 2.0f / 1.5f;
  float windowSmallR = bodyW / 2.0f / 2.0f;
  Pos windowPos = bodyPos + Pos(0.0f, windowLargeR, 0.0f);

  Ren::Color windowLargeCol = Ren::RgbColor(50, 150, 220, 255);
  Ren::Color windowSmallCol = Ren::RgbColor(45, 65, 85, 255);

  rocket.append(BuildWindow(windowPos, windowLargeR, windowLargeCol));
  rocket.append(BuildWindow(windowPos, windowSmallR, windowSmallCol));

  return rocket;
}

Ren::Surface RocketBuilder::BuildBody(const Pos& pos, float w, float h) {
  Ren::Surface body;
  body.SetColor(Ren::RgbColor(190, 195, 200, 255));

  Pos topLeft = pos + Pos(-w / 2.0f, h / 2.0f, 0.0f);
  Pos topRight = topLeft + Pos(w, 0.0f, 0.0f);
  Pos bottomLeft = topLeft + Pos(0.0f, -h, 0.0f);
  Pos bottomRight = topLeft + Pos(w, -h, 0.0f);

  body.SetVertices({
      topLeft,
      topRight,
      bottomRight,
      bottomLeft,
  });

  // clang-format off
  body.SetIndices({
      0, 1, 2,
      2, 3, 0,
  });
  // clang-format on

  return body;
}

Ren::Surface RocketBuilder::BuildHead(const Pos& pos, float w, float h) {
  Ren::Surface head;
  head.SetColor(Ren::RgbColor(200, 50, 50, 255));

  Pos baseLeft = pos + Pos(-w / 2.0f, -h / 2.0f, 0.0f);
  Pos baseRight = baseLeft + Pos(w, 0.0f, 0.0f);
  Pos apex = pos + Pos(0.0f, h / 2.0f, 0.0f);

  head.SetVertices({
      baseLeft,
      baseRight,
      apex,
  });

  // clang-format off
  head.SetIndices({
      0, 1, 2,
  });
  // clang-format on

  return head;
}

Ren::Surface RocketBuilder::BuildTail(const Pos& pos, float topW, float baseW,
                                      float h) {
  Ren::Surface tail;
  tail.SetColor(Ren::RgbColor(46, 66, 86, 255));

  Pos topLeft = pos + Pos(-topW / 2.0f, h / 2.0f, 0.0f);
  Pos topRight = topLeft + Pos(topW, 0.0f, 0.0f);

  Pos baseLeft = pos + Pos(-baseW / 2.0f, -h / 2.0f, 0.0f);
  Pos baseRight = baseLeft + Pos(baseW, 0.0f, 0.0f);

  tail.SetVertices({
      topLeft,
      topRight,
      baseRight,
      baseLeft,
  });

  // clang-format off
  tail.SetIndices({
      0, 1, 2,
      2, 3, 0,
  });
  // clang-format on

  return tail;
}

Ren::Surface RocketBuilder::BuildWing(const Pos& pos, float w, float leftH,
                                      float rightH) {
  Ren::Surface wing;
  wing.SetColor(Ren::RgbColor(200, 50, 50, 255));

  Pos leftTop = pos + Pos(-w / 2.0f, leftH / 2.0f, 0.0f);
  Pos leftBottom = pos + Pos(-w / 2.0f, -rightH / 2.0f, 0.0f);

  Pos rightTop = pos + Pos(w / 2.0f, rightH / 2.0f, 0.0f);
  Pos rightBottom = rightTop + Pos(0.0f, -rightH, 0.0f);

  wing.SetVertices({
      leftTop,
      rightTop,
      rightBottom,
      leftBottom,
  });

  // clang-format off
  wing.SetIndices({
      0, 1, 2,
      2, 3, 0,
  });
  // clang-format on

  return wing;
}

Ren::Surface RocketBuilder::BuildWindow(const Pos& pos, float r,
                                        const Ren::Color& color) {
  constexpr int TotalPoints = 128;
  constexpr int TotalTriangles = TotalPoints - 2;
  constexpr float Angle = (2 * M_PI) / TotalPoints;

  std::vector<Ren::Vertex> vertices;
  std::vector<Ren::Ind> indices;

  vertices.reserve(TotalPoints);
  for (int i = 0; i < TotalPoints; i++) {
    float currAngle = Angle * i;
    float x = r * std::cos(currAngle);
    float y = r * std::sin(currAngle);
    float z = 0.0f;

    vertices.emplace_back(Pos(x, y, z) + pos);
  }

  indices.reserve(TotalTriangles * 3);
  for (int i = 0; i < TotalTriangles; i++) {
    indices.emplace_back(0);
    indices.emplace_back(i + 1);
    indices.emplace_back(i + 2);
  }

  Ren::Surface window;
  window.SetColor(color);
  window.SetVertices(std::move(vertices));
  window.SetIndices(std::move(indices));

  return window;
}
