// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ApiData.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ApiData.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace fantasybit {

namespace {

const ::google::protobuf::Descriptor* FantasyNameHash_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  FantasyNameHash_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ApiData_2eproto() {
  protobuf_AddDesc_ApiData_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ApiData.proto");
  GOOGLE_CHECK(file != NULL);
  FantasyNameHash_descriptor_ = file->message_type(0);
  static const int FantasyNameHash_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FantasyNameHash, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FantasyNameHash, hash_),
  };
  FantasyNameHash_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      FantasyNameHash_descriptor_,
      FantasyNameHash::default_instance_,
      FantasyNameHash_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FantasyNameHash, _has_bits_[0]),
      -1,
      -1,
      sizeof(FantasyNameHash),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FantasyNameHash, _internal_metadata_),
      -1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ApiData_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      FantasyNameHash_descriptor_, &FantasyNameHash::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ApiData_2eproto() {
  delete FantasyNameHash::default_instance_;
  delete FantasyNameHash_reflection_;
}

void protobuf_AddDesc_ApiData_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rApiData.proto\022\nfantasybit\"-\n\017FantasyNa"
    "meHash\022\014\n\004name\030\n \001(\t\022\014\n\004hash\030\036 \001(\004", 74);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ApiData.proto", &protobuf_RegisterTypes);
  FantasyNameHash::default_instance_ = new FantasyNameHash();
  FantasyNameHash::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ApiData_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ApiData_2eproto {
  StaticDescriptorInitializer_ApiData_2eproto() {
    protobuf_AddDesc_ApiData_2eproto();
  }
} static_descriptor_initializer_ApiData_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD;
static void MergeFromFail(int line) {
  GOOGLE_CHECK(false) << __FILE__ << ":" << line;
}

}  // namespace


// ===================================================================

#ifndef _MSC_VER
const int FantasyNameHash::kNameFieldNumber;
const int FantasyNameHash::kHashFieldNumber;
#endif  // !_MSC_VER

FantasyNameHash::FantasyNameHash()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:fantasybit.FantasyNameHash)
}

void FantasyNameHash::InitAsDefaultInstance() {
}

FantasyNameHash::FantasyNameHash(const FantasyNameHash& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:fantasybit.FantasyNameHash)
}

void FantasyNameHash::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  hash_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FantasyNameHash::~FantasyNameHash() {
  // @@protoc_insertion_point(destructor:fantasybit.FantasyNameHash)
  SharedDtor();
}

void FantasyNameHash::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != default_instance_) {
  }
}

void FantasyNameHash::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* FantasyNameHash::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FantasyNameHash_descriptor_;
}

const FantasyNameHash& FantasyNameHash::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ApiData_2eproto();
  return *default_instance_;
}

FantasyNameHash* FantasyNameHash::default_instance_ = NULL;

FantasyNameHash* FantasyNameHash::New(::google::protobuf::Arena* arena) const {
  FantasyNameHash* n = new FantasyNameHash;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void FantasyNameHash::Clear() {
  if (_has_bits_[0 / 32] & 3u) {
    if (has_name()) {
      name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    hash_ = GOOGLE_ULONGLONG(0);
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool FantasyNameHash::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:fantasybit.FantasyNameHash)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(16383);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string name = 10;
      case 10: {
        if (tag == 82) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "fantasybit.FantasyNameHash.name");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(240)) goto parse_hash;
        break;
      }

      // optional uint64 hash = 30;
      case 30: {
        if (tag == 240) {
         parse_hash:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &hash_)));
          set_has_hash();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:fantasybit.FantasyNameHash)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:fantasybit.FantasyNameHash)
  return false;
#undef DO_
}

void FantasyNameHash::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:fantasybit.FantasyNameHash)
  // optional string name = 10;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "fantasybit.FantasyNameHash.name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      10, this->name(), output);
  }

  // optional uint64 hash = 30;
  if (has_hash()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(30, this->hash(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:fantasybit.FantasyNameHash)
}

::google::protobuf::uint8* FantasyNameHash::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:fantasybit.FantasyNameHash)
  // optional string name = 10;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "fantasybit.FantasyNameHash.name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        10, this->name(), target);
  }

  // optional uint64 hash = 30;
  if (has_hash()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(30, this->hash(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:fantasybit.FantasyNameHash)
  return target;
}

int FantasyNameHash::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & 3u) {
    // optional string name = 10;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // optional uint64 hash = 30;
    if (has_hash()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->hash());
    }

  }
  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void FantasyNameHash::MergeFrom(const ::google::protobuf::Message& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const FantasyNameHash* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const FantasyNameHash>(
          &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void FantasyNameHash::MergeFrom(const FantasyNameHash& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_name()) {
      set_has_name();
      name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
    }
    if (from.has_hash()) {
      set_hash(from.hash());
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void FantasyNameHash::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FantasyNameHash::CopyFrom(const FantasyNameHash& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FantasyNameHash::IsInitialized() const {

  return true;
}

void FantasyNameHash::Swap(FantasyNameHash* other) {
  if (other == this) return;
  InternalSwap(other);
}
void FantasyNameHash::InternalSwap(FantasyNameHash* other) {
  name_.Swap(&other->name_);
  std::swap(hash_, other->hash_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata FantasyNameHash::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = FantasyNameHash_descriptor_;
  metadata.reflection = FantasyNameHash_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// FantasyNameHash

// optional string name = 10;
bool FantasyNameHash::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void FantasyNameHash::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
void FantasyNameHash::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
void FantasyNameHash::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_name();
}
 const ::std::string& FantasyNameHash::name() const {
  // @@protoc_insertion_point(field_get:fantasybit.FantasyNameHash.name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void FantasyNameHash::set_name(const ::std::string& value) {
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:fantasybit.FantasyNameHash.name)
}
 void FantasyNameHash::set_name(const char* value) {
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:fantasybit.FantasyNameHash.name)
}
 void FantasyNameHash::set_name(const char* value, size_t size) {
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:fantasybit.FantasyNameHash.name)
}
 ::std::string* FantasyNameHash::mutable_name() {
  set_has_name();
  // @@protoc_insertion_point(field_mutable:fantasybit.FantasyNameHash.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* FantasyNameHash::release_name() {
  clear_has_name();
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void FantasyNameHash::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    set_has_name();
  } else {
    clear_has_name();
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:fantasybit.FantasyNameHash.name)
}

// optional uint64 hash = 30;
bool FantasyNameHash::has_hash() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void FantasyNameHash::set_has_hash() {
  _has_bits_[0] |= 0x00000002u;
}
void FantasyNameHash::clear_has_hash() {
  _has_bits_[0] &= ~0x00000002u;
}
void FantasyNameHash::clear_hash() {
  hash_ = GOOGLE_ULONGLONG(0);
  clear_has_hash();
}
 ::google::protobuf::uint64 FantasyNameHash::hash() const {
  // @@protoc_insertion_point(field_get:fantasybit.FantasyNameHash.hash)
  return hash_;
}
 void FantasyNameHash::set_hash(::google::protobuf::uint64 value) {
  set_has_hash();
  hash_ = value;
  // @@protoc_insertion_point(field_set:fantasybit.FantasyNameHash.hash)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace fantasybit

// @@protoc_insertion_point(global_scope)
