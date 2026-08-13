#include <iostream>
#include "satellite.h"

int main() {
    std::cout << "=====================================\n";
    std::cout << "      TERAWAVE SATELLITE SYSTEM      \n";
    std::cout << "=====================================\n";

    SatelliteTelemetry telemetry = collect_telemetry();

    std::cout << "Temperature: "
              << telemetry.temperature_c
              << " C\n";

    std::cout << "Battery Voltage: "
              << telemetry.battery_voltage
              << " V\n";

    std::cout << "Packets TX: "
              << telemetry.packets_tx
              << "\n";

    std::cout << "Packets RX: "
              << telemetry.packets_rx
              << "\n";

    if (satellite_health_check(telemetry)) {
        std::cout << "Satellite Health: HEALTHY\n";
        return 0;
    }

    std::cout << "Satellite Health: DEGRADED\n";
    return 1;
}
