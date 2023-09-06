#include "../include/exhaust_system.h"

#include "../include/units.h"

static GasSystem::Mix mix;

ExhaustSystem::ExhaustSystem() {
    m_primaryFlowRate = 0;
    m_outletFlowRate = 0;
    m_collectorCrossSectionArea = 0;
    m_length = 0;
    m_primaryTubeLength = 0;
    m_audioVolume = 0;
    m_velocityDecay = 0;
    m_flow = 0;
    m_index = -1;
    m_impulseResponse = nullptr;
}

ExhaustSystem::~ExhaustSystem() {
    /* void */
}

void ExhaustSystem::initialize(const Parameters &params) {
    const double systemWidth = std::sqrt(params.collectorCrossSectionArea);
    const double volume = params.collectorCrossSectionArea * params.length;
    const double systemLength = params.length;
    m_system.initialize(
            units::pressure(1.0, units::atm),
            volume,
            units::celcius(25.0),
            mix);
    m_system.setGeometry(
        systemLength,
        systemWidth,
        1.0,
        0.0);

    m_atmosphere.initialize(
        units::pressure(1.0, units::atm),
        units::volume(1000.0, units::m3),
        units::celcius(25.0),
        mix);
    m_atmosphere.setGeometry(
        units::distance(10.0, units::m),
        units::distance(10.0, units::m),
        1.0,
        0.0);

    m_primaryFlowRate = params.primaryFlowRate;
    m_audioVolume = params.audioVolume;
    m_outletFlowRate = params.outletFlowRate;
    m_collectorCrossSectionArea = params.collectorCrossSectionArea;
    m_velocityDecay = params.velocityDecay;
    m_impulseResponse = params.impulseResponse;
    m_length = params.length;
    m_primaryTubeLength = params.primaryTubeLength;
}

void ExhaustSystem::destroy() {
    /* void */
}

void ExhaustSystem::process(double dt) {
    GasSystem::Mix airMix;
    airMix.p_fuel = 0;
    airMix.p_inert = 1.0;
    airMix.p_o2 = 0.0;

    m_atmosphere.reset(units::pressure(1.0, units::atm), units::celcius(25.0), airMix);
    GasSystem::FlowParameters flowParams;
    flowParams.crossSectionArea_0 = m_collectorCrossSectionArea;
    flowParams.crossSectionArea_1 = units::area(10, units::m2);
    flowParams.direction_x = 1.0;
    flowParams.direction_y = 0.0;
    flowParams.dt = dt;
    flowParams.system_0 = &m_atmosphere;
    flowParams.system_1 = &m_system;
    flowParams.k_flow = m_outletFlowRate;

    m_flow = m_system.flow(flowParams);

    m_system.dissipateExcessVelocity();
    m_system.updateVelocity(dt, m_velocityDecay);
}
