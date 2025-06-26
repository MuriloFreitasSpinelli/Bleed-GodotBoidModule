# Bleed-GodotBoidModule
Module to extend Godot physics functionality to include multithreading and c++ logic especifically for boids.

Created using GDExtendion for easier Godot implementation.

Module is being used in the game Bleed... (name)

Adds a few new classes (nodes, resources, etc..), they are:
  BoidSpatialHash (Optimization) - extends - Node
  BoidPhysicsBody - extends - RigidBody2D
  BoidPhysicsSimulator - extends - Node
  BoidPhysicsStep - extends - RefCounted
  BoidPhysicsRule - extends - Resource
  BoidPhysicsRuleSet - extends - Resource
  BoidPhysicsGlobal (AutoLoad (Singleton)) - extends - Node

More details in the technicals lower in the readme

Quickstart:
  - Add SpatialHash node to scene tree
  - Add Simulator node to scene tree as a child of SpatialHash
  - Create a new scene with sBody as root
  - In Body create a RuleSet for the boid, there are various basic presets. 
  - In RuleSet there are various rules that the rule set will apply to the boid
  - In RuleSet there are various Rules, this is where you can pick which layers or teams you want the rule to affect, you can change these teams and layers as you wish (more on that in Rule)
  - Call simulator.register(body : Body, position : Position, initial_vel : Vector2): somehow in some way depending on what you want of course.
Its posible to not pass initial velocity or position and it will be given a random position and velocity all within SpatialHash Bounds.


  
  
  
