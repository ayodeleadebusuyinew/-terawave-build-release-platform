package telemetry

import "time"

type SatelliteTelemetry struct {
	SatelliteID    string    `json:"satellite_id"`
	TemperatureC   float64   `json:"temperature_c"`
	BatteryVoltage float64   `json:"battery_voltage"`
	PacketsTX      int       `json:"packets_tx"`
	PacketsRX      int       `json:"packets_rx"`
	Timestamp      time.Time `json:"timestamp"`
}

func (t SatelliteTelemetry) Healthy() bool {
	temperatureOK := t.TemperatureC > -40 && t.TemperatureC < 85
	voltageOK := t.BatteryVoltage >= 24 && t.BatteryVoltage <= 32

	return temperatureOK && voltageOK
}
