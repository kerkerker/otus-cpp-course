#pragma once

#include "Types.h"
#include "Document.h"
#include "Shape.h"

#include <memory>

class Controller;
class Model;
class View;

/** Model.
 *
 */
class Model
{
 public:
  void SetView(const std::shared_ptr<View>& view);
  void OpenDocument(DocumentPtr doc);
  void CloseDocument();
  void ExportDocument(IExporter& exporter);
  void AddObject(IShapePtr shape);

 private:
  void Notify() const; // Observer pattern

  DocumentPtr         document_;
  std::weak_ptr<View> view_;
};

/** Controller.
 *
 */
class Controller
{
 public:
  explicit Controller(std::shared_ptr<Model> model);
  void AddShape(IShapePtr shape);
  void RemoveShape(/*coordinates*/);
  void CreateDocument();
  void ImportDocument();
  void ExportDocument();

 private:
  std::shared_ptr<Model> model_;
  Exporter exporter_;
};

/** View.
 *
 */
class View
{
 public:
  View(std::weak_ptr<Model> model, std::weak_ptr<Controller> controller);
  void Draw(const DocumentPtr& doc);

 private:
  std::weak_ptr<Model>      model_;
  std::weak_ptr<Controller> controller_;
  VectorRenderer            renderer_;
};

