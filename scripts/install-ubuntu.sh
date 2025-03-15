#!/bin/bash

# APT sources
echo "Adding APT sources"
wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-noble.list http://packages.lunarg.com/vulkan/lunarg-vulkan-noble.list

# APT update
echo "Updating APT index"
sudo apt update

# GNU build tools
echo "Installing GNU Make and GCC"
sudo apt install make gcc

# Git
echo "Instaling Git"
sudo apt install git

# OpenGL
echo "Installing OpenGL and Xorg dependencies"
sudo apt install libopengl-dev libgl-dev libgl1-mesa-dev libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev libwayland-dev libxkbcommon-dev pkg-config

# Vulkan
echo "Installing Vulkan dependencies"
sudo apt install vulkan-sdk

# VCPKG
echo "Installing vcpkg"
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh

# CMake
echo "Installing CMake"
wget https://github.com/Kitware/CMake/releases/download/v3.27.0/cmake-3.27.0-linux-x86_64.sh
sudo chmod +x ./cmake-3.27.0-linux-x86_64.sh
mkdir cmake3.27.0
sudo ./cmake-3.27.0-linux-x86_64.sh --prefix=cmake3.27.0 --skip-license
ln -s ./cmake3.27.0/bin/cmake ~/