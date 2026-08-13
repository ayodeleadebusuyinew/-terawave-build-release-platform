package telemetry

import "testing"

func TestHealthyTelemetry(t *testing.T) {
	data := SatelliteTelemetry{
		SatelliteID:    "SAT-LEO-001",
		TemperatureC:   42.5,
		BatteryVoltage: 28.4,
		PacketsTX:      1250,
		PacketsRX:      1187,
	}

	if !data.Healthy() {
		t.Fatal("expected telemetry to be healthy")
	}
}

func TestOverTemperatureTelemetry(t *testing.T) {
	data := SatelliteTelemetry{
		SatelliteID:    "SAT-LEO-002",
		TemperatureC:   100.0,
		BatteryVoltage: 28.4,
	}

	if data.Healthy() {
		t.Fatal("expected over-temperature telemetry to be unhealthy")
	}
}

func TestLowVoltageTelemetry(t *testing.T) {
	data := SatelliteTelemetry{
		SatelliteID:    "SAT-LEO-003",
		TemperatureC:   42.5,
		BatteryVoltage: 20.0,
	}

	if data.Healthy() {
		t.Fatal("expected low-voltage telemetry to be unhealthy")
	}
}
