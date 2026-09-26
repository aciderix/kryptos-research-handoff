import bpy, numpy as np
from mathutils import Vector
bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
obj=bpy.data.objects["Kryptos.Part.CopperSheet"]; me=obj.data
n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
M=np.array(obj.matrix_world); cow=(np.c_[co,np.ones(n)]@M.T)[:,:3]
# scale to meters (12ft anchor): model unit -> meters
MPU=0.02935
cow*=MPU
faces=[list(p.vertices) for p in me.polygons]
with open("copper_sheet_meters.obj","w") as f:
    f.write("# Kryptos copper sheet (both panels), from Bowen 3D model, scaled to meters (12ft anchor)\n")
    f.write("# RECONSTRUCTION (photos + Gary Phillips letter positions), not a survey\n")
    f.write(f"# {n} vertices, {len(faces)} faces\n")
    for v in cow: f.write(f"v {v[0]:.5f} {v[1]:.5f} {v[2]:.5f}\n")
    for fc in faces:
        f.write("f "+" ".join(str(i+1) for i in fc)+"\n")
import os
print("wrote copper_sheet_meters.obj", os.path.getsize("copper_sheet_meters.obj"),"bytes")
