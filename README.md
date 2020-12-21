## Raytracing c++
This repository contains the source code to render objects using raytracing

<!-- ![Ray tracing figure](images/ray-tracing-image.jpg?style=center) -->
<p align="center">
  <img src="images/ray-tracing-image.jpg?style=center" alt="Sublime's custom image" width=600px/>
</p>

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

In the first part of the program, I have implemented the following integrators:

* Flat
* Normals
* Direct
* Whitted

| Flat | Normals | Direct | Whitted |
|:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:
| ![](images/results/petanque_flat.png) | ![](images/results/petanque_normals.png) | ![](images/results/petanque_direct.png) | ![](images/results/petanque_whitted.png) |
| ![](images/results/phong_flat.png) | ![](images/results/phong_normals.png) | ![](images/results/phong_direct.png) | ![](images/results/phong_whitted.png) |
| ![](images/results/troisSpheres_flat.png) | ![](images/results/troisSpheres_normals.png) | ![](images/results/troisSpheres_direct.png) | ![](images/results/troisSpheres_whitted.png) |
| ![](images/results/tasSpheres_flat.png) | ![](images/results/tasSpheres_normals.png) | ![](images/results/tasSpheres_direct.png) | ![](images/results/tasSpheres_whitted.png) |

## Rendering Mesh

* BVH : When using BVH the time gains very remarkable (from 60s to 1s)

| Face Normals | Barycentriques Normals | Increasing Mesh Resolution |
|:-------------------------:|:-------------------------:|:-------------------------:|
| ![](images/results2/maillage.png) | ![](images/results2/maillage1.png) | ![](images/results2/maillage2.png) |

## Missing features

### Textures
| Petanque ⬜️ | Terre & Lune ⬜️ | Deux Plans ✅ | Tas Spheres ⬜️ |
|:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:|
| ![](images/results2/petanque.png) | ![](images/results2/terre-lune.png) | ![](images/results2/deuxPlans.png) | ![](images/results2/tasSpheres.png) |

### Rednering killeroo

When trying to render killeroo, the program runs into an allocation error.

| Killeroo | Killeroo Glass |
|:-------------------------:|:-------------------------:|
| ![](images/results2/killeroo.png) | ![](images/results2/killerooGlass.png) |


## Building and running the project

Build by making a build directory (i.e. `build/`), run `cmake` in that dir, and then use `make` to build the desired target.

#### First compilation:
  
``` bash
> mkdir build
> cd build
> cmake ../ray_tracing
> make -j6
```

#### Recompile:

``` bash
> cmake ../ray_tracing -DCMAKE_BUILD_TYPE=Debug
```

#### Run:

``` bash
> ./m3d_raytracer ../m3d_td1/data/troisSpheres.scn
```

## References
https://www.labri.fr/perso/pbenard/teaching/mondes3d/
