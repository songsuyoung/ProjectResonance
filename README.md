# Resonance

Unreal Engine 5 기반 오픈월드 생활형 NPC AI 시스템 프로토타입.

NPC가 도시 내 Region을 자율적으로 이동하고, 감정 상태에 따라 행동이 달라지며, 농부처럼 반복적인 생활 행동을 수행하는 시스템을 구현한다.

---

## 모듈 구성

```
Source/
├── ResonanceCore/      공통 기반 (의존성 없음) — 인터페이스, 공통 구조체, JSON 유틸
├── Resonance/          메인 런타임 — 캐릭터, NPC AI, 전투, 농부 시스템
├── ResonanceClient/    클라이언트 UI — CommonUI 기반 HUD, NPC 상태 디스플레이
└── ResonanceEditor/    에디터 툴 — 경로 그래프 Bake 시스템
```

---

## 구현 이력

| 단계 | 내용 |
|---|---|
| 기반 | 모듈 분리 (Core/Runtime/Client/Editor), Input Command 패턴, DataManager 템플릿 |
| 전투 | 콤보 공격, AnimNotify 기반 히트 체크, GameplayTag 상태 전환, Skill/Weapon 모듈화 |
| NPC 기반 | WayPoint 이동, Region 순차 방문, Spawn 시스템, NPC 풀링 |
| 경로 탐색 | Dijkstra 알고리즘, 직선/순환 이동, 우측 보행 규칙, Spline 부드러운 이동 |
| NPC 감정 | EmotionComponent, 스태미나 연동, 장소 체류 시간 보정 |
| NPC UI | NPC ViewTarget, NPC 상태 디스플레이 위젯 |
| 농부 - 기초 | 밭갈기 이동 루틴, 1회 사이클, 지그재그 패턴, 농부 메시 |
| 농부 - 고도화 | NavigateToPlot → FarmWork 반복, 도구 교체(ChangeTool/DecideTool), 집 귀환 |
| 농부 - 데이터화 | DataTable 기반 일과 스케줄(`RFarmDailySchedule`), `RTimeManager` 게임 내 시간 |

---

## 주요 시스템

### NPC AI (StateTree)

UE5 StateTree 기반. `ARAIController`가 `UStateTreeAIComponent`를 소유하며 NPC 행동 전체를 제어한다.

**일반 NPC 이동 흐름:**
```
DecideNextLocation → FindLinePath → MoveToSpline → EnterRegion
                                                  ↘ RestToRecover (스태미나 부족 시)
```

| Task | 역할 |
|---|---|
| `URSTTask_DecideNextLocation` | 감정 상태 반영 가중치로 다음 Region 결정 |
| `URSTTask_FindLinePath` | Dijkstra + 방향 편향으로 최단 경로 탐색 |
| `URSTTask_MoveToSpline` | 스플라인 경로 따라 이동. LookAhead Steering, 감속 반경 적용 |
| `URSTTask_EnterRegion` | Region 입장 처리 (TurnInPlace → Walk → 숨김) |
| `URSTTask_RestToRecover` | 목표 스태미나 비율까지 회복 |

---

### 농부 시스템 (Farm)

`ARFarmPlotVolume`이 Row × Column 격자 밭을 관리한다. 작업 순서는 Snake 패턴 (짝수 행 좌→우, 홀수 행 우→좌).

**작업 단계:** `Till → Clear → Sow → Water → Weed → Harvest`

**농부 State 흐름:**
```
DecideTool → ChangeTool → NavigateToPlot → FarmWork → (반복)
                                                     ↘ 귀환 (모든 플롯 완료 시)
```

| Task / Condition | 역할 |
|---|---|
| `URSTTask_NavigateToPlot` | 다음 작업할 플롯 위치로 이동 |
| `URSTTask_FarmWork` | 플롯에서 밭일 수행 (`CompleteCurrentPlot` 호출) |
| `URSTTask_DecideTool` | 현재 FarmTask에 맞는 도구 결정 |
| `URSTTask_ChangeTool` | `URJobComponent`에 도구 메시 교체 요청 |
| `URSTCondition_IsFarmTaskMatching` | 현재 Task와 도구가 일치하는지 확인 |
| `URSTCondition_IsToolChangeRequired` | 도구 교체 필요 여부 확인 |

**데이터 구조:**
- `ARFarmPlotVolume`: `ActivePlotIndex` 하나로 Snake 순서 추적. `GetActivePlot()` / `CompleteCurrentPlot()` 인터페이스 제공
- `FRFarmDailySchedule`: DataTable Row — 하루 수행할 `ERequiredFarmTask` 배열 정의
- `URJobComponent`: NPC가 소유한 도구 컴포넌트. Async 로드로 StaticMesh 교체

---

### 시간 시스템 (TimeManager)

`URTimeManager` — `UTickableWorldSubsystem` 파생. 게임 내 시간 흐름을 관리한다.

- `URTimeSettings` (`UDeveloperSettings`): 에디터 Project Settings에서 `HourDuration`(초/시간), `HoursPerDay` 설정 가능
- `OnHourChanged` / `OnDayChanged` 멀티캐스트 델리게이트로 시간 이벤트 전파
- `ARFarmPlotVolume`이 `OnDayChanged`를 수신해 매일 밭 플롯 초기화(`InitializePlotData`) 실행

---

### 감정 시스템 (Emotion)

`UREmotionComponent`가 스탯 변화를 수신해 우선순위 기반으로 활성 감정을 결정한다.

| 감정 | 우선순위 | WalkSpeed 배율 |
|---|---|---|
| Angry | 0 (최고) | x2.0 |
| Tired | 1 | x0.6 |
| Annoyed | 2 | x1.1 |
| Calm | 5 (기본) | x1.0 |

감정은 장소 체류 시간(`HangAroundTime`)에도 영향을 준다:
```
TotalDuration = BaseDuration × EmotionMult × LocationPref × StaminaMult
DurationTime  = Clamp(TotalDuration, 5.0f, 20.0f)
```

---

### Region 시스템

`ARRegionVolume`(AVolume 파생)이 NPC 입장 위치/방향을 제공한다.

- `URRegionManager`: Region 등록/조회, 가중치 기반 다음 방문 Region 추첨. 현재 Region 가중치는 0.5배 감소시켜 반복 방문 억제
- `URNPCSpawnManager`: NPC 풀링 관리. Region 입장 시 NPC 숨김 → HangAroundTime 타이머 만료 후 재활성화

---

### 경로 탐색 시스템

에디터에서 PathPoint Actor를 배치하고 Bake 버튼을 누르면 그래프 데이터(`GraphNodeData.json`)가 생성된다. 런타임에 `URPathFinder`가 이를 로드해 Dijkstra 탐색을 수행한다.

- **방향 편향**: 매 탐색마다 좌/우 방향을 랜덤 선택해 경로 다양성 확보
- **실제 보행 경로**: 그래프 노드 간 구간마다 NavMesh `FindPathToLocationSynchronously` 호출

---

### 플레이어 전투 시스템

GameplayTag 기반 `URActionStateComponent`가 상태 전환을 관리한다.

**콤보 흐름:**
1. `RAnimNotifyState_PlayNextCombo` Begin → 콤보 입력 허용
2. 공격 입력 → `PendingComboSkill` 예약
3. `RAnimNotify_PlayNextCombo` 발생 → 예약된 스킬 실행

**히트 체크:** `RAnimNotifyState_HitCheck`가 매 Tick 이전/현재 본 트랜스폼을 보간하며 `SweepMultiByChannel` 실행.

---

### 이벤트 시스템

Observer 패턴. `UREventManager` + `IRMessageReceiver` 인터페이스.

```cpp
REVENT_MESSAGE_NOTIFY(StartGame);
REVENT_MESSAGE_NOTIFY_MSG(EnterRegion, Payload);
```

주요 메시지 타입: `StartGame`, `EnterRegion`, `InitStat`, `UpdateStat`, `UpdateEmotion`

---

## 기술 스택

| 항목 | 내용 |
|---|---|
| 엔진 | Unreal Engine 5 |
| AI | StateTree (`UStateTreeAIComponent`) |
| 이동 | NavMesh + USplineComponent |
| 입력 | Enhanced Input |
| UI | CommonUI (`UCommonActivatableWidget`) |
| 데이터 | DataTable + JSON (JsonObjectConverter) |
| 에디터 | LevelEditor Toolbar Extension + 커스텀 Bake 시스템 |
| 시간 | `UTickableWorldSubsystem` 기반 게임 내 시간 (`URTimeManager`) |
