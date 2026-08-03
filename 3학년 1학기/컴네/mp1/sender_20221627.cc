#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "netsim.h"

using namespace std;

// CRC-32 테이블 (표준 다항식: 0x04C11DB7)
uint32_t crc32_table[256];

// CRC-32 테이블 만들기
// 현재 임시 나머지 32비트에서, 최상위 비트 8비트가 무엇이냐에 따라, 해당 8비트 계산 후 남은 32비트에 미칠 영향 계산.
void init_crc32_table() {
    for (int i = 0; i < 256; i++) {
        uint32_t crc = i << 24;
        for (int j = 0; j < 8; j++) {

            // 맨 앞 비트가 1 이면 나누기 시행
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ 0x04C11DB7;
            } 
            
            // 맨 앞 비트가 0이 아니면 나누기 안함
            else {
                crc = (crc << 1);
            }
        }
        crc32_table[i] = crc;
    }
}

// CRC-32 계산
uint32_t calculate_crc32(const uint8_t* data, size_t len) {
    uint32_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc = (crc << 8) ^ crc32_table[(crc >> 24) ^ data[i]];
    }
    return crc;
}

// BER 추정 및 최적 페이로드 크기 계산 함수
double estimate_ber(uint64_t naks, uint64_t bits) {
    // Laplace smoothing을 적용하여 초기 추정치를 보수적(1e-3)으로 잡음
    return (naks + 0.5) / (bits + 500.0);
}

double estimate_ber_better(uint64_t naks, uint64_t total_frames, size_t P) {
    // 1. 데이터가 없을 때의 초기값 (보수적 1e-3 설정)
    if (total_frames == 0) return 0.001;

    // 2. 프레임 에러율(P_fe) 계산
    // Laplace Smoothing을 적용하여 관측되지 않은 에러에 대한 가능성 열어둠
    double p_fe = (double)(naks + 0.5) / (total_frames + 1.0);

    // 3. 에러가 발생할 수 있는 취약 영역(L) 계산 (비트 단위)
    // 명세서 조건: PAYLOAD(P) + CRC(4) 영역에서만 에러 발생
    double L = (double)(P + 4) * 8.0;

    // 4. 역산 공식 적용
    // P_fe = 1 - (1 - BER)^L  ==>  BER = 1 - (1 - P_fe)^(1/L)
    double estimated_ber = 1.0 - std::pow(1.0 - p_fe, 1.0 / L);

    // 5. 수치적 안정성을 위한 클램핑 (0 ~ 1 사이 유지)
    return std::max(0.0, std::min(1.0, estimated_ber));
}

size_t get_optimal_P(double ber) {
    size_t best_P = 100; 
    double best_cost = 1e18;
    
    // 1바이트부터 65535바이트까지 모든 크기에 대해 예상 Cost를 계산
    for (size_t P = 1; P <= 65535; P++) {
        double lambda = ber * 8.0 * (P + 4.0); // 프레임에서 오류가 발생할 평균 비트 수
        if (lambda > 15.0) continue; // 재전송이 너무 많아질 크기는 스킵
        
        double expected_attempts = exp(lambda); // 성공할 때까지의 예상 전송 횟수
        double cost_per_byte = (P + 256.0) * expected_attempts / (double)P;
        
        if (cost_per_byte < best_cost) {
            best_cost = cost_per_byte;
            best_P = P;
        }
    }
    return best_P;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    init_crc32_table();

    // 로그 파일 열기 (append 모드)
    FILE* log_file = fopen("output.md", "a");
    if (!log_file) {
        cerr << "Error: Cannot open output.md for logging." << endl;
        return 1;
    }
    fprintf(log_file, "## Transmission Log for %s\n", argv[1]);
    fprintf(log_file, "| Attempt | Offset | Size | Result |\n");
    fprintf(log_file, "| :--- | :--- | :--- | :--- |\n");

    // 파일 읽기
    ifstream file(argv[1], ios::binary | ios::ate);
    if (!file) {
        cerr << "Error: Cannot open input file." << endl;
        return 1;
    }
    
    streamsize file_size = file.tellg();
    file.seekg(0, ios::beg);
    
    vector<uint8_t> data(file_size);
    if (file_size > 0) {
        if (!file.read(reinterpret_cast<char*>(data.data()), file_size)) {
            cerr << "Error: Cannot read input file." << endl;
            return 1;
        }
    }

    size_t offset = 0;
    
    uint64_t total_naks = 0;
    uint64_t total_frames = 0; 
    uint64_t attempt_count = 0; // 로그용 전송 시도 횟수
    size_t last_p = 512;       
    
    vector<uint8_t> frame;

    while (offset < file_size) {
        size_t remaining = file_size - offset;
        
        double current_ber = estimate_ber_better(total_naks, total_frames, last_p);
        size_t optimal_p = get_optimal_P(current_ber);
        size_t current_p = min(optimal_p, remaining);
        
        frame.resize(2 + current_p + 4);

        frame[0] = (current_p >> 8) & 0xFF;
        frame[1] = current_p & 0xFF;
        
        copy(data.begin() + offset, data.begin() + offset + current_p, frame.begin() + 2);
        
        uint32_t crc = calculate_crc32(frame.data(), 2 + current_p);
        
        frame[2 + current_p] = (crc >> 24) & 0xFF;
        frame[3 + current_p] = (crc >> 16) & 0xFF;
        frame[4 + current_p] = (crc >> 8) & 0xFF;
        frame[5 + current_p] = crc & 0xFF;

        // 전송 및 결과 처리
        int result = send_frame(frame.data(), frame.size());
        total_frames++;
        attempt_count++;
        
        // 로그 출력 (output.md 파일에 Markdown 테이블 형식으로 기록)
        fprintf(log_file, "| %lu | %zu | %zu | ", attempt_count, offset, current_p);

        if (result == NETSIM_ACK) {
            fprintf(log_file, "ACK |\n");
            fflush(log_file);
            last_p = current_p;
            offset += current_p;
        } else if (result == NETSIM_NAK) {
            fprintf(log_file, "NAK (Adaptive sizing) |\n");
            fflush(log_file);
            last_p = current_p;
            total_naks++;
        } else {
            fprintf(log_file, "ERROR |\n");
            fflush(log_file);
            fclose(log_file);
            cerr << "Error: netsim communication error." << endl;
            return 1;
        }
    }

    fclose(log_file);
    return 0;
}