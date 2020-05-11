#include <string>
#include <memory>

class IShape;
using IShapePtr = std::unique_ptr<IShape>;

class Dot;
class Circle;
class CompoundShape;
class Line;
class ColoredShape;

using Color = std::string;

class Renderer;

class IExporter;
class Exporter;

