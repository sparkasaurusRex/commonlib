TODO:

BPhysBaker goals:
* integrate minilzo to decompress blender smoke data: http://www.oberhumer.com/opensource/lzo/
* read smoke voxel data (heat, velocity vectors, density, etc) from a smoke simulation
* Project this data onto mesh either into vertex data or a texture by finding the closest
  voxel to every texel or vertex and splatting the data onto the surface of the mesh.
* Do this with multiple shell layers, like a Russian nesting doll
* In a realtime application / game, use this texture (probably as flipbooks) on a fireball
  shader, interpreting velocity as a flow-map texture, and density as opacity
