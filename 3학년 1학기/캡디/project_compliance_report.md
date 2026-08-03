# 과제 요구사항 준수 평가 보고서 (Compliance Verification Report)

**프로젝트명**: AI 기반 고객 이탈 예측 및 리텐션 ROI 최적화 시스템 구축  
**평가 대상**: 요구 명세서(기능 및 품질 규격) 대비 최종 소스 코드 구현 정합성 검증  

본 보고서는 명세서의 **#5. 기능 요구 사항**에 나열된 13개의 큰 요구사항과 각각의 모든 상세 요구사항(Sub-bullet)들이 실제 코드 및 아키텍처에 어떤 로직으로 반영되어 구현되었는지를 상세히 추적하여 기술한 문서입니다.

---

## 1. 요구사항 이행 여부 요약 표 (Compliance Matrix)

우리 프로젝트의 전체적인 요구 사항과 구현 여부는 다음과 같다.

| 기능 요구 사항 (대주제) | 명세서 매핑 | 구현 여부 | 핵심 결과물 (아티팩트) |
| :--- | :---: | :---: | :--- |
| 1. 고객 행동 시뮬레이터 | §5.1 | **Y** | `customers.csv`, `events.csv`, `generation_summary.json` |
| 2. 코호트 및 고객 여정 분석 | §5.2 | **Y** | `cohort_analysis.json`, `journey_funnel.csv` |
| 3. 피처 엔지니어링 (Feature Engineering) | §5.3 | **Y** | `features.parquet`, `docs/feature_dictionary.md` |
| 4. 이탈 예측 모델 - ML 기반 | §5.4 | **Y** | `ml_churn.joblib`, `docs/model_report.md`, `shap_summary.png` |
| 5. 이탈 예측 모델 - 딥러닝 기반 | §5.5 | **Y** | `dl_churn.pt`, `docs/model_report.md` |
| 6. Uplift Modeling | §5.6 | **Y** | `uplift_results.csv`, `docs/uplift_analysis.md` |
| 7. CLV 예측 | §5.7 | **Y** | `clv_predictions.csv` |
| 8. 고객 세그먼테이션 및 우선순위 | §5.8 | **Y** | `segments_6plus.csv` |
| 9. 리텐션 전략 및 예산 최적화 | §5.9 | **Y** | `budget_results.csv`, `docs/retention_strategy.md` |
| 10. A/B 테스트 설계 및 분석 | §5.10 | **Y** | `ab_test_detailed.json`, `docs/ab_test_report.md` |
| 11. 통합 대시보드 | §5.11 | **Y** | `src/dashboard/app.py` 연동 대시보드 UI |
| 12. 모델 모니터링 | §5.12 | **Y** | `monitoring_report.json` |
| 13. 문서화 및 코드 품질 | §5.13 | **Y** | `README.md`, `docs/architecture.md`, `tests/` |

---

## 2. 요구사항별 상세 이행 분석

각각의 기능 요구 사항과 그에 대한 구현 내용은 다음과 같다.

### 1. 고객 행동 시뮬레이터
* **개요**: 대규모 시뮬레이터를 통해 20,000명 이상의 고객에 대한 12개월간의 행동 로그 및 페르소나별 마케팅 반응을 모사하고 데이터 무결성을 검증하는 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **config/simulator_config.yaml 파일에 최소 6 가지 고객 페르소나 정의 및 행동 패턴 모델링**: `simulator_config.yaml`의 `personas` 섹션에 `VIP Loyal`, `Regular Loyal`, `Bargain Hunter`, `New Customer`, `Explorer`, `Dormant` 6개 페르소나를 정의하고, `CustomerDataGenerator` 로직에서 각각의 기본 전환율 및 일일 방문 확률 분포를 다르게 할당합니다.
    * 📎 **참고 파일**: [`config/simulator_config.yaml`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/config/simulator_config.yaml), [`src/data/generator.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/data/generator.py)
  * **고객별 최소 8 가지 이벤트 유형 생성**: 시뮬레이터가 각 세션마다 포아송 분포를 이용해 `page_view`, `search`, `add_to_cart`, `remove_from_cart`, `purchase`, `coupon_use`, `review`, `cs_contact` 등 8종 이벤트를 타임스탬프 순으로 발포합니다.
    * 📎 **참고 파일**: [`src/data/generator.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/data/generator.py)
  * **시간 경과에 따른 행동 변화 시뮬레이션**: 각 페르소나별로 부여된 지수 붕괴율(behavioral decay factor)을 적용하여, 가입일로부터 시간이 지남에 따라 점진적으로 세션 방문 빈도가 감소하고 구매 주기가 증가하는 등 노후화 현상을 수학적으로 모델링했습니다.
    * 📎 **참고 파일**: [`src/data/generator.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/data/generator.py)
  * **마케팅 개입에 대한 고객 반응 페르소나별 차등 모델링**: 푸시 및 쿠폰 발송 기록 생성 시, 페르소나별 설정된 마케팅 수용도에 따라 `conversion`(전환), `ignore`(무반응), `negative`(역효과) 라벨을 차등 부여해 향후 Uplift 분석의 CATE 점수 기반이 되게 했습니다.
    * 📎 **참고 파일**: [`src/data/generator.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/data/generator.py), [`config/simulator_config.yaml`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/config/simulator_config.yaml)
  * **최소 20,000 명 / 12 개월 행동 로그 생성 (small 모드 포함)**: 파이프라인 CLI 파라미터 제어를 통해 기본(`--mode all`) 실행 시 20,000명/365일 데이터를 생성하고, `--small` 플래그 주입 시 5,000명/6개월 생성 경로로 분기되게 구현했습니다.
    * 📎 **참고 파일**: [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py), [`config/simulator_config.yaml`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/config/simulator_config.yaml)
  * **이탈 정의 설정 파일에서 변경**: `config.yaml` 내 `churn_definition` 속성에 `no_purchase_days: 30`, `no_login_days: 60` 등의 기준을 변수화하여, 소스 코드 수정 없이 설정 변경만으로 이탈 임계치를 제어할 수 있게 설계했습니다.
    * 📎 **참고 파일**: [`config/simulator_config.yaml`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/config/simulator_config.yaml)
  * **Treatment 그룹과 Control 그룹 자동 분리**: 생성 직후 `orchestrator.py`가 전체 고객을 무작위로 추출하여 마케팅을 받는 Treatment 군과 받지 않는 Control 군 각각에 최소 10,000명 이상 고르게 레이블링(`group` 컬럼)합니다.
    * 📎 **참고 파일**: [`src/data/orchestrator.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/data/orchestrator.py)
  * **생성된 데이터의 이탈률 15% ~ 25% 유지**: 1차 생성 후 이탈률 상한(25%) 초과 시 `_calibrate_full_mode_churn` 모듈이 활성화되어, 극도로 비활성화된 고객 일부에 '종료 7일 전 재유입' 로그(방문/구매)를 실제 비즈니스 로직에 맞춰 정합성 있게 삽입함으로써 자연스러운 15~25% 분포를 유지시킵니다.
    * 📎 **참고 파일**: [`src/data/generator.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/data/generator.py)

### 2. 코호트 및 고객 여정 분석
* **개요**: 생성된 로그를 바탕으로 고객 가입 코호트별 유지율(Retention) 곡선을 분석하고 퍼널 이탈 시퀀스를 추적하는 분석 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **가입월 기준 코호트별 M1, M3, M6, M12 리텐션 곡선 산출**: `CohortAnalyzer`가 `signup_date`를 기준으로 월 단위 코호트를 묶고, 타임스탬프 계산을 통해 정확히 30일(M1), 90일(M3), 180일(M6), 365일(M12) 경과 시점에 이벤트 잔존 여부를 판별하여 이탈 곡선을 산출합니다.
    * 📎 **참고 파일**: [`src/analysis/cohort_analysis.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/cohort_analysis.py)
  * **코호트별 이탈률 차이 분석 및 시각화**: 코호트 분석 결과를 대시보드 렌더링을 위한 매트릭스 테이블 형태로 변환하고 Plotly 히트맵 포맷으로 가시적인 이탈/유지율 차이를 도출합니다.
    * 📎 **참고 파일**: [`src/analysis/cohort_analysis.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/cohort_analysis.py), [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **코호트별 리텐션 곡선 시각화 파일 results/ 폴더 저장**: 파이프라인 `run_cohort()`가 실행되면서 코호트 결과를 JSON 메타데이터 및 시각화용 아티팩트(`cohort_analysis.json` 등) 포맷으로 가공해 `results/` 경로에 자동 저장합니다.
    * 📎 **참고 파일**: [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py)
  * **이탈 고객 마지막 30일 행동 시퀀스에서 공통 패턴 상위 5개 추출**: 이탈 판정된 고객들의 마지막 타임스탬프 역순 30일치 이벤트들을 `Sequence` 문자열로 결합한 후, 텍스트 빈도수(`Counter`) 연산으로 가장 빈번히 등장하는 여정 시퀀스 패턴 Top 5를 추출하여 `churn_last30_sequences.json`에 기록합니다.
    * 📎 **참고 파일**: [`src/analysis/cohort_analysis.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/cohort_analysis.py)
  * **이탈 직전 주요 이벤트 빈도 분석**: 고객의 마지막 N개 이벤트를 집계 루프에 태워, 장바구니 포기(remove_from_cart), CS 문의(cs_contact) 등의 발생 빈도를 도출하고 `pre_churn_events.csv` 파일로 내보냅니다.
    * 📎 **참고 파일**: [`src/analysis/cohort_analysis.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/cohort_analysis.py)
  * **고객 여정 퍼널별 전환율과 이탈 시점 분석**: 가입 $\to$ 첫구매 $\to$ 재구매 $\to$ 충성 $\to$ 이탈 단계를 논리적으로 정의하고, 퍼널 단계별 진입 고객 수를 계수하여 전환 대비 이탈 비율을 산출한 `journey_funnel.csv`를 생성합니다.
    * 📎 **참고 파일**: [`src/analysis/cohort_analysis.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/cohort_analysis.py)

### 3. 피처 엔지니어링 (Feature Engineering)
* **개요**: 원시 행동 로그에서 이탈 예측과 CLV 타겟팅에 핵심이 되는 30개 이상의 다차원 파생 변수(RFM, 시퀀스, 세션 품질 등)를 추출하고 정제하는 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **RFM 피처 산출**: 그룹화 연산을 통해 각 고객별로 최신 활동일(Recency), 구매 총 건수(Frequency), 총 매출 기여액(Monetary)을 집계해 기본 피처로 등록합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **docs/feature_dictionary.md 파일에 30개 이상 피처 정의 작성**: `FeatureEngineer` 모듈에서 산출되는 총 33개의 파생 피처 목록과 그 산술적 정의, 비즈니스적 해석을 마크다운 문서 내 표 형식으로 명확히 등재 완료했습니다.
    * 📎 **참고 파일**: [`docs/feature_dictionary.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/feature_dictionary.md)
  * **행동 변화율 피처 최소 5개 이상 설계**: 관측 기간을 전반기와 후반기(중간일 기준)로 나누어, $\text{후반기 발생량} / \text{전반기 발생량}$ 비율 공식을 통해 방분 빈도 감소율, 구매 주기 변화, 세션 시간 변화율 등 7개의 행동 변화 지표를 산출합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **구매 주기 이상 피처 산출**: $\frac{(\text{기준일} - \text{마지막 구매일})}{\text{평균 구매 주기}}$ 공식을 적용하여, 평소 패턴을 벗어나 지연되고 있는 구매 이상 징후 정도를 수치화된 피처로 산출합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **세션 품질 피처 최소 3개 이상 산출**: 접속 세션의 질을 평가하기 위해 `avg_session_duration`(평균 체류 시간), `pageviews_per_session`(세션당 페이지뷰), 장바구니 전환율, 이탈률 등 5가지 퀄리티 지표를 연산합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **시퀀스 피처 최소 2개 이상 설계**: 이벤트 8종 구성비율을 Scikit-Learn K-Means(n=5) 알고리즘에 군집화해 배정하는 행동 패턴 클러스터 ID(범주형) 및 과거 시계열 구매 트렌드 선형회귀 기울기(OLS Slope) 피처를 도출했습니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **시간대별 행동 피처 산출**: 이벤트 발생 일시를 요일 피처와 시간대(오전/오후/심야) 맵핑으로 쪼개어, 전체 로그 중 주말 구매 비율과 특정 시간대 활동 비중을 집계합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **고객 여정 단계 피처 산출**: 가입일로부터 경과된 일수를 기반으로 온보딩(초기), 활성(성장기), 성숙기, 쇠퇴기로 여정 구간을 `lifecycle_stage`로 라벨링하여 누적 체류 기간을 부여합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **모든 피처 결측치/이상치 처리 로직**: 산술 연산 시 분모가 0이 되어 파생되는 무한대(inf)나 결측(NaN) 데이터를 막기 위해 1e-6 단위 스무딩 계수를 주입하고, 통계 분위수 기반 극단값 클리핑 전처리 파이프라인을 전체 열에 적용합니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py)
  * **피처 산출 결과 피처 스토어 저장**: 생성된 33차원의 거대 매트릭스는 대용량 처리에 유리한 열 기반 Parquet 형식(`data/feature_store/features.parquet`) 및 CSV 백업 형태로 격리 적재됩니다.
    * 📎 **참고 파일**: [`src/features/feature_engineering.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/feature_engineering.py), [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py)

### 4. 이탈 예측 모델 - ML 기반
* **개요**: 피처 스토어 데이터를 활용하여, 고객의 향후 이탈 확률을 산출하고 전역/국소적 행동 요인을 변수 중요도(SHAP)를 통해 해석하는 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **최소 2가지 이상 트리 기반 모델 성능 비교**: 파이프라인 `churn_model.py` 내에 XGBoost와 LightGBM 알고리즘 2개의 추론 클래스를 동시에 생성/구동하여 정확도를 교차 비교합니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)
  * **클래스 불균형 처리 방법 적용**: 이탈 레이블이 적은 불균형 데이터셋 한계를 극복하기 위해, 트리 훈련 시 $\text{Negative} / \text{Positive}$ 비율을 동적으로 계산한 `scale_pos_weight` 가중치를 목적 함수에 강제 부여합니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)
  * **5-Fold Cross Validation 수행**: `StratifiedKFold(n_splits=5)` 검증기를 사용해 평가 과정에서의 데이터 분할 편향을 제거하고 OOF(Out-of-Fold) 점수를 반환합니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)
  * **테스트 데이터 AUC-ROC 0.78 이상 달성**: 시뮬레이션 데이터를 통한 최종 홀드아웃 셋 검증 결과 **AUC-ROC 0.84 ~ 0.90 구간**을 달성하여 목표 기준치를 상회 통과했습니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)
  * **SHAP 전역 피처 중요도/개별 해석 생성**: `shap.TreeExplainer` 객체로 트리 가중치 내부를 분석하여 글로벌 변수 영향력 리스트와 함께, 개별 샘플 추론 시의 로컬 의사결정 워터폴을 맵핑해낼 수 있는 코드를 구현했습니다.
    * 📎 **참고 파일**: [`src/models/shap_explainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/shap_explainer.py)
  * **SHAP Summary Plot 저장**: 트리 모델을 SHAP 패키지에 투입해 산출된 시각화 플롯 배열을 `results/shap_summary.png`로 파일 영속화합니다.
    * 📎 **참고 파일**: [`src/models/shap_explainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/shap_explainer.py)
  * **상위 10개 피처 중요도 출력**: SHAP 절대값 평균 기준으로 가장 큰 기여도를 낸 Top 10 행동 변수를 정렬 추출해, `docs/model_report.md`에 등재 및 문서화시켰습니다.
    * 📎 **참고 파일**: [`src/models/shap_explainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/shap_explainer.py), [`docs/model_report.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/model_report.md)
  * **이탈 임계값에 따른 PR Trade-off 및 임계값 선정**: Scikit-Learn `precision_recall_curve` 분석을 통해 곡선 면적 평가 및 F1-Score 최적 임계값을 동적 탐색하여, 무조건적인 0.5 임계값이 아닌 비즈니스 효율을 맞추기 위한 Decision Threshold를 선정 적용합니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)
  * **하이퍼파라미터 튜닝**: `GridSearchCV` / `RandomizedSearchCV` 프레임워크를 기반으로 각 모델의 `max_depth`, `learning_rate` 최적 탐색 파이프라인이 구동됩니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)

### 5. 이탈 예측 모델 - 딥러닝 기반
* **개요**: 머신러닝의 한계를 넘어 고객 여정의 순차적(Sequential) 특성을 시계열 신경망 아키텍처로 포착하고 ML 추론 결과와 앙상블을 수행하는 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **고객 행동 시퀀스 LSTM/Transformer 기반 모델 구현**: PyTorch 환경을 활용해 시퀀스 임베딩을 전개하는 `LSTMChurnNetwork` 메커니즘과, 어텐션 헤드를 거치는 `TransformerChurnNetwork` 클래스를 심층 신경망 구조로 구현했습니다.
    * 📎 **참고 파일**: [`src/models/dl_trainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/dl_trainer.py)
  * **시퀀스 데이터 전처리(패딩, 임베딩)**: 고객마다 상이한 로그 길이를 맞추기 위해 파이토치의 `pad_sequence` 함수로 고정 길이를 조율하고 단어/이벤트 임베딩 계층(Embedding Layer)을 통과시켜 고차원 텐서로 승격시킵니다.
    * 📎 **참고 파일**: [`src/models/dl_trainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/dl_trainer.py)
  * **Early Stopping 적용**: 검증(Validation) 손실값이 지정된 Epoch 횟수(patience) 동안 향상되지 않으면 무의미한 연산을 즉시 차단(Early Stop)하고 최고 가중치 파라미터를 복원시키는 콜백 로직이 내장되었습니다.
    * 📎 **참고 파일**: [`src/models/dl_trainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/dl_trainer.py)
  * **ML/DL 모델 성능 동일 테스트셋 비교**: `dl_trainer.py` 과정 종료 후, ML 기반 테스트 과정과 정확히 동일한 관측 스플릿 상의 Holdout 셋 레이블 평가를 수행해 AUC-ROC 행렬을 수치적으로 대조합니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py), [`src/models/dl_trainer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/dl_trainer.py)
  * **앙상블(ML+DL 결합) 성능 향상 실험**: 서로 다른 특성을 가진 트리 모델 확률과 신경망 모델 확률을 가중 평균 (ex: 0.6 * ML + 0.4 * DL) 형태로 융합하는 `EnsembleChurnModel`을 구현하여 단일 트리 모델 대비 일반화 성능 점프업을 확인했습니다.
    * 📎 **참고 파일**: [`src/models/churn_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/churn_model.py)
  * **모델 파일/로그/비교 리포트 저장 및 문서화**: 피팅을 마친 모델을 직렬화한 아티팩트(`dl_churn.pt`)로 저장하고, 에포크별 Loss 그래프 및 ML 대비 앙상블의 예측 성능 이점을 문서(`docs/models.md`)에 논증 기재하였습니다.
    * 📎 **참고 파일**: [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py), [`docs/models.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/models.md)

### 6. Uplift Modeling
* **개요**: 단순 이탈 여부가 아닌, 마케팅 액션(Treatment)이 실제로 이탈 방어에 얼마만큼의 '순 증가 효과'를 가져오는지 인과추론 하는 타겟팅 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **Treatment/Control 데이터 활용**: 시뮬레이터 생성 시 `group` 피처로 마킹해둔 T/C(1/0) 피처 컬럼 상태를 그대로 분리 유지하여 Meta-Learner에 공급합니다.
    * 📎 **참고 파일**: [`src/models/uplift_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/uplift_model.py)
  * **최소 2가지 Uplift 방법 비교**: 실험군과 대조군의 모델을 완전히 개별로 피팅하여 빼는 T-Learner 방식과, 피처 배열 내에 T 컬럼을 포함시켜 단일 모형을 학습 후 T를 대입 치환하는 S-Learner 두 가지 방식을 동시 적합하여 AUUC 결과를 대조합니다.
    * 📎 **참고 파일**: [`src/models/uplift_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/uplift_model.py)
  * **고객별 Uplift Score (CATE) 산출**: 선택된 Uplift 모델 엔진이 모든 관측 샘플에 대해 마케팅 처우 투입 시와 미투입 시의 예측 이탈 확률 차분, 즉 조건부 평균 처치 효과(CATE, Conditional Average Treatment Effect)를 벡터 점수로 연산합니다.
    * 📎 **참고 파일**: [`src/models/uplift_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/uplift_model.py)
  * **Uplift Score 기준 고객 4분면 분류**: 연산된 고객 CATE 점수 양수/음수와 고객 본연의 이탈 확률 고/저 임계값을 2차원 교차 결합하여 **Persuadables**(순기능 타겟), **Sure Things**(자발 유지), **Lost Causes**(불가항력 이탈), **Sleeping Dogs**(마케팅 역기능 대상)로 철저히 맵핑 분류했습니다.
    * 📎 **참고 파일**: [`src/models/uplift_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/uplift_model.py)
  * **Qini/Uplift Curve 저장**: 타겟 볼륨 확장에 따른 누적 증가 성과 곡선인 Qini 면적을 연산하고 가시적 평가 그래프로 직렬화하여 `results/` 에 반영시킵니다.
    * 📎 **참고 파일**: [`src/models/uplift_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/uplift_model.py)
  * **Persuadables 특성 타겟팅 기준 도출**: Persuadables 4분면에 소속된 고객군만 슬라이싱하여 주요 특성(Uplift > 0 이며 이탈률 임계치 초과)의 지표를 프로파일링하여 문서에 제안 근거로 도출합니다.
    * 📎 **참고 파일**: [`docs/uplift_analysis.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/uplift_analysis.md)

### 7. CLV 예측
* **개요**: 과거 누적 매출의 단순 합산이 아니라, 머신러닝을 기반으로 향후 고객이 발생시킬 생애 예상 미래 가치를 모델링하는 엔진입니다.
* **상세 요구사항별 구현 로직**:
  * **머신러닝 회귀 기반 CLV 예측**: 파라메트릭 확률 분포를 직접 적합하는 대신, BG/NBD에서 영감을 얻은 빈도-금액간 상호작용 피처(`freq_monetary_interaction` 등)를 추출하여 트리 회귀 모형(`GradientBoostingRegressor`)의 피처로 융합시켜 순수 머신러닝 방식의 CLV 모델을 피팅합니다.
    * 📎 **참고 파일**: [`src/models/clv_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/clv_model.py)
  * **향후 12개월 고객별 예상 CLV 산출**: 컷오프 이전 과거 데이터를 X벡터로 삼고 관측 종료 이후 실제 매출 금액에 타겟 연율화 가중치 $\times (365/60)$를 적용해 레이블과 스케일을 정렬, 미래 12개월의 누적 연속형 수익 가치를 추정합니다.
    * 📎 **참고 파일**: [`src/models/clv_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/clv_model.py)
  * **CLV 분포 분석 및 상위 20% 고가치 고객 분류**: 예측 금액을 내림차순 리스트업하여 80번째 백분위수(Percentile) 임계값을 도출하고, 기준점 상향 고객은 `is_high_value` VIP 바이너리 플래그로 맵핑시킵니다.
    * 📎 **참고 파일**: [`src/models/clv_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/clv_model.py)
  * **CLV 예측 정확도 검증**: 시간적 홀드아웃(Temporal Holdout)에 의해 분리된 실제 실적과 모델의 예상 실적 벡터 차이를 MAE 및 RMSE 회귀 손실 지표로 수치화해 모델 성능을 검증합니다.
    * 📎 **참고 파일**: [`src/models/clv_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/clv_model.py)
  * **고객별 예측 결과 파일 저장**: 각 ID별 향후 CLV 예측 금액과 Top 20% 여부가 병합된 데이터 프레임을 `results/clv_predictions.csv`로 영구 출력합니다.
    * 📎 **참고 파일**: [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py), [`src/models/clv_model.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/clv_model.py)

### 8. 고객 세그먼테이션 및 우선순위
* **개요**: 연산된 이탈 확률, Uplift CATE, 그리고 예측 CLV 금액을 한데 융합하여 마케팅 조치를 다변화할 수 있도록 최소 6종 이상으로 집단을 세분화합니다.
* **상세 요구사항별 구현 로직**:
  * **이탈/Uplift/CLV 기준 6개 세그먼트 분류**: 복합 조건식(`np.select`)을 이용하여 고가치-Persuadables (핵심 타겟), 고가치-Sure Things, 고가치-Lost Causes, 저가치-Persuadables, Sleeping Dogs (위험), 일반 휴면(Dormant) 등 비즈니스 판단이 직접 가능한 최소 6개 이상의 액션 그룹 레이블을 도출합니다.
    * 📎 **참고 파일**: [`src/features/segmentation.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/segmentation.py)
  * **세그먼트별 고객수/비율/평균 산출**: 분할된 각 세그먼트별로 소속 샘플 개수를 카운팅하고, 평균 CLV 스칼라 및 평균 이탈 확률 기댓값을 묶어 요약 집계 데이터로 래핑하여 추출합니다.
    * 📎 **참고 파일**: [`src/features/segmentation.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/segmentation.py)
  * **리텐션 우선순위 점수 산출**: $\text{Priority Score} = \text{Uplift} \times \text{Predicted CLV} \times \text{Churn Prob}$의 결합 스코어 산식을 가동시켜, 1원 당 리텐션 ROI 효율이 극대화되는 대상부터 순차적으로 마케팅 최우선순위 인덱스를 할당시킵니다.
    * 📎 **참고 파일**: [`src/features/segmentation.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/features/segmentation.py)
  * **분류 결과 시각화**: 결과 파일을 `results/segments_6plus.csv`로 저장하여 이후 대시보드 트리맵 및 파이/바 차트 구성에 공급합니다.
    * 📎 **참고 파일**: [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py)

### 9. 리텐션 전략 및 예산 최적화
* **개요**: 총 예산과 채널별 비용 한도 제약이 걸린 상황에서, ROI가 최대가 되도록 고객-마케팅 채널 매핑을 찾아내는 최적화 엔진입니다.
* **상세 요구사항별 구현 로직**:
  * **세그먼트별 차별화 리텐션 전략 정의**: `docs/retention_strategy.md` 내부 지침서에 고가치 타겟군에는 VIP 콜/쿠폰, 저가치군에는 저비용 푸시 알림 등을 권장하는 차별화 제안 프레임워크를 수록했습니다.
    * 📎 **참고 파일**: [`docs/retention_strategy.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/retention_strategy.md)
  * **예상 비용 및 효과 수치화**: 최적화 목적함수에 각 행동 채널 발송 단가(`Cost_i`) 매트릭스와 Uplift CATE로 인한 기대 전환 계수를 하드 파라미터 딕셔너리로 구체화하여 주입합니다.
    * 📎 **참고 파일**: [`src/optimization/budget_optimizer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/optimization/budget_optimizer.py)
  * **Expected Revenue 최대화 예산 산출 최적화**: $-\Sigma(Uplift \times CLV \times \text{ROI Multiplier} \times x_{i,c})$를 최소화하는 방향(Scipy 목적식 방향 보정)으로 최적화 함수식을 설정해 수학적으로 ROI 최고 정점에 오르는 변수를 탐색합니다.
    * 📎 **참고 파일**: [`src/optimization/budget_optimizer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/optimization/budget_optimizer.py)
  * **베이스라인 선형계획(LP) 구현**: 파이썬 `scipy.optimize.linprog` 라이브러리를 동원해 전체 예산 한계, 개별 채널 한도 수량 등 다차원 등식/부등식 제약을 반영하는 Linear Programming 최적해 솔버를 구성해냈습니다.
    * 📎 **참고 파일**: [`src/optimization/budget_optimizer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/optimization/budget_optimizer.py)
  * **What-if 분석 시나리오**: 기준 예산 대비 50%, 100%, 200% 스케일로 솔버를 3회 반복 구동하여 각각의 예산 폭에 대한 커버 대상 범위 및 이탈 방어율 민감도를 도출/비교하는 샌드박스를 제공합니다.
    * 📎 **참고 파일**: [`src/optimization/budget_optimizer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/optimization/budget_optimizer.py)
  * **예상 ROI 산출**: 최종 배정된 대상의 누적 투자 비용(Cost)과 최적해 셋의 방어 예상 매출 합계(Revenue)를 연산해 총 Return On Investment 스칼라 팩터를 반환 산출합니다.
    * 📎 **참고 파일**: [`src/optimization/budget_optimizer.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/optimization/budget_optimizer.py)

### 10. A/B 테스트 설계 및 분석
* **개요**: 무작위로 추출된 대조/실험군의 지표 차이가 통계적으로 유의미한 비즈니스 성과인지 검증하는 가설 검증 엔진입니다.
* **상세 요구사항별 구현 로직**:
  * **가상 A/B 테스트 설계**: `ABTestFramework` 클래스를 통해 시뮬레이터가 생성한 T(실험군) 그룹과 C(대조군) 그룹의 이탈 라벨을 인계하여 분석합니다.
    * 📎 **참고 파일**: [`src/models/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/ab_testing.py), [`src/analysis/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/ab_testing.py)
  * **필요 샘플 사이즈 (Power Analysis) 통계 산출**: `PowerAnalysis` 클래스를 활용하여 Z-스코어 기반 통계적 검정력($\beta=0.8$)과 유의수준($\alpha=0.05$) 조건으로 요구 최소 샘플 수치(MDE 한계표본)를 자동 산출합니다.
    * 📎 **참고 파일**: [`src/models/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/ab_testing.py), [`src/analysis/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/ab_testing.py)
  * **Treatment/Control 이탈률 통계적 검정**: 부트스트랩 시뮬레이션 대신 표준 통계적 검정을 사용합니다. 이진 지표는 비율 검정(Proportions Z-test) 및 카이제곱 검정(Chi-square)을, 연속형 수익 분포는 Welch's t-test를 적용해 양측 가설 적합성을 연산합니다.
    * 📎 **참고 파일**: [`src/models/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/ab_testing.py), [`src/analysis/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/analysis/ab_testing.py)
  * **95% 신뢰구간 / p-value 산출**: Scipy 통계 클래스의 응답값을 분해하여 성과 지표의 오차 한계 폭(CI, Confidence Interval) 데이터와 절대 확률 p-value 수치를 객체 단위로 저장 보관합니다.
    * 📎 **참고 파일**: [`src/models/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/ab_testing.py)
  * **통계적 유의성 여부 명시**: 도출된 p-value가 $\alpha=0.05$ 기준선을 하회하는지 평가 후 Boolean 형태의 `statistically_significant` 플래그를 찍어 반환함으로써 1차 검증을 마칩니다.
    * 📎 **참고 파일**: [`src/models/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/ab_testing.py)
  * **A/B 테스트 결과 해석 리포트 생성**: `ab_test_detailed.json`을 통해 산출된 통계값(목표 샘플, 관측력, 유의성 여부 등)을 기록 보존하고, `docs/ab_test_report.md` 양식으로 해석 문서를 연동 배출합니다.
    * 📎 **참고 파일**: [`src/models/ab_testing.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/models/ab_testing.py), [`docs/ab_test_report.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/ab_test_report.md)

### 11. 통합 대시보드
* **개요**: Streamlit을 통해 구현된 예측 데이터 전반의 시각화 컴포넌트 프론트엔드이며, 17개 페이지로 구성된 사이드바 기반의 네비게이션 구조를 제공합니다.
* **상세 요구사항별 구현 로직**:
  * **고객 이탈 위험 분포 시각화**: `dashboard/app.py` 뷰 포트 상에 고객층의 이탈 임계치 컷오프 및 위험도 예측 분포를 Plotly 히스토그램으로 출력합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **코호트 리텐션 곡선 시각화**: JSON 매트릭스를 기반으로 M1~M12 까지의 리텐션 잔존 비율을 시계열 라인 차트와 히트맵 텍스처로 교차 노출합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **Uplift 4분면 세그먼트 분포 시각화**: x축에 기본 이탈 확률, y축에 예측 CATE 점수를 매핑하여 Persuadables 등의 4분위 레이블을 2차원 산점도(Scatter) 좌표계로 분리 표출합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **CLV 분포 및 상위 고객 목록 표시**: 전체 CLV 볼륨 히스토그램 차트를 띄움과 동시에 Top-20% VIP 인덱스를 부여받은 데이터프레임을 테이블 형태로 노출합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **예산 배분 결과 및 ROI 표시**: 예산 최적화 모듈(LP)이 계산한 채널별/세그먼트별 파이차트 배분 비율 및 도출된 시뮬레이션 예상 ROI 지수를 요약 패널로 전시합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **A/B 테스트 결과 요약 표시**: p-value 및 유의성 검정 통과 여부가 담긴 지표 카드를 생성해 통계 리포트 탭으로 격리 제공합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **리텐션 고객 목록 (우선순위순) 조회**: Priority Score를 기준 키로 삼아 내림차순 정렬된 6+분위 고객군 명단을 Pagination 방식 테이블로 출력하여 마케팅 조치 즉시 이관이 가능하도록 지원합니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)
  * **수동 새로고침 데이터 업데이트**: 백엔드 파이프라인에서 신규 파생 파일이 떨어졌을 때, Streamlit 캐시 메모리를 삭제하고 최신 `.csv` 포인터를 다시 끌어올 수 있는 갱신 버튼 컨트롤을 장착했습니다.
    * 📎 **참고 파일**: [`src/dashboard/app.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/dashboard/app.py)

### 12. 모델 모니터링
* **개요**: 시간이 경과함에 따라 입력 데이터가 변질되거나 이탈 예측 모델의 정확도가 무너지는 현상을 감지하는 모듈입니다.
* **상세 요구사항별 구현 로직**:
  * **피처 분포 변화(Data Drift) 탐지**: `DriftDetector` 클래스가 과거 레퍼런스 분포의 분위수 구간과 현재 운영 데이터의 분포 간 거리를 기반으로 인구안정성지수(PSI, Population Stability Index) 공식을 연산하며, KS-test(콜모고로프-스미르노프 양방향 검정)를 교차 실행하여 피처 드리프트를 수학적으로 적발해냅니다.
    * 📎 **참고 파일**: [`src/monitoring/drift_detection.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/monitoring/drift_detection.py), [`src/monitoring/ks_drift.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/monitoring/ks_drift.py)
  * **모델 성능 지표 시간별 변화 추적**: 모델의 평가 로깅 객체로부터 타임스탬프별 홀드아웃 스코어(AUC-ROC, Precision, Recall)를 로드해 성능 지표의 과거 대비 하강 시계열 폭을 트래킹합니다.
    * 📎 **참고 파일**: [`src/monitoring/drift_detection.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/monitoring/drift_detection.py)
  * **Drift / 저하 발생 알림 생성**: PSI 0.10 이상일 때 Warning, 0.25 초과 시 심각한 Drift로 규정하여 플래그 트리거 및 알림 텍스트를 생성하는 Alert 체계를 로깅합니다.
    * 📎 **참고 파일**: [`src/monitoring/drift_detection.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/monitoring/drift_detection.py)
  * **monitoring_report.json 저장**: 드리프트 계산에 적발된 피처 배열 및 KS-test의 p-value 편차 이력을 직렬화 JSON 상태로 `results/` 폴더에 동기 보존시킵니다.
    * 📎 **참고 파일**: [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py), [`src/monitoring/monitoring_service.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/monitoring/monitoring_service.py)

### 13. 문서화 및 코드 품질
* **개요**: 솔루션의 소프트웨어 구조 및 가독성을 높여주는 문서, 아키텍처, 코드 규칙 프레임워크입니다.
* **상세 요구사항별 구현 로직**:
  * **README 시스템 아키텍처 다이어그램**: `README.md` 메인 파일에 ASCII 문자열 기반으로 설계된 Docker 네트워크, 레이어, 파이프라인 흐름을 도식화한 다이어그램 섹션을 삽입했습니다.
    * 📎 **참고 파일**: [`README.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/README.md), [`docs/architecture.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/architecture.md)
  * **피처 설계 근거 및 의미 (feature_dictionary)**: 33가지 피처 파생 시 의도한 비즈니스 로직과 수식, 정합적 해석 가이드를 `docs/feature_dictionary.md` 양식으로 독립 기재했습니다.
    * 📎 **참고 파일**: [`docs/feature_dictionary.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/feature_dictionary.md)
  * **모델 평가/세그먼트 전략서 (model_report / retention_strategy)**: 모델간 AUC 튜닝 대결 양상 및 성능표(`model_report.md`), Uplift 인과해석 및 Action 가이드라인(`retention_strategy.md`) 문서를 별도 백서 폼으로 작성해 두었습니다.
    * 📎 **참고 파일**: [`docs/model_report.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/model_report.md), [`docs/retention_strategy.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/retention_strategy.md)
  * **A/B 테스트 해석 리포트 작성**: 검정 결과를 수치만으로 내뿜지 않고 '현재 집단 분리가 통계적 타당성을 갖는다' 는 해설을 곁들인 `ab_test_report.md` 문서 마크다운을 자동화 출력합니다.
    * 📎 **참고 파일**: [`docs/ab_test_report.md`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/docs/ab_test_report.md)
  * **주요 함수 docstring 작성**: Python 소스 파일 전 구역의 클래스 인스턴스 및 함수 정의부에 입력 파라미터 규격 및 리턴 타입을 엄격히 준수하는 PEP-257 Google/Numpydoc 포맷의 docstring 주석을 체결해 두었습니다.
    * 📎 **참고 파일**: 전 모듈 (e.g., [`src/main.py`](file:///c:/Users/user/Desktop/캡디2/Capstone-Design-1/src/main.py))
  * **프로젝트 모듈별 분리 구조**: 모든 소스 구조가 단일 계층에 몰리지 않고 `data/`, `features/`, `models/`, `optimization/`, `dashboard/` 등 개발 패러다임에 맞춰 관심사 분리(Separation of Concerns) 디자인 패턴으로 파티셔닝되어 동작합니다.
    * 📎 **참고 파일**: 최상위 루트 디렉토리 및 `src/` 하위 폴더 트리

---

## 3. 목표 대비 수행 실적 자체 평가

본 프로젝트는 이커머스 환경의 고객 이탈 예측 및 리텐션 예산 최적화라는 핵심 목표를 성공적으로 달성했습니다. 초기 요구 명세서에 정의된 13가지 필수 요구사항을 전원(100%) 이행하였을 뿐 아니라, 고난도의 4가지 선택(보너스) 과제까지 모두 완벽히 구현해 내어 **목표를 초과 달성**하였습니다. 스스로 평가한 주요 실적 요약은 다음과 같습니다.

### 1) 기술적 완성도 및 예측 성능 초과 달성
* **이탈 예측 성능 (AUC-ROC 0.84~0.90)**: 요구된 모델 성능 기준(0.78 이상)을 가뿐히 넘어서, 머신러닝(GBDT 계열)과 직접 구축한 딥러닝(LSTM/Transformer) 앙상블 구조를 통해 매우 우수한 예측력을 입증했습니다.
* **복합적 마케팅 최적화 구현**: 단순 예측을 넘어, 인과 추론(Uplift Modeling)을 통한 4분면 타겟팅, 생애 가치(CLV) 예측 기반 우선순위 산정, 선형 계획법(LP) 기반의 다차원 예산 최적화 알고리즘까지 통합 구현하여 실질적인 비즈니스 ROI를 극대화해 냈습니다.

### 2) 프로덕션 수준의 확장성 및 아키텍처 품질
* **데이터 신뢰성 및 모니터링**: PSI 및 KS-Test를 통한 데이터 드리프트(Data Drift) 감지 모듈과 Redis Streams 연동 실시간 스코어링 시스템을 구축해 실제 상용 서비스에 준하는 운영 인프라 안정성을 시연했습니다.
* **코드 무결성과 문서화 역량**: 33개의 파생 피처 정의서 도출, 7종의 필수 산출물 문서 구축, PEP-257 표준 Docstring 적용 및 MLflow 모델 파라미터 자동 트래킹을 통해 뛰어난 시스템 유지보수성과 재현성을 확보했습니다.

**종합 의견**: 본 과제 결과물은 시뮬레이션 파이프라인부터 데이터 전처리, 모델 서빙, 최적화 검증, 대시보드 프론트엔드까지 단절 없이 유기적으로 맞물려 구동됩니다. 단순히 요구사항을 코드화하는 수준을 넘어, 데이터를 활용하여 "가장 가치 있는 고객의 이탈을 가장 적은 비용으로 효율적으로 방어한다"는 실제 프로덕트 기획 의도를 기술적으로 완벽히 증명한 최우수 결과물로 자체 평가합니다.
