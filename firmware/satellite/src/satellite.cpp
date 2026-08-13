#include "satellite.h"

SatelliteTelemetry collect_telemetry() {
    SatelliteTelemetry telemetry;

    telemetry.temperature_c = 42.5;
    telemetry.battery_voltage = 28.4;
    telemetry.packets_tx = 1250;
    telemetry.packets_rx = 1187;

    return telemetry;
}

bool satellite_health_check(const SatelliteTelemetry& telemetry) {
    bool temperature_ok =
        telemetry.temperature_c > -40.0 &&
        telemetry.temperature_c < 85.0;

    bool voltage_ok =
        telemetry.battery_voltage >= 24.0 &&
        telemetry.battery_voltage <= 32.0;

    return temperature_ok && voltage_ok;
}
