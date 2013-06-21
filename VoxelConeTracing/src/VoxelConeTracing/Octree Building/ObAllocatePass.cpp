/*
 Copyright (c) 2012 The VCT Project

  This file is part of VoxelConeTracing and is an implementation of
  "Interactive Indirect Illumination Using Voxel Cone Tracing" by Crassin et al

  VoxelConeTracing is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  VoxelConeTracing is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with VoxelConeTracing.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
* \author Dominik Lazarek (dominik.lazarek@gmail.com)
* \author Andreas Weinmann (andy.weinmann@gmail.com)
*/

#include "VoxelConeTracing/Octree Building/ObAllocatePass.h"
#include "VoxelConeTracing/FullscreenQuad.h"

#include "KoRE\RenderManager.h"
#include "KoRE\ResourceManager.h"

#include "Kore\Operations\Operations.h"

ObAllocatePass::~ObAllocatePass(void) {
}

ObAllocatePass::ObAllocatePass(VCTscene* vctScene, uint level,
                               kore::EOperationExecutionType executionType) {
  using namespace kore;
  
  this->setExecutionType(executionType);

  _vctScene = vctScene;
  _renderMgr = RenderManager::getInstance();
  _sceneMgr = SceneManager::getInstance();
  _resMgr = ResourceManager::getInstance();
  _level = level;

  _allocateShader
     .loadShader("./assets/shader/ObAllocateVert.shader",
                 GL_VERTEX_SHADER);

  _allocateShader.init();
  _allocateShader.setName("ObAllocate shader");
  this->setShaderProgram(&_allocateShader);

  

  addStartupOperation(new MemoryBarrierOp(GL_ALL_BARRIER_BITS));

  _bindIndCmdBufOp =
    new kore::BindBuffer(GL_DRAW_INDIRECT_BUFFER,
                         vctScene->getNodePool()->
                         getAllocIndCmdBufForLevel(level)->getHandle());

  addStartupOperation(_bindIndCmdBufOp);

/*  addStartupOperation(
    new FunctionOp(std::bind(&ObAllocatePass::debugIndirectCmdBuff, this))); */
  
  addStartupOperation(new BindImageTexture(
                      vctScene->getNodePool()->getShdNodePool(NEXT),
                      _allocateShader.getUniform("nodePool_next")));

  addStartupOperation(new BindImageTexture(
                      vctScene->getNodePool()->getShdNodePool(COLOR),
                      _allocateShader.getUniform("nodePool_color")));

  addStartupOperation(
    new BindAtomicCounterBuffer(vctScene->getBrickPool()->getShdAcNextFree(),
                                _allocateShader.getUniform("nextFreeBrick")));

  addStartupOperation(
      new BindUniform(vctScene->getBrickPool()->getShdBrickPoolResolution(),
                      _allocateShader.getUniform("brickPoolResolution")));
  
  addStartupOperation(new BindAtomicCounterBuffer(
                       vctScene->getNodePool()->getShdAcNextFree(),
                       _allocateShader.getUniform("nextFreeAddress")));

  addStartupOperation(new DrawIndirectOp(GL_POINTS, 0));
  addFinishOperation(new MemoryBarrierOp(GL_ALL_BARRIER_BITS));
}

void ObAllocatePass::debugIndirectCmdBuff(){

  _renderMgr->bindBuffer(GL_DRAW_INDIRECT_BUFFER, _vctScene->getNodePool()->
                        getAllocIndCmdBufForLevel(_level)->getHandle());

  const GLuint* ptr = (const GLuint*) glMapBuffer(GL_DRAW_INDIRECT_BUFFER, GL_READ_ONLY);
  kore::Log::getInstance()->write("Alloc indirectCmdBuf contents on level %u:\n", _level);
  for (uint i = 0; i < 4; ++i) {
    kore::Log::getInstance()->write("%u ", ptr[i]);
  }
  kore::Log::getInstance()->write("\n");

  glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
}

void ObAllocatePass::setLevel(uint level) {
  _level = level;

  _bindIndCmdBufOp->connect(GL_DRAW_INDIRECT_BUFFER,
    _vctScene->getNodePool()->getAllocIndCmdBufForLevel(level)->getHandle());
}
