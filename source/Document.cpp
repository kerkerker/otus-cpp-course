#include "Document.h"
#include "Shape.h"

void Document::AddShape(IShapePtr shape)
{
  shapes_.push_back(std::move(shape));
}

Document::Iterator Document::begin() const
{
  return {shapes_.begin()};
}

Document::Iterator Document::end() const
{
  return {shapes_.end()};
}

void Document::Export(IExporter& exporter) const
{
  for (const auto& shape: *this) {
    shape->Export(exporter);
  }
}

void Document::Import() const
{

}

Document::Iterator::Iterator(std::list<IShapePtr>::const_iterator iterator) : iterator_(iterator)
{

}

const IShapePtr& Document::Iterator::operator*() const
{
  return *iterator_;
}

Document::Iterator& Document::Iterator::operator++()
{
  iterator_++;
  return *this;
}

Document::Iterator& Document::Iterator::operator--()
{
  iterator_--;
  return *this;
}

bool Document::Iterator::operator!=(Document::Iterator other)
{
  return iterator_ != other.iterator_;
}
