rm -f ./src/resources/shaders/fragment.frag.spv
rm -f ./src/resources/shaders/vertex.vert.spv

glslc ./src/resources/shaders/fragment.frag -o ./src/resources/shaders/fragment.frag.spv &
glslc ./src/resources/shaders/vertex.vert -o ./src/resources/shaders/vertex.vert.spv