#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

struct NeighborMap {
	std::unordered_map<uint64_t, NeighborBlock> 
};

struct NeighborBlock {
	std::vector<float, 64> pos_x, pos_y;
	std::vector<float, 64> vel_x, vel_y;
	std::vector<uint32_t, 64> id;
	size_t count = 0;
};

class BoidNeighborFilter : public Node {
	GDCLASS(BoidNeighborFilter, Node)

private:


protected:


public:


};
