#pragma once

#include "Types.h"

#include <list>

/** Document.
 *
 */
class Document
{
 public:
  class Iterator
  {
   public:
    Iterator(std::list<IShapePtr>::const_iterator iterator);
    const IShapePtr& operator*() const;
    Iterator& operator++();
    Iterator& operator--();
    bool operator!=(Iterator other);

   private:
    std::list<IShapePtr>::const_iterator iterator_;
  };

  [[nodiscard]] Iterator begin() const;
  [[nodiscard]] Iterator end() const;

  void Export(IExporter& exporter) const;
  void Import() const;
  void AddShape(IShapePtr shape);

 private:
  std::list<IShapePtr> shapes_;
};

using DocumentPtr = std::unique_ptr<Document>;