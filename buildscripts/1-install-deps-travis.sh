#!/bin/bash
set -x
set -e
set -u

sudo mkdir -p /data/bin
sudo chmod uga+rwx /data/bin

GITHUB_RELEASE_TOOL_USER="c4milo"
GITHUB_RELEASE_TOOL_VERSION="v1.1.0"

mkdir -p deps
pushd deps

if [ "$(uname)" == "Darwin" ];
then
  brew install ninja md5sha1sum
  GITHUB_RELEASE_TOOL_ARCH="darwin_amd64"
  wget https://github.com/paulthomson/build-angle/releases/download/v-592879ad24e66c7c68c3a06d4e2227630520da36/Darwin-x64-Release.zip
  unzip Darwin-x64-Release.zip
fi

if [ "$(uname)" == "Linux" ];
then
  sudo apt-get update
  sudo apt-get -y install libxrandr-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev cmake zip git ninja-build
  GITHUB_RELEASE_TOOL_ARCH="linux_amd64"
  wget https://github.com/paulthomson/build-angle/releases/download/v-592879ad24e66c7c68c3a06d4e2227630520da36/Linux-x64-Release.zip
  unzip Linux-x64-Release.zip
fi

popd

pushd /data/bin
wget "https://github.com/${GITHUB_RELEASE_TOOL_USER}/github-release/releases/download/${GITHUB_RELEASE_TOOL_VERSION}/github-release_${GITHUB_RELEASE_TOOL_VERSION}_${GITHUB_RELEASE_TOOL_ARCH}.tar.gz"
tar xf "github-release_${GITHUB_RELEASE_TOOL_VERSION}_${GITHUB_RELEASE_TOOL_ARCH}.tar.gz"
popd
