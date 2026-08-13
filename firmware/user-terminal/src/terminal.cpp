#include "terminal.h"

TerminalTelemetry collect_terminal_telemetry() {
    TerminalTelemetry telemetry;

    telemetry.signal_strength_dbm = -62.5;
    telemetry.temperature_c = 38.2;
    telemetry.uplink_packets = 820;
    telemetry.downlink_packets = 905;

    return telemetry;
}

bool terminal_health_check(const TerminalTelemetry& telemetry) {
    bool signal_ok =
        telemetry.signal_strength_dbm >= -90.0;

    bool temperature_ok =
        telemetry.temperature_c >= -20.0 &&
        telemetry.temperature_c <= 75.0;

    return signal_ok && temperature_ok;
}
