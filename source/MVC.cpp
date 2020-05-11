#include "MVC.h"

#include <iostream>

// Model
void Model::SetView(const std::shared_ptr<View>& view)
{
  view_ = view;
}

void Model::OpenDocument(DocumentPtr doc)
{
  document_ = std::move(doc);
}

void Model::CloseDocument()
{
  document_.reset();
}

void Model::ExportDocument(IExporter& exporter)
{
  document_->Export(exporter);
}

void Model::AddObject(IShapePtr shape)
{
  if (document_) {
    document_->AddShape(std::move(shape));
    Notify();
  }
}

void Model::Notify() const
{
  if (auto v = view_.lock(); v) {
    v->Draw(document_);
  }
}

// Controller
Controller::Controller(std::shared_ptr<Model> model) : model_(std::move(model)), exporter_(std::cout)
{}

void Controller::AddShape(IShapePtr shape)
{
  model_->AddObject(std::move(shape));
}

void Controller::RemoveShape(/*coordinates*/)
{

}

void Controller::CreateDocument()
{
  model_->OpenDocument(std::make_unique<Document>());
}

void Controller::ImportDocument()
{

}
void Controller::ExportDocument()
{
  model_->ExportDocument(exporter_);
}

// View

View::View(std::weak_ptr<Model> model, std::weak_ptr<Controller> controller)
    : model_(std::move(model)), controller_(std::move(controller)), renderer_(std::cout)
{
}
void View::Draw(const DocumentPtr& doc)
{
  std::cout << "View::Draw" << std::endl;
  if (doc) {
    for (auto& shape: *doc) {
      shape->Draw(renderer_);
    }
  }
}