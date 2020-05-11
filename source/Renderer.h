#pragma once

#include "Types.h"

#include <sstream>
#include <string>

/** Interface for renderers.
 *
 */
class Renderer
{
 public:
  virtual void render(const Dot&) = 0;
  virtual void render(const Circle&) = 0;
  virtual void render(const CompoundShape&) = 0;
  virtual void render(const Line&) = 0;
  virtual void render(const ColoredShape&) = 0;

  virtual ~Renderer() = default;
};

/** Implements vector rendering.
 *
 */
class VectorRenderer : public Renderer
{
 public:
  explicit VectorRenderer(std::ostream& os);
  void render(const Dot& dot) override;
  void render(const Circle& circle) override;
  void render(const CompoundShape& compound_shape) override;
  void render(const Line& line) override;
  void render(const ColoredShape& colored_shape) override;

 private:
  std::ostream& os_;
  std::string renderer_name_ = "VectorRenderer";
};