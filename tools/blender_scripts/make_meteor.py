
import bpy
import random
import math

mesh_res = 2

def displace_obj(o, noise_scale, strength):
    dis_mod = o.modifiers.new('Displacer', 'DISPLACE')
    dis_mod.texture = bpy.data.textures['Texture']
    dis_mod.texture.noise_scale = noise_scale * random.uniform(0.5, 2.0)
    dis_mod.texture_coords = 'GLOBAL'
    dis_mod.strength = strength * random.uniform(0.5, 1.5)
    o.location = (random.uniform(-10.0, 10.0), random.uniform(-10.0, 10.0), random.uniform(-10.0, 10.0))
    bpy.ops.object.modifier_apply(modifier = dis_mod.name)
    o.location = (0,0,0)

def magnitude(v):
    return math.sqrt(sum(v[i]*v[i] for i in range(len(v))))

def normalize(v):
    vmag = magnitude(v)
    return [ v[i]/vmag  for i in range(len(v)) ]

#add a unit geodesic sphere @ the origin
bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=mesh_res)
root_sphere = bpy.context.object
root_sphere.name = 'Meteor'

#add a displacement modifier to the sphere to make it all wobbly
displace_obj(root_sphere, 1.0, 1.0)

for i in range(1, 25):
    sphere_size = random.uniform(0.2, 0.6)
    sphere_loc = (random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
    sphere_loc = normalize(sphere_loc)
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=mesh_res, size=sphere_size, location=sphere_loc)
    spherelet = bpy.context.object
    displace_obj(spherelet, 1.5, 0.7)
    spherelet.location = sphere_loc

    #boolean subtract this little sphere from the root sphere
    bool_mod = root_sphere.modifiers.new('Whats_the_difference', 'BOOLEAN')
    bool_mod.object = spherelet
    bool_mod.operation ='DIFFERENCE'
    bpy.context.scene.objects.active = root_sphere
    root_sphere.select = True
    spherelet.select = False
    bpy.ops.object.modifier_apply(modifier = bool_mod.name)

    #delete the spherelet
    root_sphere.select = False
    spherelet.select = True
    bpy.ops.object.delete()

#clean up the mesh and remove double verts, etc...
root_sphere.select = True
bpy.ops.object.mode_set(mode='EDIT')
bpy.ops.mesh.remove_doubles(threshold=0.075, use_unselected=True)
bpy.ops.object.mode_set(mode='OBJECT')
