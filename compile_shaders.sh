#!/bin/bash
rm /home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/fragment.frag.spv
rm /home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/vertex.vert.spv
glslc /home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/fragment.frag -o /home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/fragment.frag.spv
glslc /home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/vertex.vert -o /home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/vertex.vert.spv
