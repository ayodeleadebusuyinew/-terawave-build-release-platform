package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"sync/atomic"
	"time"

	"github.com/terawave/terawave-build-release-platform/services/telemetry-ingestion/internal/telemetry"
)

var received uint64

func healthHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")

	response := map[string]string{
		"service": "terawave-telemetry-ingestion",
		"status":  "healthy",
	}

	json.NewEncoder(w).Encode(response)
}

func telemetryHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "POST required", http.StatusMethodNotAllowed)
		return
	}

	var data telemetry.SatelliteTelemetry

	if err := json.NewDecoder(r.Body).Decode(&data); err != nil {
		http.Error(w, "invalid telemetry payload", http.StatusBadRequest)
		return
	}

	if data.Timestamp.IsZero() {
		data.Timestamp = time.Now().UTC()
	}

	atomic.AddUint64(&received, 1)

	response := map[string]interface{}{
		"accepted":     true,
		"satellite_id": data.SatelliteID,
		"healthy":      data.Healthy(),
		"timestamp":    data.Timestamp,
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

func metricsHandler(w http.ResponseWriter, r *http.Request) {
	count := atomic.LoadUint64(&received)

	w.Header().Set("Content-Type", "text/plain")
	fmt.Fprintf(w, "terawave_telemetry_received_total %d\n", count)
}

func main() {
	http.HandleFunc("/health", healthHandler)
	http.HandleFunc("/telemetry", telemetryHandler)
	http.HandleFunc("/metrics", metricsHandler)

	log.Println("TeraWave Telemetry Ingestion listening on :8085")

	if err := http.ListenAndServe(":8085", nil); err != nil {
		log.Fatal(err)
	}
}
