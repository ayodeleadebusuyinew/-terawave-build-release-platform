#include <cassert>
#include <iostream>

#include "satellite.h"

int main() {
    std::cout << "=====================================\n";
    std::cout << "   TERAWAVE SATELLITE FIRMWARE TEST  \n";
    std::cout << "=====================================\n";

    SatelliteTelemetry telemetry = collect_telemetry();

    std::cout << "[TEST] Nominal telemetry ........ ";

    assert(telemetry.temperature_c == 42.5);
    assert(telemetry.battery_voltage == 28.4);
    assert(satellite_health_check(telemetry));

    std::cout << "PASS\n";

    std::cout << "[TEST] Over-temperature detection  ";

    SatelliteTelemetry overheating = telemetry;
    overheating.temperature_c = 100.0;

    assert(!satellite_health_check(overheating));

    std::cout << "PASS\n";

    std::cout << "[TEST] Low-voltage detection ...... ";

    SatelliteTelemetry low_voltage = telemetry;
    low_voltage.battery_voltage = 20.0;

    assert(!satellite_health_check(low_voltage));

    std::cout << "PASS\n";

    std::cout << "\nALL SATELLITE FIRMWARE TESTS: PASS\n";

    return 0;
}
