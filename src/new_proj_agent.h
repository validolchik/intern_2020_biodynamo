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
// A simulation of a conceptual model of a cancer tumor growth
//

#ifndef DEMO_TUMOR_CONCEPT_H_
#define DEMO_TUMOR_CONCEPT_H_

#include "biodynamo.h"

namespace bdm {

// Define my custom agent Agent, which extends agent by adding extra data
// members: agent_color and can_divide
class Agent : public Cell {  // our object extends the agent object
                              // create the header with our new data member
  BDM_SIM_OBJECT_HEADER(Agent, Cell, 1, infected_, color_);

 public:
  Agent() {}
  explicit Agent(const Double3& position) : Base(position) {}

  /// If Agent divides, daughter 2 copies the data members from the mother
  Agent(const Event& event, SimObject* other, uint64_t new_oid = 0)
      : Base(event, other, new_oid) {
    // if (auto* mother = dynamic_cast<Agent*>(other)) {
    //    = mother->;
    //   if (event.GetId() == agentDivisionEvent::kEventId) {
    //     // the daughter will be able to divide
    //     infected_ = true;
    //   } else {
    //     infected_ = mother->infected_;
    //   }
    // }
  }

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
  // declare new data member and define their type
  // private data can only be accessed by public function and not directly
  bool infected_;
  int color_;
};

// Define growth behaviour
struct GrowthModule : public BaseBiologyModule {
  BDM_STATELESS_BM_HEADER(GrowthModule, BaseBiologyModule, 1);

  GrowthModule() : BaseBiologyModule(gAllEventIds) {}

  /// Empty default event constructor, because GrowthModule does not have state.
  template <typename TEvent, typename TBm>
  GrowthModule(const TEvent& event, TBm* other, uint64_t new_oid = 0)
      : BaseBiologyModule(event, other, new_oid) {}

  /// event handler not needed, because Chemotaxis does not have state.

  void Run(SimObject* so) override {
    if (auto* agent = dynamic_cast<Agent*>(so)) {
      // if (agent->GetDiameter() < 8) {
      //   auto* random = Simulation::GetActive()->GetRandom();
      //   // Here 400 is the speed and the change to the volume is based on the
      //   // simulation time step.
      //   // The default here is 0.01 for timestep, not 1.
      //   agent->ChangeVolume(400);

      //   // create an array of 3 random numbers between -2 and 2
      //   Double3 agent_movements = random->template UniformArray<3>(-2, 2);
      //   // update the agent mass location, ie move the agent
      //   agent->UpdatePosition(agent_movements);
      // } else {  //
      //   auto* random = Simulation::GetActive()->GetRandom();

      //   if (agent->GetCanDivide() && random->Uniform(0, 1) < 0.8) {
      //     agent->Divide();
      //   } else {
      //     agent->SetCanDivide(false);  // this agent won't divide anymore
      //   }
      // }
    }
  }
};

inline int Simulate(int argc, const char** argv) {
  auto set_param = [](Param* param) {
    param->bound_space_ = true;
    param->min_bound_ = 0;
    param->max_bound_ = 1000;  // cube
  };

  Simulation simulation(argc, argv, set_param);
  auto* rm = simulation.GetResourceManager();
  auto* param = simulation.GetParam();
  auto* myrand = simulation.GetRandom();

  size_t nb_of_agents = 240;  // number of agents in the simulation
  double x_coord, y_coord, z_coord;

  for (size_t i = 0; i < nb_of_agents; ++i) {
    // our modelling will be a agent cube of 100*100*100
    // random double between 0 and 100
    x_coord = myrand->Uniform(param->min_bound_, param->max_bound_);
    y_coord = myrand->Uniform(param->min_bound_, param->max_bound_);
    // z_coord = myrand->Uniform(param->min_bound_, param->max_bound_);
    z_coord = 0;

    // creating the agent at position x, y, z
    Agent* agent = new Agent({x_coord, y_coord, z_coord});
    // set agent parameters
    agent->SetDiameter(7.5);
    // will vary from 0 to 5. so 6 different layers depending on y_coord
    // agent->SetagentColor(static_cast<int>((y_coord / param->max_bound_ * 6)));
    agent->SetInfected(false);
    rm->push_back(agent);  // put the created agent in our agents structure
  }

  // create a cancerous agent, containing the biology module GrowthModule
  Agent* agent = new Agent({20, 50, 0});
  agent->SetDiameter(6);
  // agent->SetagentColor(8);
  // agent->SetCanDivide(true);
  // agent->AddBiologyModule(new GrowthModule());
  agent->SetInfected(true);
  rm->push_back(agent);  // put the created agent in our agents structure

  // Run simulation
  simulation.GetScheduler()->Simulate(1);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // DEMO_TUMOR_CONCEPT_H_
