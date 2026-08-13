#include <iostream>
#include "terminal.h"

int main() {
    std::cout << "=====================================\n";
    std::cout << "       TERAWAVE USER TERMINAL        \n";
    std::cout << "=====================================\n";

    TerminalTelemetry telemetry =
        collect_terminal_telemetry();

    std::cout << "Signal Strength: "
              << telemetry.signal_strength_dbm
              << " dBm\n";

    std::cout << "Temperature: "
              << telemetry.temperature_c
              << " C\n";

    std::cout << "Uplink Packets: "
              << telemetry.uplink_packets
              << "\n";

    std::cout << "Downlink Packets: "
              << telemetry.downlink_packets
              << "\n";

    if (terminal_health_check(telemetry)) {
        std::cout << "Terminal Health: HEALTHY\n";
        return 0;
    }

    std::cout << "Terminal Health: DEGRADED\n";
    return 1;
}
