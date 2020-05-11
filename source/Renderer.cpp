#include "Renderer.h"
#include "Shape.h"

VectorRenderer::VectorRenderer(std::ostream& os) : os_(os)
{}

void VectorRenderer::render(const Dot& dot)
{
  os_ << renderer_name_ << ": " << dot.Str() << std::endl;
}
void VectorRenderer::render(const Circle& circle)
{
  os_ << renderer_name_ << ": " << circle.Str() << std::endl;
}
void VectorRenderer::render(const CompoundShape& compound_shape)
{
  os_ << renderer_name_ << ": " << compound_shape.Str() << std::endl;
}
void VectorRenderer::render(const Line& line)
{
  os_ << renderer_name_ << ": " << line.Str() << std::endl;
}
void VectorRenderer::render(const ColoredShape& colored_shape)
{
  os_ << renderer_name_ << ": " << colored_shape.Str() << std::endl;
}
