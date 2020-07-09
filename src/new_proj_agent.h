// -----------------------------------------------------------------------------
//
// Copyright (C) The BioDynaMo Project.
// All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------
//


#include "biodynamo.h"

namespace bdm {

class Agent : public Cell {
  BDM_SIM_OBJECT_HEADER(Agent, Cell, 1, infected_, color_);

 public:
  Agent() {}
  explicit Agent(const Double3& position) : Base(position) {}

  Agent(const Event& event, SimObject* other, uint64_t new_oid = 0)
      : Base(event, other, new_oid) {}

  /// If a agent divides, daughter keeps the same state from its mother.
  void EventHandler(const Event& event, SimObject* other1,
                    SimObject* other2 = nullptr) override {
    Base::EventHandler(event, other1, other2);
  }

  // getter and setter for our new data member
  void SetInfected(bool i) {
  	infected_ = i; 
  	if (i) {SetColor(4);} else {SetColor(0);} 
  }
  bool GetInfected() const { return infected_; }
  int GetColor() const { return color_;}
  void SetColor(int new_color){ color_ = new_color; }
 private:
  bool infected_;
  int color_;
};

// Define growth behaviour
struct GrowthModule : public BaseBiologyModule {
  BDM_STATELESS_BM_HEADER(GrowthModule, BaseBiologyModule, 1);

  GrowthModule() : BaseBiologyModule(gAllEventIds) {}

  template <typename TEvent, typename TBm>
  GrowthModule(const TEvent& event, TBm* other, uint64_t new_oid = 0)
      : BaseBiologyModule(event, other, new_oid) {}

  void Run(SimObject* so) override {}
};

inline int Simulate(int argc, const char** argv) {
  auto set_param = [](Param* param) {
    param->bound_space_ = true;
    param->min_bound_ = 0;
    param->max_bound_ = 1000;
  };

  Simulation simulation(argc, argv, set_param);
  auto* rm = simulation.GetResourceManager();
  auto* param = simulation.GetParam();
  auto* myrand = simulation.GetRandom();

  size_t nb_of_agents = 240;
  double x_coord, y_coord, z_coord;

  for (size_t i = 0; i < nb_of_agents; ++i) {
    x_coord = myrand->Uniform(param->min_bound_, param->max_bound_);
    y_coord = myrand->Uniform(param->min_bound_, param->max_bound_);
    z_coord = 0;
    Agent* agent = new Agent({x_coord, y_coord, z_coord});
    agent->SetDiameter(7.5);
    agent->SetInfected(false);
    rm->push_back(agent);
  }

  Agent* agent = new Agent({20, 50, 0});
  agent->SetDiameter(6);

  simulation.GetScheduler()->Simulate(1);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

