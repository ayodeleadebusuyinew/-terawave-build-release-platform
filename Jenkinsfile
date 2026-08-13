pipeline {
    agent any

    options {
        timestamps()
        disableConcurrentBuilds()
    }

    environment {
        CI_IMAGE = 'terawave/ci-builder:0.1.0'
        TELEMETRY_IMAGE = 'terawave/telemetry-ingestion'
        TELEMETRY_TAG = "ci-${BUILD_NUMBER}"
    }

    stages {

        stage('Workspace Validation') {
            steps {
                sh '''
                    echo "============================================"
                    echo " TERAWAVE SOURCE-TO-PRODUCTION PIPELINE"
                    echo "============================================"

                    echo
                    echo "BUILD NUMBER:"
                    echo "${BUILD_NUMBER}"

                    echo
                    echo "WORKSPACE:"
                    pwd

                    echo
                    echo "SOURCE TREE:"
                    ls -la

                    echo
                    echo "DOCKER:"
                    docker --version
                '''
            }
        }

        stage('CI Environment Validation') {
            steps {
                sh '''
                    docker run --rm \
                      -v "$WORKSPACE:/workspace" \
                      -w /workspace \
                      ${CI_IMAGE} \
                      bash -c '
                        set -e

                        echo "===== BAZEL ====="
                        bazel version | grep -E "Bazelisk version|Build label"

                        echo
                        echo "===== GO ====="
                        go version

                        echo
                        echo "===== ARM64 ====="
                        aarch64-linux-gnu-g++ --version | head -1

                        echo
                        echo "===== RISC-V ====="
                        riscv64-linux-gnu-g++ --version | head -1

                        echo
                        echo "===== GIT ====="
                        git --version
                      '
                '''
            }
        }

        stage('Parallel Builds') {
            failFast true

            parallel {

                stage('ARM64 Satellite Firmware') {
                    steps {
                        sh '''
                            docker run --rm \
                              -v "$WORKSPACE:/workspace" \
                              -w /workspace \
                              ${CI_IMAGE} \
                              bash -c '
                                set -e

                                bazel build \
                                  --platforms=//toolchains/arm:arm64_platform \
                                  //firmware/satellite:satellite_firmware

                                echo
                                echo "===== ARM64 ARTIFACT ====="

                                file \
                                  bazel-bin/firmware/satellite/satellite_firmware
                              '
                        '''
                    }
                }

                stage('RISC-V User Terminal') {
                    steps {
                        sh '''
                            docker run --rm \
                              -v "$WORKSPACE:/workspace" \
                              -w /workspace \
                              ${CI_IMAGE} \
                              bash -c '
                                set -e

                                bazel build \
                                  --platforms=//toolchains/riscv:riscv64_platform \
                                  //firmware/user-terminal:user_terminal_firmware

                                echo
                                echo "===== RISC-V ARTIFACT ====="

                                file \
                                  bazel-bin/firmware/user-terminal/user_terminal_firmware
                              '
                        '''
                    }
                }

                stage('Go Telemetry Service') {
                    steps {
                        sh '''
                            docker run --rm \
                              -v "$WORKSPACE:/workspace" \
                              -w /workspace \
                              ${CI_IMAGE} \
                              bash -c '
                                set -e

                                bazel build \
                                  //services/telemetry-ingestion/...

                                echo
                                echo "===== GO BUILD COMPLETE ====="
                              '
                        '''
                    }
                }
            }
        }

        stage('Automated Tests') {
            steps {
                sh '''
                    docker run --rm \
                      -v "$WORKSPACE:/workspace" \
                      -w /workspace \
                      ${CI_IMAGE} \
                      bash -c '
                        set -e

                        echo "===== SATELLITE TESTS ====="
                        bazel test \
                          //firmware/satellite:satellite_test

                        echo
                        echo "===== GO TESTS ====="
                        bazel test \
                          //services/telemetry-ingestion/... \
                          --test_output=errors
                      '
                '''
            }
        }

        stage('Build Telemetry Container') {
            steps {
                sh '''
                    set -e

                    GO_BINARY=$(find \
                      bazel-bin/services/telemetry-ingestion \
                      -type f \
                      -executable \
                      | grep '/server_/server$' \
                      | head -1)

                    if [ -z "$GO_BINARY" ]; then
                        echo "ERROR: Go telemetry binary not found"
                        exit 1
                    fi

                    echo "Telemetry binary:"
                    echo "$GO_BINARY"

                    cp "$GO_BINARY" containers/telemetry/server

                    docker build \
                      -t ${TELEMETRY_IMAGE}:${TELEMETRY_TAG} \
                      containers/telemetry
                '''
            }
        }

        stage('Container Smoke Test') {
            steps {
                sh '''
                    set -e

                    SMOKE_NAME="terawave-telemetry-ci-${BUILD_NUMBER}"

                    docker rm -f "$SMOKE_NAME" 2>/dev/null || true

                    docker run -d \
                      --name "$SMOKE_NAME" \
                      -p 18085:8085 \
                      ${TELEMETRY_IMAGE}:${TELEMETRY_TAG}

                    cleanup() {
                        docker logs "$SMOKE_NAME" || true
                        docker rm -f "$SMOKE_NAME" || true
                    }

                    trap cleanup EXIT

                    echo "Waiting for telemetry service..."

                    for i in $(seq 1 20); do
                        if curl -fsS \
                           http://localhost:18085/health >/tmp/health.json; then
                            break
                        fi

                        sleep 1
                    done

                    echo
                    echo "===== HEALTH ====="
                    cat /tmp/health.json

                    echo
                    echo "===== TELEMETRY ====="

                    curl -fsS \
                      -X POST \
                      http://localhost:18085/telemetry \
                      -H "Content-Type: application/json" \
                      -d '{
                        "satellite_id":"SAT-CI-001",
                        "temperature_c":42.5,
                        "battery_voltage":28.4,
                        "packets_tx":1250,
                        "packets_rx":1187
                      }'

                    echo
                    echo
                    echo "===== METRICS ====="

                    curl -fsS \
                      http://localhost:18085/metrics
                '''
            }
        }

        stage('Artifact Integrity') {
            steps {
                sh '''
                    mkdir -p artifacts/evidence/jenkins

                    echo "===== ARTIFACT HASHES =====" \
                      | tee artifacts/evidence/jenkins/build-${BUILD_NUMBER}.sha256

                    find bazel-bin/firmware \
                      -type f \
                      -executable \
                      -exec sha256sum {} \\; \
                      | tee -a \
                        artifacts/evidence/jenkins/build-${BUILD_NUMBER}.sha256

                    GO_BINARY=$(find \
                      bazel-bin/services/telemetry-ingestion \
                      -type f \
                      -executable \
                      | grep '/server_/server$' \
                      | head -1)

                    sha256sum "$GO_BINARY" \
                      | tee -a \
                        artifacts/evidence/jenkins/build-${BUILD_NUMBER}.sha256
                '''
            }
        }

        stage('Push on Green') {
            steps {
                sh '''
                    echo "============================================"
                    echo " TERAWAVE PUSH-ON-GREEN GATE"
                    echo "============================================"
                    echo
                    echo "ARM64 BUILD .............. PASS"
                    echo "RISC-V BUILD ............. PASS"
                    echo "GO BUILD ................. PASS"
                    echo "AUTOMATED TESTS .......... PASS"
                    echo "CONTAINER BUILD .......... PASS"
                    echo "SMOKE TEST ............... PASS"
                    echo "ARTIFACT INTEGRITY ....... PASS"
                    echo
                    echo "RELEASE GATE: GREEN"
                    echo
                    echo "============================================"
                '''
            }
        }
    }

    post {
        always {
            sh '''
                docker rm -f \
                  terawave-telemetry-ci-${BUILD_NUMBER} \
                  2>/dev/null || true

                rm -f containers/telemetry/server
            '''
        }

        success {
            echo 'TeraWave CI pipeline completed successfully.'
        }

        failure {
            echo 'TeraWave CI pipeline failed. Release promotion blocked.'
        }
    }
}
