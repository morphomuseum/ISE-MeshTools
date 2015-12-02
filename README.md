# ISE-MeshTools
3D surface editor, designed for palaeontologists, palaeo-anthropologists and biologists

Visit the official web page at http://morphomuseum.com/meshtools


##Build/Install MeshTools on Linux
###Install on Ubuntu > 15.04:
Use the PPA: https://launchpad.net/~zarquon42/+archive/ubuntu/meshtools
    
To add the PPA and install MeshTools run:

    
    sudo apt-add-repository ppa:zarquon42/meshtools
    sudo apt-get update
    sudo apt-get install meshtools
    
    
### Build MeshTools

1. Get Dependencies: Devel versions of ```fltk1.3, VTK (>=6.1), glew, xinerama, cmake```. (On Ubuntu this would be the packages:   ```cmake, libfltk1.3-dev, libglew-dev, libvtk6-dev, libxinerama-dev  ```)


      * If your distribution comes with an older version of VTK, you need to build and install it yorself: http://www.vtk.org/Wiki/VTK/Configure_and_Build
* Get MeshTools sources: Download [tarball](https://github.com/morphomuseum/ISE-MeshTools/archive/master.zip) or clone the repo:
  
  ```git clone https://github.com/morphomuseum/ISE-MeshTools.git```
* Build it:
 
  ```
  mkdir MeshTools-build && cd MeshTools-build
  cmake [Path_To_MeshTools_source] -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_CXX_FLAGS='--std=c++0x'
  make 
  make install
   ```


    
