#!/bin/sh
echo 'installing grub2'
nix profile install nixpkgs#grub2
echo 'installing qemu'
nix profile install nixpkgs#qemu
echo 'intalling libisoburn'
nix profile install nixpkgs#libisoburn
