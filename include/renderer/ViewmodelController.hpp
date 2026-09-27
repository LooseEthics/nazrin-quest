
#pragma once

#include <vector>

#include "ViewmodelRenderCall.hpp"

class ViewmodelController
{
public:
  ViewmodelController();

  const std::vector<ViewmodelRenderCall*> callVector() const;

private:
  std::vector<ViewmodelRenderCall*> callVector_;

  ViewmodelRenderCall mantleCall_;
};
