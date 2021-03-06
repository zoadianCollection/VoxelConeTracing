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

#version 420 core

layout(r32ui) uniform volatile uimageBuffer nodePool_color;
layout(r32ui) uniform volatile uimageBuffer nodePool_next;
layout(r32ui) uniform volatile uimageBuffer nodePool_normal;
//layout(r32ui) uniform volatile uimageBuffer nodePool_X;
//layout(r32ui) uniform volatile uimageBuffer nodePool_Y;
//layout(r32ui) uniform volatile uimageBuffer nodePool_Z;
//layout(r32ui) uniform volatile uimageBuffer nodePool_X_neg;
//layout(r32ui) uniform volatile uimageBuffer nodePool_Y_neg;
//layout(r32ui) uniform volatile uimageBuffer nodePool_Z_neg;


void main() {
  imageStore(nodePool_color,gl_VertexID,uvec4(0));
  imageStore(nodePool_next,gl_VertexID,uvec4(0));
  imageStore(nodePool_normal,gl_VertexID,uvec4(0));

  /*
  imageStore(nodePool_X, gl_VertexID, uvec4(0));
  imageStore(nodePool_Y, gl_VertexID, uvec4(0));
  imageStore(nodePool_Z, gl_VertexID, uvec4(0));
  imageStore(nodePool_X_neg, gl_VertexID, uvec4(0));
  imageStore(nodePool_Y_neg, gl_VertexID, uvec4(0));
  imageStore(nodePool_Z_neg, gl_VertexID, uvec4(0));
  */
}


