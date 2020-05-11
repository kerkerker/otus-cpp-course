#include "Exporter.h"
#include "Shape.h"

Exporter::Exporter(std::ostream& os) : os_(os)
{

}

void Exporter::Export(const Dot& dot)
{
  os_ << exporter_name_ << ": " << dot.Str() << std::endl;
}

void Exporter::Export(const Circle& circle)
{
  os_ << exporter_name_ << ": " << circle.Str() << std::endl;
}

void Exporter::Export(const Line& line)
{
  os_ << exporter_name_ << ": " << line.Str() << std::endl;
}

void Exporter::Export(const ColoredShape& colored_shape)
{
  os_ << exporter_name_ << ": " << colored_shape.Str() << std::endl;
}

void Exporter::Export(const CompoundShape& compound_shape)
{
  os_ << exporter_name_ << ": " << compound_shape.Str() << std::endl;
}