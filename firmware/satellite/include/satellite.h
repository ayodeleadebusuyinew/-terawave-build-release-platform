#ifndef TERAWAVE_SATELLITE_H
#define TERAWAVE_SATELLITE_H

struct SatelliteTelemetry {
    double temperature_c;
    double battery_voltage;
    int packets_tx;
    int packets_rx;
};

SatelliteTelemetry collect_telemetry();

bool satellite_health_check(const SatelliteTelemetry& telemetry);

#endif
