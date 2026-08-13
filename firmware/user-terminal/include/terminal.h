#ifndef TERAWAVE_TERMINAL_H
#define TERAWAVE_TERMINAL_H

struct TerminalTelemetry {
    double signal_strength_dbm;
    double temperature_c;
    int uplink_packets;
    int downlink_packets;
};

TerminalTelemetry collect_terminal_telemetry();

bool terminal_health_check(const TerminalTelemetry& telemetry);

#endif
