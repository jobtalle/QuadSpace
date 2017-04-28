# QuadSpace
An 8-level quad tree spatial organization algorithm

This algorithm can be used to fill the area of a square economically using a quadtree. Possible applications are:

* (Dynamic) texture atlases
* A quick but suboptimal solution for the packing problem
* Procedurally placing content in randomly generated environments

The root of the quad tree consists of *128x128* so called "*atoms*" and the tree has 8 levels. Level 0 is the root node, level 1 has *64x64* atoms per node, level 2 *32x32* atoms per node and so forth. When allocating *8x8* atoms (level 4), the algorithm will find a free space within its *128x128* atom space to place such a fragment and return a node to represent and later release it.
