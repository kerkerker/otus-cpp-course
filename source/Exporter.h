#pragma once

#include "Types.h"

#include <sstream>
#include <string>

/** Interface for exporter.
 *
 */
class IExporter // Visitor pattern
{
 public:
  virtual void Export(const Dot& dot) = 0;
  virtual void Export(const Circle& circle) = 0;
  virtual void Export(const Line& circle) = 0;
  virtual void Export(const ColoredShape& circle) = 0;
  virtual void Export(const CompoundShape& compound_shape) = 0;

  virtual ~IExporter() = default;
};

/** Implements basic exporter.
 *
 */
class Exporter : public IExporter
{
 public:
  explicit Exporter(std::ostream& os);
  void Export(const Dot& dot) override;
  void Export(const Circle& circle) override;
  void Export(const Line& circle) override;
  void Export(const ColoredShape& circle) override;
  void Export(const CompoundShape& compound_shape) override;

 private:
  std::string exporter_name_ = "Basic Exporter";
  std::ostream& os_;
};

