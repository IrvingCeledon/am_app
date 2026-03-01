#pragma once

#include "engine_base.hpp"

template<typename IndividualType, typename ConfigType>
class GeneticEngineBase : public EngineBase<IndividualType, ConfigType>
{
protected:
    virtual void initialize() = 0;    // Initialize() sorts population at the end.
    virtual void evaluate() = 0;
    virtual void selection() = 0;
    virtual void crossover() = 0;
    virtual void mutate() = 0;
    virtual void save_history(std::vector<Genome>& target) = 0;
    
    void perform_iteration() override
    {
        this->selection();
        this->crossover();
        this->mutate();
        this->evaluate();
        std::sort(this->pop.begin(), this->pop.end());
    }

public: 
    explicit GeneticEngineBase(const ConfigType& config) 
        : EngineBase<IndividualType, ConfigType>(config) {}
};
