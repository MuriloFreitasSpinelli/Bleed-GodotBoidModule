#pragma once
#include <godot_cpp/classes/node.hpp>

using namespace godot;





enum TeamFlags {
	TEAM_SPLAT = 1 << 0,
	TEAM_ABYSS = 1 << 1,
	TEAM_CHAOS = 1 << 2,
};

enum LayerFlags {
	LAYER_GROUND = 1 << 0,
	LAYER_FLYING = 1 << 1,
};


class GlobalVariables : public Node {
	GDCLASS(GlobalVariables, Node)

private:
	//World dimentions based on spatial grid fro boid calculations
	static float world_margin;
	static float left_border;
	static float right_border;
	static float top_border;
	static float bottom_border;

	//Layers and temas info for neearby filters and logic
	static int n_layers;
	static int n_teams;

protected:
	static void _bind_methods();

public:

};