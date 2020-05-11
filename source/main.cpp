#include "Document.h"
#include "Shape.h"
#include "MVC.h"

#include <iostream>

class AppManager
{
 public:
  AppManager()
  {
    model_      = std::make_shared<Model>();
    controller_ = std::make_shared<Controller>(model_);
    view_       = std::make_shared<View>(model_, controller_);

    model_->SetView(view_);
  }

  void MainLoop()
  {
    controller_->CreateDocument();
    controller_->AddShape(std::make_unique<Circle>(0, 1, 1));
    controller_->AddShape(std::make_unique<Line>(Dot{0, 1}, Dot{4, 5}));

    controller_->ExportDocument();
  }

 private:
  std::shared_ptr<Model>      model_;
  std::shared_ptr<Controller> controller_;
  std::shared_ptr<View>       view_;
};
int main()
{
  AppManager app_manager;
  app_manager.MainLoop();

  return 0;
}