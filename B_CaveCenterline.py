import bpy
import bmesh
import sys
from importlib import reload

sys.path.append('/home/stuart/Documents/CaveSurvey/')
import phd
from phd import *
reload(phd)

processSurvey()

verts = StationLocations()
edges = ShotList()

mesh_data = bpy.data.meshes.new("cube_mesh_data")
mesh_data.from_pydata(verts, edges, [])
mesh_data.update()

obj = bpy.data.objects.new("My_Object", mesh_data)

bpy.context.scene.collection.objects.link(obj)
bpy.context.scene.update()