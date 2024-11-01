// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HEARTBEAT_AIMM_HEARTBEAT_H_
#define FLATBUFFERS_GENERATED_HEARTBEAT_AIMM_HEARTBEAT_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace aimm {
namespace heartbeat {

struct WaterValveState;
struct WaterValveStateBuilder;
struct WaterValveStateT;

struct PressurizationStatus;
struct PressurizationStatusBuilder;
struct PressurizationStatusT;

struct VoltageLevels;
struct VoltageLevelsBuilder;
struct VoltageLevelsT;

struct Heartbeat;
struct HeartbeatBuilder;
struct HeartbeatT;

inline const ::flatbuffers::TypeTable *WaterValveStateTypeTable();

inline const ::flatbuffers::TypeTable *PressurizationStatusTypeTable();

inline const ::flatbuffers::TypeTable *VoltageLevelsTypeTable();

inline const ::flatbuffers::TypeTable *HeartbeatTypeTable();

struct WaterValveStateT : public ::flatbuffers::NativeTable {
  typedef WaterValveState TableType;
  std::vector<bool> states{};
};

struct WaterValveState FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef WaterValveStateT NativeTableType;
  typedef WaterValveStateBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return WaterValveStateTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_STATES = 4
  };
  const ::flatbuffers::Vector<uint8_t> *states() const {
    return GetPointer<const ::flatbuffers::Vector<uint8_t> *>(VT_STATES);
  }
  ::flatbuffers::Vector<uint8_t> *mutable_states() {
    return GetPointer<::flatbuffers::Vector<uint8_t> *>(VT_STATES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_STATES) &&
           verifier.VerifyVector(states()) &&
           verifier.EndTable();
  }
  WaterValveStateT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(WaterValveStateT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<WaterValveState> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const WaterValveStateT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct WaterValveStateBuilder {
  typedef WaterValveState Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_states(::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> states) {
    fbb_.AddOffset(WaterValveState::VT_STATES, states);
  }
  explicit WaterValveStateBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<WaterValveState> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<WaterValveState>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<WaterValveState> CreateWaterValveState(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> states = 0) {
  WaterValveStateBuilder builder_(_fbb);
  builder_.add_states(states);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<WaterValveState> CreateWaterValveStateDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *states = nullptr) {
  auto states__ = states ? _fbb.CreateVector<uint8_t>(*states) : 0;
  return aimm::heartbeat::CreateWaterValveState(
      _fbb,
      states__);
}

::flatbuffers::Offset<WaterValveState> CreateWaterValveState(::flatbuffers::FlatBufferBuilder &_fbb, const WaterValveStateT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct PressurizationStatusT : public ::flatbuffers::NativeTable {
  typedef PressurizationStatus TableType;
  float pressure = 0.0f;
  bool is_target_pressure = false;
};

struct PressurizationStatus FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PressurizationStatusT NativeTableType;
  typedef PressurizationStatusBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return PressurizationStatusTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PRESSURE = 4,
    VT_IS_TARGET_PRESSURE = 6
  };
  float pressure() const {
    return GetField<float>(VT_PRESSURE, 0.0f);
  }
  bool mutate_pressure(float _pressure = 0.0f) {
    return SetField<float>(VT_PRESSURE, _pressure, 0.0f);
  }
  bool is_target_pressure() const {
    return GetField<uint8_t>(VT_IS_TARGET_PRESSURE, 0) != 0;
  }
  bool mutate_is_target_pressure(bool _is_target_pressure = 0) {
    return SetField<uint8_t>(VT_IS_TARGET_PRESSURE, static_cast<uint8_t>(_is_target_pressure), 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_PRESSURE, 4) &&
           VerifyField<uint8_t>(verifier, VT_IS_TARGET_PRESSURE, 1) &&
           verifier.EndTable();
  }
  PressurizationStatusT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(PressurizationStatusT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<PressurizationStatus> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const PressurizationStatusT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct PressurizationStatusBuilder {
  typedef PressurizationStatus Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_pressure(float pressure) {
    fbb_.AddElement<float>(PressurizationStatus::VT_PRESSURE, pressure, 0.0f);
  }
  void add_is_target_pressure(bool is_target_pressure) {
    fbb_.AddElement<uint8_t>(PressurizationStatus::VT_IS_TARGET_PRESSURE, static_cast<uint8_t>(is_target_pressure), 0);
  }
  explicit PressurizationStatusBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<PressurizationStatus> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<PressurizationStatus>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<PressurizationStatus> CreatePressurizationStatus(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float pressure = 0.0f,
    bool is_target_pressure = false) {
  PressurizationStatusBuilder builder_(_fbb);
  builder_.add_pressure(pressure);
  builder_.add_is_target_pressure(is_target_pressure);
  return builder_.Finish();
}

::flatbuffers::Offset<PressurizationStatus> CreatePressurizationStatus(::flatbuffers::FlatBufferBuilder &_fbb, const PressurizationStatusT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct VoltageLevelsT : public ::flatbuffers::NativeTable {
  typedef VoltageLevels TableType;
  float main_battery = 0.0f;
  float auxiliary_battery = 0.0f;
  float auxiliary_battery_two = 0.0f;
  float auxiliary_battery_three = 0.0f;
  float voltage_auxillarly_computer = 0.0f;
};

struct VoltageLevels FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef VoltageLevelsT NativeTableType;
  typedef VoltageLevelsBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return VoltageLevelsTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MAIN_BATTERY = 4,
    VT_AUXILIARY_BATTERY = 6,
    VT_AUXILIARY_BATTERY_TWO = 8,
    VT_AUXILIARY_BATTERY_THREE = 10,
    VT_VOLTAGE_AUXILLARLY_COMPUTER = 12
  };
  float main_battery() const {
    return GetField<float>(VT_MAIN_BATTERY, 0.0f);
  }
  bool mutate_main_battery(float _main_battery = 0.0f) {
    return SetField<float>(VT_MAIN_BATTERY, _main_battery, 0.0f);
  }
  float auxiliary_battery() const {
    return GetField<float>(VT_AUXILIARY_BATTERY, 0.0f);
  }
  bool mutate_auxiliary_battery(float _auxiliary_battery = 0.0f) {
    return SetField<float>(VT_AUXILIARY_BATTERY, _auxiliary_battery, 0.0f);
  }
  float auxiliary_battery_two() const {
    return GetField<float>(VT_AUXILIARY_BATTERY_TWO, 0.0f);
  }
  bool mutate_auxiliary_battery_two(float _auxiliary_battery_two = 0.0f) {
    return SetField<float>(VT_AUXILIARY_BATTERY_TWO, _auxiliary_battery_two, 0.0f);
  }
  float auxiliary_battery_three() const {
    return GetField<float>(VT_AUXILIARY_BATTERY_THREE, 0.0f);
  }
  bool mutate_auxiliary_battery_three(float _auxiliary_battery_three = 0.0f) {
    return SetField<float>(VT_AUXILIARY_BATTERY_THREE, _auxiliary_battery_three, 0.0f);
  }
  float voltage_auxillarly_computer() const {
    return GetField<float>(VT_VOLTAGE_AUXILLARLY_COMPUTER, 0.0f);
  }
  bool mutate_voltage_auxillarly_computer(float _voltage_auxillarly_computer = 0.0f) {
    return SetField<float>(VT_VOLTAGE_AUXILLARLY_COMPUTER, _voltage_auxillarly_computer, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_MAIN_BATTERY, 4) &&
           VerifyField<float>(verifier, VT_AUXILIARY_BATTERY, 4) &&
           VerifyField<float>(verifier, VT_AUXILIARY_BATTERY_TWO, 4) &&
           VerifyField<float>(verifier, VT_AUXILIARY_BATTERY_THREE, 4) &&
           VerifyField<float>(verifier, VT_VOLTAGE_AUXILLARLY_COMPUTER, 4) &&
           verifier.EndTable();
  }
  VoltageLevelsT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(VoltageLevelsT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<VoltageLevels> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const VoltageLevelsT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct VoltageLevelsBuilder {
  typedef VoltageLevels Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_main_battery(float main_battery) {
    fbb_.AddElement<float>(VoltageLevels::VT_MAIN_BATTERY, main_battery, 0.0f);
  }
  void add_auxiliary_battery(float auxiliary_battery) {
    fbb_.AddElement<float>(VoltageLevels::VT_AUXILIARY_BATTERY, auxiliary_battery, 0.0f);
  }
  void add_auxiliary_battery_two(float auxiliary_battery_two) {
    fbb_.AddElement<float>(VoltageLevels::VT_AUXILIARY_BATTERY_TWO, auxiliary_battery_two, 0.0f);
  }
  void add_auxiliary_battery_three(float auxiliary_battery_three) {
    fbb_.AddElement<float>(VoltageLevels::VT_AUXILIARY_BATTERY_THREE, auxiliary_battery_three, 0.0f);
  }
  void add_voltage_auxillarly_computer(float voltage_auxillarly_computer) {
    fbb_.AddElement<float>(VoltageLevels::VT_VOLTAGE_AUXILLARLY_COMPUTER, voltage_auxillarly_computer, 0.0f);
  }
  explicit VoltageLevelsBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<VoltageLevels> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<VoltageLevels>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<VoltageLevels> CreateVoltageLevels(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float main_battery = 0.0f,
    float auxiliary_battery = 0.0f,
    float auxiliary_battery_two = 0.0f,
    float auxiliary_battery_three = 0.0f,
    float voltage_auxillarly_computer = 0.0f) {
  VoltageLevelsBuilder builder_(_fbb);
  builder_.add_voltage_auxillarly_computer(voltage_auxillarly_computer);
  builder_.add_auxiliary_battery_three(auxiliary_battery_three);
  builder_.add_auxiliary_battery_two(auxiliary_battery_two);
  builder_.add_auxiliary_battery(auxiliary_battery);
  builder_.add_main_battery(main_battery);
  return builder_.Finish();
}

::flatbuffers::Offset<VoltageLevels> CreateVoltageLevels(::flatbuffers::FlatBufferBuilder &_fbb, const VoltageLevelsT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct HeartbeatT : public ::flatbuffers::NativeTable {
  typedef Heartbeat TableType;
  std::string timestamp{};
  float pump_pressure = 0.0f;
  float depth = 0.0f;
  std::unique_ptr<aimm::heartbeat::WaterValveStateT> valveStates{};
  std::unique_ptr<aimm::heartbeat::PressurizationStatusT> pressurization{};
  std::unique_ptr<aimm::heartbeat::VoltageLevelsT> voltages{};
  HeartbeatT() = default;
  HeartbeatT(const HeartbeatT &o);
  HeartbeatT(HeartbeatT&&) FLATBUFFERS_NOEXCEPT = default;
  HeartbeatT &operator=(HeartbeatT o) FLATBUFFERS_NOEXCEPT;
};

struct Heartbeat FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef HeartbeatT NativeTableType;
  typedef HeartbeatBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return HeartbeatTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TIMESTAMP = 4,
    VT_PUMP_PRESSURE = 6,
    VT_DEPTH = 8,
    VT_VALVESTATES = 10,
    VT_PRESSURIZATION = 12,
    VT_VOLTAGES = 14
  };
  const ::flatbuffers::String *timestamp() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TIMESTAMP);
  }
  ::flatbuffers::String *mutable_timestamp() {
    return GetPointer<::flatbuffers::String *>(VT_TIMESTAMP);
  }
  float pump_pressure() const {
    return GetField<float>(VT_PUMP_PRESSURE, 0.0f);
  }
  bool mutate_pump_pressure(float _pump_pressure = 0.0f) {
    return SetField<float>(VT_PUMP_PRESSURE, _pump_pressure, 0.0f);
  }
  float depth() const {
    return GetField<float>(VT_DEPTH, 0.0f);
  }
  bool mutate_depth(float _depth = 0.0f) {
    return SetField<float>(VT_DEPTH, _depth, 0.0f);
  }
  const aimm::heartbeat::WaterValveState *valveStates() const {
    return GetPointer<const aimm::heartbeat::WaterValveState *>(VT_VALVESTATES);
  }
  aimm::heartbeat::WaterValveState *mutable_valveStates() {
    return GetPointer<aimm::heartbeat::WaterValveState *>(VT_VALVESTATES);
  }
  const aimm::heartbeat::PressurizationStatus *pressurization() const {
    return GetPointer<const aimm::heartbeat::PressurizationStatus *>(VT_PRESSURIZATION);
  }
  aimm::heartbeat::PressurizationStatus *mutable_pressurization() {
    return GetPointer<aimm::heartbeat::PressurizationStatus *>(VT_PRESSURIZATION);
  }
  const aimm::heartbeat::VoltageLevels *voltages() const {
    return GetPointer<const aimm::heartbeat::VoltageLevels *>(VT_VOLTAGES);
  }
  aimm::heartbeat::VoltageLevels *mutable_voltages() {
    return GetPointer<aimm::heartbeat::VoltageLevels *>(VT_VOLTAGES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TIMESTAMP) &&
           verifier.VerifyString(timestamp()) &&
           VerifyField<float>(verifier, VT_PUMP_PRESSURE, 4) &&
           VerifyField<float>(verifier, VT_DEPTH, 4) &&
           VerifyOffset(verifier, VT_VALVESTATES) &&
           verifier.VerifyTable(valveStates()) &&
           VerifyOffset(verifier, VT_PRESSURIZATION) &&
           verifier.VerifyTable(pressurization()) &&
           VerifyOffset(verifier, VT_VOLTAGES) &&
           verifier.VerifyTable(voltages()) &&
           verifier.EndTable();
  }
  HeartbeatT *UnPack(const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(HeartbeatT *_o, const ::flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static ::flatbuffers::Offset<Heartbeat> Pack(::flatbuffers::FlatBufferBuilder &_fbb, const HeartbeatT* _o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct HeartbeatBuilder {
  typedef Heartbeat Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_timestamp(::flatbuffers::Offset<::flatbuffers::String> timestamp) {
    fbb_.AddOffset(Heartbeat::VT_TIMESTAMP, timestamp);
  }
  void add_pump_pressure(float pump_pressure) {
    fbb_.AddElement<float>(Heartbeat::VT_PUMP_PRESSURE, pump_pressure, 0.0f);
  }
  void add_depth(float depth) {
    fbb_.AddElement<float>(Heartbeat::VT_DEPTH, depth, 0.0f);
  }
  void add_valveStates(::flatbuffers::Offset<aimm::heartbeat::WaterValveState> valveStates) {
    fbb_.AddOffset(Heartbeat::VT_VALVESTATES, valveStates);
  }
  void add_pressurization(::flatbuffers::Offset<aimm::heartbeat::PressurizationStatus> pressurization) {
    fbb_.AddOffset(Heartbeat::VT_PRESSURIZATION, pressurization);
  }
  void add_voltages(::flatbuffers::Offset<aimm::heartbeat::VoltageLevels> voltages) {
    fbb_.AddOffset(Heartbeat::VT_VOLTAGES, voltages);
  }
  explicit HeartbeatBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Heartbeat> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Heartbeat>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Heartbeat> CreateHeartbeat(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> timestamp = 0,
    float pump_pressure = 0.0f,
    float depth = 0.0f,
    ::flatbuffers::Offset<aimm::heartbeat::WaterValveState> valveStates = 0,
    ::flatbuffers::Offset<aimm::heartbeat::PressurizationStatus> pressurization = 0,
    ::flatbuffers::Offset<aimm::heartbeat::VoltageLevels> voltages = 0) {
  HeartbeatBuilder builder_(_fbb);
  builder_.add_voltages(voltages);
  builder_.add_pressurization(pressurization);
  builder_.add_valveStates(valveStates);
  builder_.add_depth(depth);
  builder_.add_pump_pressure(pump_pressure);
  builder_.add_timestamp(timestamp);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Heartbeat> CreateHeartbeatDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *timestamp = nullptr,
    float pump_pressure = 0.0f,
    float depth = 0.0f,
    ::flatbuffers::Offset<aimm::heartbeat::WaterValveState> valveStates = 0,
    ::flatbuffers::Offset<aimm::heartbeat::PressurizationStatus> pressurization = 0,
    ::flatbuffers::Offset<aimm::heartbeat::VoltageLevels> voltages = 0) {
  auto timestamp__ = timestamp ? _fbb.CreateString(timestamp) : 0;
  return aimm::heartbeat::CreateHeartbeat(
      _fbb,
      timestamp__,
      pump_pressure,
      depth,
      valveStates,
      pressurization,
      voltages);
}

::flatbuffers::Offset<Heartbeat> CreateHeartbeat(::flatbuffers::FlatBufferBuilder &_fbb, const HeartbeatT *_o, const ::flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline WaterValveStateT *WaterValveState::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<WaterValveStateT>(new WaterValveStateT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void WaterValveState::UnPackTo(WaterValveStateT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = states(); if (_e) { _o->states.resize(_e->size()); for (::flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->states[_i] = _e->Get(_i) != 0; } } else { _o->states.resize(0); } }
}

inline ::flatbuffers::Offset<WaterValveState> WaterValveState::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const WaterValveStateT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateWaterValveState(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<WaterValveState> CreateWaterValveState(::flatbuffers::FlatBufferBuilder &_fbb, const WaterValveStateT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const WaterValveStateT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _states = _o->states.size() ? _fbb.CreateVector(_o->states) : 0;
  return aimm::heartbeat::CreateWaterValveState(
      _fbb,
      _states);
}

inline PressurizationStatusT *PressurizationStatus::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<PressurizationStatusT>(new PressurizationStatusT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void PressurizationStatus::UnPackTo(PressurizationStatusT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = pressure(); _o->pressure = _e; }
  { auto _e = is_target_pressure(); _o->is_target_pressure = _e; }
}

inline ::flatbuffers::Offset<PressurizationStatus> PressurizationStatus::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const PressurizationStatusT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreatePressurizationStatus(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<PressurizationStatus> CreatePressurizationStatus(::flatbuffers::FlatBufferBuilder &_fbb, const PressurizationStatusT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const PressurizationStatusT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _pressure = _o->pressure;
  auto _is_target_pressure = _o->is_target_pressure;
  return aimm::heartbeat::CreatePressurizationStatus(
      _fbb,
      _pressure,
      _is_target_pressure);
}

inline VoltageLevelsT *VoltageLevels::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<VoltageLevelsT>(new VoltageLevelsT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void VoltageLevels::UnPackTo(VoltageLevelsT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = main_battery(); _o->main_battery = _e; }
  { auto _e = auxiliary_battery(); _o->auxiliary_battery = _e; }
  { auto _e = auxiliary_battery_two(); _o->auxiliary_battery_two = _e; }
  { auto _e = auxiliary_battery_three(); _o->auxiliary_battery_three = _e; }
  { auto _e = voltage_auxillarly_computer(); _o->voltage_auxillarly_computer = _e; }
}

inline ::flatbuffers::Offset<VoltageLevels> VoltageLevels::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const VoltageLevelsT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateVoltageLevels(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<VoltageLevels> CreateVoltageLevels(::flatbuffers::FlatBufferBuilder &_fbb, const VoltageLevelsT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const VoltageLevelsT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _main_battery = _o->main_battery;
  auto _auxiliary_battery = _o->auxiliary_battery;
  auto _auxiliary_battery_two = _o->auxiliary_battery_two;
  auto _auxiliary_battery_three = _o->auxiliary_battery_three;
  auto _voltage_auxillarly_computer = _o->voltage_auxillarly_computer;
  return aimm::heartbeat::CreateVoltageLevels(
      _fbb,
      _main_battery,
      _auxiliary_battery,
      _auxiliary_battery_two,
      _auxiliary_battery_three,
      _voltage_auxillarly_computer);
}

inline HeartbeatT::HeartbeatT(const HeartbeatT &o)
      : timestamp(o.timestamp),
        pump_pressure(o.pump_pressure),
        depth(o.depth),
        valveStates((o.valveStates) ? new aimm::heartbeat::WaterValveStateT(*o.valveStates) : nullptr),
        pressurization((o.pressurization) ? new aimm::heartbeat::PressurizationStatusT(*o.pressurization) : nullptr),
        voltages((o.voltages) ? new aimm::heartbeat::VoltageLevelsT(*o.voltages) : nullptr) {
}

inline HeartbeatT &HeartbeatT::operator=(HeartbeatT o) FLATBUFFERS_NOEXCEPT {
  std::swap(timestamp, o.timestamp);
  std::swap(pump_pressure, o.pump_pressure);
  std::swap(depth, o.depth);
  std::swap(valveStates, o.valveStates);
  std::swap(pressurization, o.pressurization);
  std::swap(voltages, o.voltages);
  return *this;
}

inline HeartbeatT *Heartbeat::UnPack(const ::flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<HeartbeatT>(new HeartbeatT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Heartbeat::UnPackTo(HeartbeatT *_o, const ::flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = timestamp(); if (_e) _o->timestamp = _e->str(); }
  { auto _e = pump_pressure(); _o->pump_pressure = _e; }
  { auto _e = depth(); _o->depth = _e; }
  { auto _e = valveStates(); if (_e) { if(_o->valveStates) { _e->UnPackTo(_o->valveStates.get(), _resolver); } else { _o->valveStates = std::unique_ptr<aimm::heartbeat::WaterValveStateT>(_e->UnPack(_resolver)); } } else if (_o->valveStates) { _o->valveStates.reset(); } }
  { auto _e = pressurization(); if (_e) { if(_o->pressurization) { _e->UnPackTo(_o->pressurization.get(), _resolver); } else { _o->pressurization = std::unique_ptr<aimm::heartbeat::PressurizationStatusT>(_e->UnPack(_resolver)); } } else if (_o->pressurization) { _o->pressurization.reset(); } }
  { auto _e = voltages(); if (_e) { if(_o->voltages) { _e->UnPackTo(_o->voltages.get(), _resolver); } else { _o->voltages = std::unique_ptr<aimm::heartbeat::VoltageLevelsT>(_e->UnPack(_resolver)); } } else if (_o->voltages) { _o->voltages.reset(); } }
}

inline ::flatbuffers::Offset<Heartbeat> Heartbeat::Pack(::flatbuffers::FlatBufferBuilder &_fbb, const HeartbeatT* _o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  return CreateHeartbeat(_fbb, _o, _rehasher);
}

inline ::flatbuffers::Offset<Heartbeat> CreateHeartbeat(::flatbuffers::FlatBufferBuilder &_fbb, const HeartbeatT *_o, const ::flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { ::flatbuffers::FlatBufferBuilder *__fbb; const HeartbeatT* __o; const ::flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _timestamp = _o->timestamp.empty() ? 0 : _fbb.CreateString(_o->timestamp);
  auto _pump_pressure = _o->pump_pressure;
  auto _depth = _o->depth;
  auto _valveStates = _o->valveStates ? CreateWaterValveState(_fbb, _o->valveStates.get(), _rehasher) : 0;
  auto _pressurization = _o->pressurization ? CreatePressurizationStatus(_fbb, _o->pressurization.get(), _rehasher) : 0;
  auto _voltages = _o->voltages ? CreateVoltageLevels(_fbb, _o->voltages.get(), _rehasher) : 0;
  return aimm::heartbeat::CreateHeartbeat(
      _fbb,
      _timestamp,
      _pump_pressure,
      _depth,
      _valveStates,
      _pressurization,
      _voltages);
}

inline const ::flatbuffers::TypeTable *WaterValveStateTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_BOOL, 1, -1 }
  };
  static const char * const names[] = {
    "states"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 1, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const ::flatbuffers::TypeTable *PressurizationStatusTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_BOOL, 0, -1 }
  };
  static const char * const names[] = {
    "pressure",
    "is_target_pressure"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 2, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const ::flatbuffers::TypeTable *VoltageLevelsTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 }
  };
  static const char * const names[] = {
    "main_battery",
    "auxiliary_battery",
    "auxiliary_battery_two",
    "auxiliary_battery_three",
    "voltage_auxillarly_computer"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 5, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const ::flatbuffers::TypeTable *HeartbeatTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_STRING, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_SEQUENCE, 0, 0 },
    { ::flatbuffers::ET_SEQUENCE, 0, 1 },
    { ::flatbuffers::ET_SEQUENCE, 0, 2 }
  };
  static const ::flatbuffers::TypeFunction type_refs[] = {
    aimm::heartbeat::WaterValveStateTypeTable,
    aimm::heartbeat::PressurizationStatusTypeTable,
    aimm::heartbeat::VoltageLevelsTypeTable
  };
  static const char * const names[] = {
    "timestamp",
    "pump_pressure",
    "depth",
    "valveStates",
    "pressurization",
    "voltages"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 6, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

inline const aimm::heartbeat::Heartbeat *GetHeartbeat(const void *buf) {
  return ::flatbuffers::GetRoot<aimm::heartbeat::Heartbeat>(buf);
}

inline const aimm::heartbeat::Heartbeat *GetSizePrefixedHeartbeat(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<aimm::heartbeat::Heartbeat>(buf);
}

inline Heartbeat *GetMutableHeartbeat(void *buf) {
  return ::flatbuffers::GetMutableRoot<Heartbeat>(buf);
}

inline aimm::heartbeat::Heartbeat *GetMutableSizePrefixedHeartbeat(void *buf) {
  return ::flatbuffers::GetMutableSizePrefixedRoot<aimm::heartbeat::Heartbeat>(buf);
}

inline bool VerifyHeartbeatBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<aimm::heartbeat::Heartbeat>(nullptr);
}

inline bool VerifySizePrefixedHeartbeatBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<aimm::heartbeat::Heartbeat>(nullptr);
}

inline void FinishHeartbeatBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<aimm::heartbeat::Heartbeat> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedHeartbeatBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<aimm::heartbeat::Heartbeat> root) {
  fbb.FinishSizePrefixed(root);
}

inline std::unique_ptr<aimm::heartbeat::HeartbeatT> UnPackHeartbeat(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<aimm::heartbeat::HeartbeatT>(GetHeartbeat(buf)->UnPack(res));
}

inline std::unique_ptr<aimm::heartbeat::HeartbeatT> UnPackSizePrefixedHeartbeat(
    const void *buf,
    const ::flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<aimm::heartbeat::HeartbeatT>(GetSizePrefixedHeartbeat(buf)->UnPack(res));
}

}  // namespace heartbeat
}  // namespace aimm

#endif  // FLATBUFFERS_GENERATED_HEARTBEAT_AIMM_HEARTBEAT_H_
