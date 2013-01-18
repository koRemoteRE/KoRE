/*
  Copyright � 2012 The KoRE Project

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

#include "core/rendermeshop.h"
#include <vector>

kore::RenderMeshOp::RenderMeshOp(void)
  : _mesh(NULL),
    _camera(NULL),
    _shader(NULL),
    kore::Operation() {
}

kore::RenderMeshOp::RenderMeshOp(const kore::MeshPtr& mesh,
                                 const kore::CameraPtr& camera,
                                 const kore::ShaderPtr& shader)
                                 : _mesh(mesh),
                                 _camera(camera),
                                 _shader(shader),
                                 kore::Operation() {
}

kore::RenderMeshOp::~RenderMeshOp(void) {
}

void kore::RenderMeshOp::execute(void) {
    const std::vector<kore::ShaderInput>& vAttributes =
                                                    _shader->getAttributes();

    for (unsigned int i = 0; i < vAttributes.size(); ++i) {
        const kore::ShaderInput& shaderAtt = vAttributes[i];
        const kore::MeshAttributeArray* meshAtt =
            _mesh->getAttributeByName(shaderAtt.name);

        if (!meshAtt) {
            Log::getInstance()->write("[ERROR] Mesh %s does not have an"
                "Attribute %s",
                _mesh->getName().c_str(),
                shaderAtt.name.c_str());
            return;
        }

        glEnableVertexAttribArray(shaderAtt.location);
        glVertexAttribPointer(shaderAtt.location, meshAtt->numComponents,
            meshAtt->componentType, GL_FALSE,
            0, meshAtt->data);
    }

    _shader->applyShader();
    // Update uniforms
    GLint iView =
        glGetUniformLocation(_shader->getProgramLocation(), "view");

    GLint iProj =
        glGetUniformLocation(_shader->getProgramLocation(), "projection");

    GLint iModel =
        glGetUniformLocation(_shader->getProgramLocation(), "model");

    glUniformMatrix4fv(iView, 1, GL_FALSE, glm::value_ptr(_camera->getView()));

    glUniformMatrix4fv(iProj, 1, GL_FALSE,
        glm::value_ptr(_camera->getProjection()));

    glUniformMatrix4fv(iModel, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    if (_mesh->hasIndices()) {
        glDrawElements(_mesh->getPrimitiveType(), _mesh->getIndices().size(),
            GL_UNSIGNED_INT, &_mesh->getIndices()[0]);
    } else {
        glDrawArrays(_mesh->getPrimitiveType(), 0, _mesh->getNumVertices());
    }

  setExecuted(true);
}

void kore::RenderMeshOp::update(void) {
}

void kore::RenderMeshOp::reset(void) {
  setExecuted(false);
}

const kore::MeshPtr& kore::RenderMeshOp::getMesh() const {
    return _mesh;
}

void kore::RenderMeshOp::setMesh(const kore::MeshPtr& mesh) {
    _mesh = mesh;
}

const kore::CameraPtr& kore::RenderMeshOp::getCamera() const {
    return _camera;
}

void kore::RenderMeshOp::setCamera(const kore::CameraPtr& camera) {
    _camera = camera;
}

const kore::ShaderPtr& kore::RenderMeshOp::getShader() const {
    return _shader;
}

void kore::RenderMeshOp::setShader(const kore::ShaderPtr& shader) {
    _shader = shader;
}
