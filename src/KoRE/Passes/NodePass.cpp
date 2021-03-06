/*
  Copyright (c) 2012 The KoRE Project

  This file is part of KoRE.

  KoRE is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KoRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KoRE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "KoRE/Passes/NodePass.h"

kore::NodePass::NodePass(void)
  : _node(NULL),
    _executionType(EXECUTE_REPEATING),
    _executed(false) {
}

kore::NodePass::NodePass(const SceneNode* node)
  : _node(node),
    _executionType(EXECUTE_REPEATING),
    _executed(false) {
}

kore::NodePass::~NodePass(void) {
  for (uint i = 0; i < _operations.size(); ++i) {
    KORE_SAFE_DELETE(_operations[i]);
  }
}

void kore::NodePass::addOperation(Operation* op) {
  if (std::find(_operations.begin(), _operations.end(), op)
      != _operations.end()) {
        return;
  }
  if(_operations.size() > 0 &&
     _operations[(_operations.size()-1)]->getType() == OP_RENDERMESH) {
    _operations.insert((_operations.end()--), op);
  } else {
    _operations.push_back(op);
  }
}

void kore::NodePass::removeOperation(Operation* op) {
  auto it = std::find(_operations.begin(), _operations.end(), op);
  if (it != _operations.end()) {
    _operations.erase(it);
  }
}

void kore::NodePass::addStartupOperation(Operation* op) {
  if (std::find(_startupOperations.begin(), _startupOperations.end(), op)
    != _startupOperations.end()) {
      return;
  }
  _startupOperations.push_back(op);
}

void kore::NodePass::removeStartupOperation(Operation* op) {
  auto it = std::find(_startupOperations.begin(), _startupOperations.end(), op);
  if(it != _startupOperations.end()) {
    _startupOperations.erase(it);
  }
}

void kore::NodePass::addFinishOperation(Operation* op) {
  if (std::find(_finishOperations.begin(), _finishOperations.end(), op)
    != _finishOperations.end()) {
      return;
  }
  _finishOperations.push_back(op);
}

void kore::NodePass::removeFinishOperation(Operation* op) {
  auto it = std::find(_finishOperations.begin(), _finishOperations.end(), op);
  if(it != _finishOperations.end()) {
    _finishOperations.erase(it);
  }
}
