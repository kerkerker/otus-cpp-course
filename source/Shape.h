#pragma once

#include "Exporter.h"
#include "Types.h"
#include "Renderer.h"

#include <list>
#include <vector>
#include <memory>
#include <sstream>

/**
 * Interface for geometric shapes.
 */
class IShape
{
 public:
  [[nodiscard]] virtual std::unique_ptr<IShape> Clone() const = 0; // pattern Prototype
  virtual void Draw(Renderer& renderer) const = 0;
  virtual void Export(IExporter& exporter) const = 0;
  [[nodiscard]] virtual std::string Str() const = 0;
  virtual ~IShape() = default;
};

/** Publicly inheriting from Shape<T> provides the type T with member functions Clone, Draw, Export.
 *
 * @tparam Derived
 */
template<typename Derived>
class Shape : public IShape
{
 public:
  [[nodiscard]] std::unique_ptr<IShape> Clone() const override // CRTP idiom
  {
    return std::make_unique<Derived>(static_cast<Derived const&>(*this));
  }

  void Draw(Renderer& renderer) const override
  {
    renderer.render(static_cast<Derived const&>(*this));
  }

  void Export(IExporter& exporter) const override
  {
    exporter.Export(static_cast<Derived const&>(*this));
  }

  [[nodiscard]] std::string Str() const override
  {
    return "Shape";
  }
};

/** Adds color property to Shape.
 *
 */
class ColoredShape : public Shape<ColoredShape>
{
 public:
  ColoredShape(Shape& shape, Color color) : shape_(shape), color_(std::move(color))
  {}

  [[nodiscard]] std::string Str() const override // Decorator pattern
  {
    std::ostringstream oss;
    oss << color_ << ' ' << shape_.Str();
    return oss.str();
  }

 private:
  Shape& shape_;
  Color color_;
};

/** Dot.
 *
 */
class Dot : public Shape<Dot>
{
 public:
  Dot(int x, int y) : x_(x), y_(y)
  {}

  [[nodiscard]] std::string Str() const override
  {
    return "Dot";
  }

 private:
  int x_;
  int y_;
};

/** Circle.
 *
 */
class Circle : public Shape<Circle>
{
 public:
  Circle(int x, int y, int radius) : dot_(x, y), radius_(radius)
  {}

  [[nodiscard]] std::string Str() const override
  {
    return "Circle";
  }

 private:
  Dot dot_;
  int radius_;
};

/** Line.
 *
 */
class Line : public Shape<Line>
{
 public:
  Line(Dot x, Dot y) : x_(x), y_(y)
  {}

  [[nodiscard]] std::string Str() const override
  {
    return "Line";
  }

 private:
  Dot x_;
  Dot y_;
};

/** CompoundShape consists of multiple shapes.
 *
 */
class CompoundShape : public Shape<CompoundShape> // Composite pattern
{
 public:
  void Add(std::unique_ptr<IShape> child)
  {
    children_.push_back(std::move(child));
  }

  void Draw(Renderer& renderer) const override
  {
    for (auto&& child: children_) {
      child->Draw(renderer);
    }
  }

  [[nodiscard]] std::string Str() const override
  {
    std::stringstream ss;
    ss << "CompoundShape:";
    for (auto&& child: children_) {
      ss << "\n\t" << child->Str();
    }
    return ss.str();
  }

 private:
  std::list<std::unique_ptr<IShape>> children_;
};
