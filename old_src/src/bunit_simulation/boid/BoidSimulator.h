#pragma once
#include <godot_cpp/classes/node.hpp>
#include "data/BoidStep.h"
#include "rules/BoidRuleSet.h"
#include "data/BoidSnapshot.h"

using namespace godot;

enum BaseSets {
	WANDERING,
	COMBAT,
};

class BoidSimulator : public Node {
	GDCLASS(BoidSimulator, Node)

private:
	TypedArray<Vector2> incremental_vectors;
	TypedArray<int> incremental_vectors_forces;
	TypedArray<Ref<BoidRuleSet>> rule_sets;
	Ref<BoidRuleSet> current_rule_set;
	Ref<BoidStep> last_step;

protected:
	static void _bind_methods();

public:
	//add _ready from godot here
	Ref<BoidStep> simulate_boid(Ref<BoidSnapshot> boid, Array<Ref<BoidSnapshot>> nearby);
	void change_rule_set(BaseSets set) {current_rule_set = rule_sets.get(set) }
};