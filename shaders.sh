#!/bin/bash

echo Deleting old Shaders

rm -rf /home/noah/Projects/Nova-Engine/bin/resources/shaders/circle.frag.spv
rm -rf /home/noah/Projects/Nova-Engine/bin/resources/shaders/circle.vert.spv
rm -rf /home/noah/Projects/Nova-Engine/bin/resources/shaders/mesh.frag.spv
rm -rf /home/noah/Projects/Nova-Engine/bin/resources/shaders/mesh.vert.spv

echo Compiling Shaders

glslc /home/noah/Projects/Nova-Engine/app/resources/shaders/circle.frag -o /home/noah/Projects/Nova-Engine/bin/resources/shaders/circle.frag.spv
glslc /home/noah/Projects/Nova-Engine/app/resources/shaders/circle.vert -o /home/noah/Projects/Nova-Engine/bin/resources/shaders/circle.vert.spv
glslc /home/noah/Projects/Nova-Engine/app/resources/shaders/mesh.frag -o /home/noah/Projects/Nova-Engine/bin/resources/shaders/mesh.frag.spv
glslc /home/noah/Projects/Nova-Engine/app/resources/shaders/mesh.vert -o /home/noah/Projects/Nova-Engine/bin/resources/shaders/mesh.vert.spv

echo Finished Compilation