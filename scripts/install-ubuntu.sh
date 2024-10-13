#!/bin/bash

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