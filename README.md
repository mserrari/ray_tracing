
## Structure
``` text
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── data
│   ├── petanque.scn
│   ├── rayure.exr
│   └── ...
├── include
│   ├── camera.h
│   ├── scene.h
│   ├── plane.h
│   ├── ray.h
│   └── ...
├── src
│   ├── main.cpp
│   ├── material.cpp
│   ├── scene.cpp
│   └── ...
└── results
    ├── petanque_direct.png
    ├── petanque_flat.png
    └── ...
```

## Examples of different integrators

| Flat | Normals | Direct | Whitted |
|:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:
| ![](results/petanque_flat.png) | ![](results/petanque_normals.png) | ![](results/petanque_direct.png) | ![](results/petanque_whitted.png) |
| ![](results/phong_flat.png) | ![](results/phong_normals.png) | ![](results/phong_direct.png) | ![](results/phong_whitted.png) |
| ![](results/troisSpheres_flat.png) | ![](results/troisSpheres_normals.png) | ![](results/troisSpheres_direct.png) | ![](results/troisSpheres_whitted.png) |
| ![](results/tasSpheres_flat.png) | ![](results/tasSpheres_normals.png) | ![](results/tasSpheres_direct.png) | ![](results/tasSpheres_whitted.png) |

## Building

Build by making a build directory (i.e. `build/`), run `cmake` in that dir, and then use `make` to build the desired target.

Example:

``` bash
> mkdir build
> cd build
> cmake ../mds3d_td1
> make -j6
```

