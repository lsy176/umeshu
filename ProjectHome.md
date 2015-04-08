# The project has been moved to [GitHub](http://www.github.com/vladimir-ch/umeshu/) #


---


Umeshu is a small, light-weight C++ library for unstructured triangular mesh generation in two dimensions using Delaunay refinement algorithms. The library implements the algorithm described in the paper J. R. Shewchuk, [Delaunay refinement algorithms for triangular mesh generation](http://dx.doi.org/10.1016/S0925-7721(01)00047-5), Computational Geometry 22(1-3) (2002), 21-74. The mesh is stored in a half-edge data structure and the implementation relies on generic programming using templates and on containers and tools provided by STL and [Boost](http://www.boost.org/).

Although other (faster, more sophisticated, complex, general) libraries and mesh generators exist (e.g. [CGAL](http://www.cgal.org/), [Triangle](http://www.cs.cmu.edu/~quake/triangle.html)), the principal advantage of Umeshu is its relative simplicity, the implementation is quite straightforward. It can be used as a learning tool for half-edge data structure, for Delaunay refinement techniques and as a basis upon which new mesh generation algorithms can be tested.

Any help, comments and bug reports are very welcome.

## Features ##

  * small and compact implementation of half-edge data structure
  * implementation of Ruppert's algorithm for unstructured mesh generation
  * handles small angles in input polygon by concentric shell splitting
  * mesh output to EPS
  * Shewchuk's adaptive floating-point predicates
  * CMake build system
  * mesh relaxation algorithm described in W. H. Frey, D. A. Field, [Mesh relaxation: A new technique for improving triangulations](http://dx.doi.org/10.1002/nme.1620310607), International Journal for Numerical Methods in Engineering 31(6) (1991), 1121-1133

## To do ##

  * implement more mesh refinement criteria
  * mesh smoothing
  * edge collapsing in mesh relaxation
  * test suite
  * mesh output to PLY, PLC (.poly), OBJ and OFF file formats
  * evaluation of mesh statistics
  * command-line application for mesh generation
  * provide means for inputting description of boundary conditions for use in FEM
  * far, far future: extend to domains with holes and with curved boundaries

## Example ##

![http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_1.png](http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_1.png)

Initial triangulation of the input polygon

![http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_2.png](http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_2.png)

Constrained Delaunay triangulation

![http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_3.png](http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_3.png)

Refined mesh

![http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_4.png](http://geraldine.fjfi.cvut.cz/~chalupecky/mesh_sample_4.png)

Mesh prepared for smoothing after application of a relaxation algorithm