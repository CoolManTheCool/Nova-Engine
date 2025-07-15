#!/bin/bash

projectroot=$PWD

sourcedir=$projectroot/app/resources
tempdir=/tmp/nova-engine-build-$(date +%s%N)
targetdir=$projectroot/bin/resources

if [[ "$1" == "-clean" ]]; then
    echo "Cleaning up resource files in $projectroot/build..."
    rm -rf "$projectroot/build"
    echo "Cleaning up resource files in $projectroot/bin..."
    rm -rf "$projectroot/bin"
    echo "Cleanup completed."
    exit 0
fi

echo "[ OK ] Cleaning: $targetdir"
echo 

rm -rf $targetdir
mkdir -p $tempdir

echo "[ OK ] Compiling Resources to $tempdir..."

echo "[ OK ] Compiling Shaders: $sourcedir/shaders -> $tempdir/shaders..."
mkdir -p $tempdir/shaders

shader_files=($(find "$sourcedir/shaders" -type f \( -name "*.frag" -o -name "*.vert" \)))
total_files=${#shader_files[@]}
compiled_count=0

# Compile each shader and track progress
for shader in "${shader_files[@]}"; do
    # Extract relative path and filename
    relpath="${shader#$sourcedir/}"
    filename=$(basename "$shader")
    output="$tempdir/${relpath}.spv"

    glslc "$shader" -o "$output"

    ((compiled_count++))
    percentage=$((compiled_count * 100 / total_files))
    printf "[%3d%%] Finished Compiling: %s\n" "$percentage" "$filename"
done

echo 
echo "[ OK ] Copying: $sourcedir/models -> $tempdir/models..."
cp -r $sourcedir/models $tempdir/models

mkdir -p $projectroot/bin

echo "[ OK ] Copying: $tempdir -> $targetdir..."
cp -r $tempdir $targetdir

echo "[ OK ] Deleting: $tempdir..."
rm -rf $tempdir

echo
echo Compilation finished successfully.