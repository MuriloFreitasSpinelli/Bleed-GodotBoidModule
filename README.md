# Bleed-GodotBoidModule
Module to extend Godot physics functionality to include multithreading and c++ logic especifically for boids.

Created using GDExtendion for easier Godot implementation.

Module is being used in the game Bleed... (name)

Adds a few new classes (nodes, resources, etc..), they are:
  BoidSpatialHash (Optimization) - extends - Node
  BoidPhysicsBody - extends - RigidBody2D
  BoidPhysicsServer (Singleton) - extends - Node
  BoidPhysicsStep - extends - RefCounted
  BoidPhysicsRule - extends - Resource
  BoidPhysicsRuleSet - extends - Resource
  BoidPhysicsGlobal (AutoLoad (Singleton)) - extends - Node

Technicals:
  BoidSpatialHash extends Node
    - #Simple spatial hash focused on quick neighbor lookups, only keeps track of ids issued by the simulator, so its easily interchangable.
    - cell_size : int
    - vertical_cells : int
    - horizontal_cells : int
    - current_tracked_ids : Array[Array[PackedIntArray]] #X and Y axis and ids
    - radius_multiplier : int #changes the
    - radius_lut : Array[Vector2i[]] # lut is incremental so if you are of range 4 you should get radius_lut from 0 to 4. radius_lut is decided by the Body ranges selected
    - # add_id_at/remove_id_at/resize/get_neighbors_at/clear_grid/world_to_grid/grid_to_world

  BoidPhysicsBody extends RigidBody
    -
  
