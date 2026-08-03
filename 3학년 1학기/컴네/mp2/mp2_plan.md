# Computer Networks - Machine Problem 2 (MP2) 수행 계획서

## 1. 과제 개요
본 과제는 
etsim2 시뮬레이터 위에서 동작하는 **분산 라우팅 프로토콜(Distributed Routing Protocol)**을 C/C++로 설계하고 구현하는 것입니다. 네트워크는 동적으로 변화(링크 비용 변경, 단절, 생성)하며, 라우터는 이웃과 제어 메시지(Control Message)를 주고받아 최적의 경로를 찾아내고 데이터 패킷을 목적지까지 성공적으로 전달해야 합니다.

## 2. 제출 및 컴파일 환경
*   **제출 파일:** 단일 소스 파일 
outer_<본인학번>.cc (예: 
outer_20221627.cc)
*   **컴파일 명령어:** g++ -O2 -o router_20221627 router_20221627.cc netsim2_lib.cc
*   **실행 명령어:** ./router_20221627 --scenario val1.scn

## 3. 평가 기준
1.  **정확성 (Correctness):** 시나리오 내의 모든 패킷이 Drop 없이 목적지에 도달해야 함. (단 하나라도 실패 시 해당 시나리오 0점)
2.  **효율성 (Efficiency):** Cost = path_cost_total + 10 * control_bytes_total
    *   path_cost_total: 패킷이 이동한 전체 경로의 링크 비용 합
    *   control_bytes_total: 라우터 간 주고받은 제어 메시지의 총 바이트 수
    *   **목표:** 정확성을 100% 달성하면서 Cost를 최소화하기 위해 제어 메시지 전송량과 최적 경로 탐색 간의 Trade-off를 잘 관리해야 함.

## 4. 제약 사항 (주의점)
*   **독립된 인스턴스:** 각 라우터 인스턴스는 격리되어 동작함. 전역(Global) 변수나 정적(Static) 변수를 사용하여 라우터 간 상태를 공유하면 **0점 처리**됨. (모든 상태는 RouterState 구조체 내에 보관)
*   단일 스레드로만 동작해야 함 (멀티스레드, fork 등 금지).
*   stdout, stdin 사용 금지 (디버깅용 출력은 stderr만 사용).
*   외부 라이브러리 사용 금지 (표준 C/C++ 라이브러리만 허용).

## 5. 구현해야 할 핵심 API (콜백 함수)
*   
outer_init(): 라우터 초기화, RouterState 할당 및 초기 이웃/링크 비용 정보 저장.
*   on_link_change(): 주변 링크 상태(비용 변경, 생성, 단절) 변화 시 호출됨. 라우팅 테이블을 업데이트하고 필요시 이웃에게 변경사항 전파.
*   on_control(): 이웃 라우터로부터 제어 메시지를 수신했을 때 호출됨. 메시지를 파싱하여 라우팅 정보를 업데이트.
*   on_packet(): 데이터 패킷 도착 시 호출됨. 라우팅 테이블을 참조하여 다음 Hop(이웃 노드 ID)을 반환.
*   on_timer(): schedule_wakeup()으로 예약한 시간에 호출됨. 주기적인 메시지 전송이나 Timeout 처리에 사용.
*   
outer_shutdown(): 시뮬레이션 종료 시 호출됨. 할당된 메모리 해제.

## 6. 라우팅 프로토콜 설계 (핵심 결정 사항)
Distance Vector(DV) 또는 Link State(LS) 알고리즘 중 하나를 선택하거나 변형해야 합니다.
*   **Distance Vector (DV):**
    *   장점: 구현이 비교적 간단하고, 상태(메모리)를 적게 차지함.
    *   단점: Count-to-Infinity 문제, 루프(Loop) 발생 가능성 존재. 이를 해결하기 위해 Split Horizon, Poison Reverse 등의 추가 기법이 필수적임.
*   **Link State (LS):**
    *   장점: 전체 토폴로지를 파악하므로 변화에 빠르게 대응하고 루프를 방지하기 쉬움.
    *   단점: 모든 노드에 Link State 정보를 Flooding해야 하므로 control_bytes_total 증가율이 높을 수 있음. (효율성 측면에서 패널티)
*   **설계 전략 제안:** 구현 복잡도가 낮고 제어 메시지 크기를 최적화하기 좋은 **Distance Vector (DV)** 방식을 기반으로 하되, 변화 발생 시 즉각 전파하는 **Triggered Update**를 결합하는 방식을 1차 목표로 삼습니다. (또는 시퀀스 넘버를 도입한 DSDV 방식 고려)

## 7. 단계별 개발 계획

### Step 1: 기본 구조 셋업
*   
outer_<학번>.cc 파일 생성 및 
etsim2.h 포함.
*   RouterState 구조체 정의: 본인 ID, 라우팅 테이블 (목적지 -> 다음 Hop, 총 비용), 이웃 링크 상태 등 저장.
*   모든 필수 콜백 함수의 기본 뼈대(Dummy 구현) 작성 및 컴파일 테스트.

### Step 2: 초기화 및 이웃 간 통신 구현
*   
outer_init에서 초기 이웃 상태 저장.
*   send_control()을 이용해 자신의 라우팅 정보(또는 링크 상태)를 이웃에게 보내는 헬퍼 함수 작성.
*   on_control()에서 수신된 메시지를 파싱하는 로직 구현.

### Step 3: 라우팅 알고리즘(DV/LS) 코어 구현
*   자신의 라우팅 테이블을 최적화하는 로직 구현.
*   Bellman-Ford 알고리즘(DV) 또는 Dijkstra 알고리즘(LS) 적용.
*   on_packet()에서 라우팅 테이블을 룩업하여 올바른 next hop 반환하도록 구현.

### Step 4: 동적 변화 대응 구현
*   on_link_change()에서 링크 단절(-1) 및 비용 변경 시 라우팅 테이블을 즉시 갱신.
*   갱신된 정보를 이웃에게 알리기 위한 Triggered Update 전송 로직 추가.

### Step 5: 타이머 및 안정화 기법 적용
*   schedule_wakeup()과 on_timer()를 활용해 주기적(Periodic) 업데이트 전송 로직 구현 (필요 시).
*   Routing Loop 방지 (Split Horizon, Poison Reverse 등) 적용.

### Step 6: 테스트 및 최적화
*   제공된 4개의 Validation 시나리오(val1.scn ~ val4.scn) 테스트.
*   패킷 Drop이 발생하는지(SUCCESS 확인) 체크.
*   Control Message 패킷 포맷을 압축/최적화하여 전송되는 Byte 크기(control_bytes_total)를 줄이기. (예: 변경된 부분만 전송, 구조체 패딩 최소화)
*   불필요한 주기적 업데이트 빈도를 줄이고 이벤트 기반(Event-driven) 업데이트 위주로 전환.

## 8. 당장 시작할 일
1. 
outer_20221627.cc (또는 본인 학번 기준) 파일을 생성한다.
2. 
etsim2.h를 기반으로 RouterState 구조체와 빈 콜백 함수들을 작성하여 컴파일 환경이 정상인지 확인한다.
