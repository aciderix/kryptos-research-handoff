import bpy, numpy as np
from mathutils import Vector
bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
obj=bpy.data.objects["Kryptos.Part.CopperSheet"]
me=obj.data
mw=obj.matrix_world
n=len(me.vertices)
co=np.empty((n,3),dtype=np.float64)
me.vertices.foreach_get("co",co.reshape(-1))
# to world
co=np.array([ (mw @ Vector(v)).to_tuple() for v in co ]) if False else co
# faster world transform
M=np.array(mw)
co_h=np.c_[co,np.ones(n)]
cow=(co_h @ M.T)[:,:3]
np.save("copper_verts.npy",cow)
print("saved",cow.shape)
for i,ax in enumerate("xyz"):
    print(ax,"min",round(cow[:,i].min(),2),"max",round(cow[:,i].max(),2),"span",round(cow[:,i].ptp(),2))
# also normals to guess sheet orientation
no=np.empty((n,3),dtype=np.float64)
me.vertices.foreach_get("normal",no.reshape(-1))
# rotate normals by world (ignore translation)
R=M[:3,:3]
now=no @ R.T
# dominant normal axis distribution
absmean=np.abs(now).mean(axis=0)
print("mean|normal| per axis (x,y,z):",np.round(absmean,3))
