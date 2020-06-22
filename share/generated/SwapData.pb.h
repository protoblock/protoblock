// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SwapData.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_SwapData_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_SwapData_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_SwapData_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_SwapData_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_SwapData_2eproto;
namespace fantasybit {
class SwapOrder;
class SwapOrderDefaultTypeInternal;
extern SwapOrderDefaultTypeInternal _SwapOrder_default_instance_;
}  // namespace fantasybit
PROTOBUF_NAMESPACE_OPEN
template<> ::fantasybit::SwapOrder* Arena::CreateMaybeMessage<::fantasybit::SwapOrder>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace fantasybit {

// ===================================================================

class SwapOrder PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:fantasybit.SwapOrder) */ {
 public:
  inline SwapOrder() : SwapOrder(nullptr) {};
  virtual ~SwapOrder();

  SwapOrder(const SwapOrder& from);
  SwapOrder(SwapOrder&& from) noexcept
    : SwapOrder() {
    *this = ::std::move(from);
  }

  inline SwapOrder& operator=(const SwapOrder& from) {
    CopyFrom(from);
    return *this;
  }
  inline SwapOrder& operator=(SwapOrder&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const SwapOrder& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SwapOrder* internal_default_instance() {
    return reinterpret_cast<const SwapOrder*>(
               &_SwapOrder_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SwapOrder& a, SwapOrder& b) {
    a.Swap(&b);
  }
  inline void Swap(SwapOrder* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SwapOrder* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SwapOrder* New() const final {
    return CreateMaybeMessage<SwapOrder>(nullptr);
  }

  SwapOrder* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SwapOrder>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SwapOrder& from);
  void MergeFrom(const SwapOrder& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SwapOrder* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "fantasybit.SwapOrder";
  }
  protected:
  explicit SwapOrder(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_SwapData_2eproto);
    return ::descriptor_table_SwapData_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFnameFieldNumber = 80,
    kMsgFieldNumber = 90,
    kRefFieldNumber = 100,
    kDirectedFieldNumber = 110,
    kIsaskFieldNumber = 10,
    kDoubleSpentFieldNumber = 120,
    kSatoshiMinFieldNumber = 20,
    kSatoshiMaxFieldNumber = 30,
    kRateFieldNumber = 40,
    kOpenqFieldNumber = 50,
    kPendqFieldNumber = 60,
    kFillqFieldNumber = 70,
  };
  // optional string fname = 80;
  bool has_fname() const;
  private:
  bool _internal_has_fname() const;
  public:
  void clear_fname();
  const std::string& fname() const;
  void set_fname(const std::string& value);
  void set_fname(std::string&& value);
  void set_fname(const char* value);
  void set_fname(const char* value, size_t size);
  std::string* mutable_fname();
  std::string* release_fname();
  void set_allocated_fname(std::string* fname);
  private:
  const std::string& _internal_fname() const;
  void _internal_set_fname(const std::string& value);
  std::string* _internal_mutable_fname();
  public:

  // optional string msg = 90;
  bool has_msg() const;
  private:
  bool _internal_has_msg() const;
  public:
  void clear_msg();
  const std::string& msg() const;
  void set_msg(const std::string& value);
  void set_msg(std::string&& value);
  void set_msg(const char* value);
  void set_msg(const char* value, size_t size);
  std::string* mutable_msg();
  std::string* release_msg();
  void set_allocated_msg(std::string* msg);
  private:
  const std::string& _internal_msg() const;
  void _internal_set_msg(const std::string& value);
  std::string* _internal_mutable_msg();
  public:

  // optional string ref = 100;
  bool has_ref() const;
  private:
  bool _internal_has_ref() const;
  public:
  void clear_ref();
  const std::string& ref() const;
  void set_ref(const std::string& value);
  void set_ref(std::string&& value);
  void set_ref(const char* value);
  void set_ref(const char* value, size_t size);
  std::string* mutable_ref();
  std::string* release_ref();
  void set_allocated_ref(std::string* ref);
  private:
  const std::string& _internal_ref() const;
  void _internal_set_ref(const std::string& value);
  std::string* _internal_mutable_ref();
  public:

  // optional string directed = 110;
  bool has_directed() const;
  private:
  bool _internal_has_directed() const;
  public:
  void clear_directed();
  const std::string& directed() const;
  void set_directed(const std::string& value);
  void set_directed(std::string&& value);
  void set_directed(const char* value);
  void set_directed(const char* value, size_t size);
  std::string* mutable_directed();
  std::string* release_directed();
  void set_allocated_directed(std::string* directed);
  private:
  const std::string& _internal_directed() const;
  void _internal_set_directed(const std::string& value);
  std::string* _internal_mutable_directed();
  public:

  // optional bool isask = 10;
  bool has_isask() const;
  private:
  bool _internal_has_isask() const;
  public:
  void clear_isask();
  bool isask() const;
  void set_isask(bool value);
  private:
  bool _internal_isask() const;
  void _internal_set_isask(bool value);
  public:

  // optional bool double_spent = 120;
  bool has_double_spent() const;
  private:
  bool _internal_has_double_spent() const;
  public:
  void clear_double_spent();
  bool double_spent() const;
  void set_double_spent(bool value);
  private:
  bool _internal_double_spent() const;
  void _internal_set_double_spent(bool value);
  public:

  // optional uint64 satoshi_min = 20;
  bool has_satoshi_min() const;
  private:
  bool _internal_has_satoshi_min() const;
  public:
  void clear_satoshi_min();
  ::PROTOBUF_NAMESPACE_ID::uint64 satoshi_min() const;
  void set_satoshi_min(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_satoshi_min() const;
  void _internal_set_satoshi_min(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // optional uint64 satoshi_max = 30;
  bool has_satoshi_max() const;
  private:
  bool _internal_has_satoshi_max() const;
  public:
  void clear_satoshi_max();
  ::PROTOBUF_NAMESPACE_ID::uint64 satoshi_max() const;
  void set_satoshi_max(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_satoshi_max() const;
  void _internal_set_satoshi_max(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // optional uint64 rate = 40;
  bool has_rate() const;
  private:
  bool _internal_has_rate() const;
  public:
  void clear_rate();
  ::PROTOBUF_NAMESPACE_ID::uint64 rate() const;
  void set_rate(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_rate() const;
  void _internal_set_rate(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // optional uint64 openq = 50;
  bool has_openq() const;
  private:
  bool _internal_has_openq() const;
  public:
  void clear_openq();
  ::PROTOBUF_NAMESPACE_ID::uint64 openq() const;
  void set_openq(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_openq() const;
  void _internal_set_openq(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // optional uint64 pendq = 60;
  bool has_pendq() const;
  private:
  bool _internal_has_pendq() const;
  public:
  void clear_pendq();
  ::PROTOBUF_NAMESPACE_ID::uint64 pendq() const;
  void set_pendq(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_pendq() const;
  void _internal_set_pendq(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // optional uint64 fillq = 70;
  bool has_fillq() const;
  private:
  bool _internal_has_fillq() const;
  public:
  void clear_fillq();
  ::PROTOBUF_NAMESPACE_ID::uint64 fillq() const;
  void set_fillq(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_fillq() const;
  void _internal_set_fillq(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // @@protoc_insertion_point(class_scope:fantasybit.SwapOrder)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr fname_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr msg_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr ref_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr directed_;
  bool isask_;
  bool double_spent_;
  ::PROTOBUF_NAMESPACE_ID::uint64 satoshi_min_;
  ::PROTOBUF_NAMESPACE_ID::uint64 satoshi_max_;
  ::PROTOBUF_NAMESPACE_ID::uint64 rate_;
  ::PROTOBUF_NAMESPACE_ID::uint64 openq_;
  ::PROTOBUF_NAMESPACE_ID::uint64 pendq_;
  ::PROTOBUF_NAMESPACE_ID::uint64 fillq_;
  friend struct ::TableStruct_SwapData_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SwapOrder

// optional bool isask = 10;
inline bool SwapOrder::_internal_has_isask() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  return value;
}
inline bool SwapOrder::has_isask() const {
  return _internal_has_isask();
}
inline void SwapOrder::clear_isask() {
  isask_ = false;
  _has_bits_[0] &= ~0x00000010u;
}
inline bool SwapOrder::_internal_isask() const {
  return isask_;
}
inline bool SwapOrder::isask() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.isask)
  return _internal_isask();
}
inline void SwapOrder::_internal_set_isask(bool value) {
  _has_bits_[0] |= 0x00000010u;
  isask_ = value;
}
inline void SwapOrder::set_isask(bool value) {
  _internal_set_isask(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.isask)
}

// optional uint64 satoshi_min = 20;
inline bool SwapOrder::_internal_has_satoshi_min() const {
  bool value = (_has_bits_[0] & 0x00000040u) != 0;
  return value;
}
inline bool SwapOrder::has_satoshi_min() const {
  return _internal_has_satoshi_min();
}
inline void SwapOrder::clear_satoshi_min() {
  satoshi_min_ = PROTOBUF_ULONGLONG(0);
  _has_bits_[0] &= ~0x00000040u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::_internal_satoshi_min() const {
  return satoshi_min_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::satoshi_min() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.satoshi_min)
  return _internal_satoshi_min();
}
inline void SwapOrder::_internal_set_satoshi_min(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000040u;
  satoshi_min_ = value;
}
inline void SwapOrder::set_satoshi_min(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_satoshi_min(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.satoshi_min)
}

// optional uint64 satoshi_max = 30;
inline bool SwapOrder::_internal_has_satoshi_max() const {
  bool value = (_has_bits_[0] & 0x00000080u) != 0;
  return value;
}
inline bool SwapOrder::has_satoshi_max() const {
  return _internal_has_satoshi_max();
}
inline void SwapOrder::clear_satoshi_max() {
  satoshi_max_ = PROTOBUF_ULONGLONG(0);
  _has_bits_[0] &= ~0x00000080u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::_internal_satoshi_max() const {
  return satoshi_max_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::satoshi_max() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.satoshi_max)
  return _internal_satoshi_max();
}
inline void SwapOrder::_internal_set_satoshi_max(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000080u;
  satoshi_max_ = value;
}
inline void SwapOrder::set_satoshi_max(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_satoshi_max(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.satoshi_max)
}

// optional uint64 rate = 40;
inline bool SwapOrder::_internal_has_rate() const {
  bool value = (_has_bits_[0] & 0x00000100u) != 0;
  return value;
}
inline bool SwapOrder::has_rate() const {
  return _internal_has_rate();
}
inline void SwapOrder::clear_rate() {
  rate_ = PROTOBUF_ULONGLONG(0);
  _has_bits_[0] &= ~0x00000100u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::_internal_rate() const {
  return rate_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::rate() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.rate)
  return _internal_rate();
}
inline void SwapOrder::_internal_set_rate(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000100u;
  rate_ = value;
}
inline void SwapOrder::set_rate(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_rate(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.rate)
}

// optional uint64 openq = 50;
inline bool SwapOrder::_internal_has_openq() const {
  bool value = (_has_bits_[0] & 0x00000200u) != 0;
  return value;
}
inline bool SwapOrder::has_openq() const {
  return _internal_has_openq();
}
inline void SwapOrder::clear_openq() {
  openq_ = PROTOBUF_ULONGLONG(0);
  _has_bits_[0] &= ~0x00000200u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::_internal_openq() const {
  return openq_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::openq() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.openq)
  return _internal_openq();
}
inline void SwapOrder::_internal_set_openq(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000200u;
  openq_ = value;
}
inline void SwapOrder::set_openq(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_openq(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.openq)
}

// optional uint64 pendq = 60;
inline bool SwapOrder::_internal_has_pendq() const {
  bool value = (_has_bits_[0] & 0x00000400u) != 0;
  return value;
}
inline bool SwapOrder::has_pendq() const {
  return _internal_has_pendq();
}
inline void SwapOrder::clear_pendq() {
  pendq_ = PROTOBUF_ULONGLONG(0);
  _has_bits_[0] &= ~0x00000400u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::_internal_pendq() const {
  return pendq_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::pendq() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.pendq)
  return _internal_pendq();
}
inline void SwapOrder::_internal_set_pendq(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000400u;
  pendq_ = value;
}
inline void SwapOrder::set_pendq(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_pendq(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.pendq)
}

// optional uint64 fillq = 70;
inline bool SwapOrder::_internal_has_fillq() const {
  bool value = (_has_bits_[0] & 0x00000800u) != 0;
  return value;
}
inline bool SwapOrder::has_fillq() const {
  return _internal_has_fillq();
}
inline void SwapOrder::clear_fillq() {
  fillq_ = PROTOBUF_ULONGLONG(0);
  _has_bits_[0] &= ~0x00000800u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::_internal_fillq() const {
  return fillq_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SwapOrder::fillq() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.fillq)
  return _internal_fillq();
}
inline void SwapOrder::_internal_set_fillq(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _has_bits_[0] |= 0x00000800u;
  fillq_ = value;
}
inline void SwapOrder::set_fillq(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_fillq(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.fillq)
}

// optional string fname = 80;
inline bool SwapOrder::_internal_has_fname() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool SwapOrder::has_fname() const {
  return _internal_has_fname();
}
inline void SwapOrder::clear_fname() {
  fname_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& SwapOrder::fname() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.fname)
  return _internal_fname();
}
inline void SwapOrder::set_fname(const std::string& value) {
  _internal_set_fname(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.fname)
}
inline std::string* SwapOrder::mutable_fname() {
  // @@protoc_insertion_point(field_mutable:fantasybit.SwapOrder.fname)
  return _internal_mutable_fname();
}
inline const std::string& SwapOrder::_internal_fname() const {
  return fname_.Get();
}
inline void SwapOrder::_internal_set_fname(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  fname_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SwapOrder::set_fname(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  fname_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:fantasybit.SwapOrder.fname)
}
inline void SwapOrder::set_fname(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  fname_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:fantasybit.SwapOrder.fname)
}
inline void SwapOrder::set_fname(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000001u;
  fname_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:fantasybit.SwapOrder.fname)
}
inline std::string* SwapOrder::_internal_mutable_fname() {
  _has_bits_[0] |= 0x00000001u;
  return fname_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SwapOrder::release_fname() {
  // @@protoc_insertion_point(field_release:fantasybit.SwapOrder.fname)
  if (!_internal_has_fname()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return fname_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SwapOrder::set_allocated_fname(std::string* fname) {
  if (fname != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  fname_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), fname,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:fantasybit.SwapOrder.fname)
}

// optional string msg = 90;
inline bool SwapOrder::_internal_has_msg() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool SwapOrder::has_msg() const {
  return _internal_has_msg();
}
inline void SwapOrder::clear_msg() {
  msg_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000002u;
}
inline const std::string& SwapOrder::msg() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.msg)
  return _internal_msg();
}
inline void SwapOrder::set_msg(const std::string& value) {
  _internal_set_msg(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.msg)
}
inline std::string* SwapOrder::mutable_msg() {
  // @@protoc_insertion_point(field_mutable:fantasybit.SwapOrder.msg)
  return _internal_mutable_msg();
}
inline const std::string& SwapOrder::_internal_msg() const {
  return msg_.Get();
}
inline void SwapOrder::_internal_set_msg(const std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  msg_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SwapOrder::set_msg(std::string&& value) {
  _has_bits_[0] |= 0x00000002u;
  msg_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:fantasybit.SwapOrder.msg)
}
inline void SwapOrder::set_msg(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000002u;
  msg_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:fantasybit.SwapOrder.msg)
}
inline void SwapOrder::set_msg(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000002u;
  msg_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:fantasybit.SwapOrder.msg)
}
inline std::string* SwapOrder::_internal_mutable_msg() {
  _has_bits_[0] |= 0x00000002u;
  return msg_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SwapOrder::release_msg() {
  // @@protoc_insertion_point(field_release:fantasybit.SwapOrder.msg)
  if (!_internal_has_msg()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000002u;
  return msg_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SwapOrder::set_allocated_msg(std::string* msg) {
  if (msg != nullptr) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  msg_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), msg,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:fantasybit.SwapOrder.msg)
}

// optional string ref = 100;
inline bool SwapOrder::_internal_has_ref() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool SwapOrder::has_ref() const {
  return _internal_has_ref();
}
inline void SwapOrder::clear_ref() {
  ref_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000004u;
}
inline const std::string& SwapOrder::ref() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.ref)
  return _internal_ref();
}
inline void SwapOrder::set_ref(const std::string& value) {
  _internal_set_ref(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.ref)
}
inline std::string* SwapOrder::mutable_ref() {
  // @@protoc_insertion_point(field_mutable:fantasybit.SwapOrder.ref)
  return _internal_mutable_ref();
}
inline const std::string& SwapOrder::_internal_ref() const {
  return ref_.Get();
}
inline void SwapOrder::_internal_set_ref(const std::string& value) {
  _has_bits_[0] |= 0x00000004u;
  ref_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SwapOrder::set_ref(std::string&& value) {
  _has_bits_[0] |= 0x00000004u;
  ref_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:fantasybit.SwapOrder.ref)
}
inline void SwapOrder::set_ref(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000004u;
  ref_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:fantasybit.SwapOrder.ref)
}
inline void SwapOrder::set_ref(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000004u;
  ref_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:fantasybit.SwapOrder.ref)
}
inline std::string* SwapOrder::_internal_mutable_ref() {
  _has_bits_[0] |= 0x00000004u;
  return ref_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SwapOrder::release_ref() {
  // @@protoc_insertion_point(field_release:fantasybit.SwapOrder.ref)
  if (!_internal_has_ref()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000004u;
  return ref_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SwapOrder::set_allocated_ref(std::string* ref) {
  if (ref != nullptr) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  ref_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ref,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:fantasybit.SwapOrder.ref)
}

// optional string directed = 110;
inline bool SwapOrder::_internal_has_directed() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool SwapOrder::has_directed() const {
  return _internal_has_directed();
}
inline void SwapOrder::clear_directed() {
  directed_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _has_bits_[0] &= ~0x00000008u;
}
inline const std::string& SwapOrder::directed() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.directed)
  return _internal_directed();
}
inline void SwapOrder::set_directed(const std::string& value) {
  _internal_set_directed(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.directed)
}
inline std::string* SwapOrder::mutable_directed() {
  // @@protoc_insertion_point(field_mutable:fantasybit.SwapOrder.directed)
  return _internal_mutable_directed();
}
inline const std::string& SwapOrder::_internal_directed() const {
  return directed_.Get();
}
inline void SwapOrder::_internal_set_directed(const std::string& value) {
  _has_bits_[0] |= 0x00000008u;
  directed_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SwapOrder::set_directed(std::string&& value) {
  _has_bits_[0] |= 0x00000008u;
  directed_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:fantasybit.SwapOrder.directed)
}
inline void SwapOrder::set_directed(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000008u;
  directed_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:fantasybit.SwapOrder.directed)
}
inline void SwapOrder::set_directed(const char* value,
    size_t size) {
  _has_bits_[0] |= 0x00000008u;
  directed_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:fantasybit.SwapOrder.directed)
}
inline std::string* SwapOrder::_internal_mutable_directed() {
  _has_bits_[0] |= 0x00000008u;
  return directed_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SwapOrder::release_directed() {
  // @@protoc_insertion_point(field_release:fantasybit.SwapOrder.directed)
  if (!_internal_has_directed()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000008u;
  return directed_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SwapOrder::set_allocated_directed(std::string* directed) {
  if (directed != nullptr) {
    _has_bits_[0] |= 0x00000008u;
  } else {
    _has_bits_[0] &= ~0x00000008u;
  }
  directed_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), directed,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:fantasybit.SwapOrder.directed)
}

// optional bool double_spent = 120;
inline bool SwapOrder::_internal_has_double_spent() const {
  bool value = (_has_bits_[0] & 0x00000020u) != 0;
  return value;
}
inline bool SwapOrder::has_double_spent() const {
  return _internal_has_double_spent();
}
inline void SwapOrder::clear_double_spent() {
  double_spent_ = false;
  _has_bits_[0] &= ~0x00000020u;
}
inline bool SwapOrder::_internal_double_spent() const {
  return double_spent_;
}
inline bool SwapOrder::double_spent() const {
  // @@protoc_insertion_point(field_get:fantasybit.SwapOrder.double_spent)
  return _internal_double_spent();
}
inline void SwapOrder::_internal_set_double_spent(bool value) {
  _has_bits_[0] |= 0x00000020u;
  double_spent_ = value;
}
inline void SwapOrder::set_double_spent(bool value) {
  _internal_set_double_spent(value);
  // @@protoc_insertion_point(field_set:fantasybit.SwapOrder.double_spent)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace fantasybit

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_SwapData_2eproto
