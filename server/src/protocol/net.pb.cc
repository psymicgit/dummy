// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: net.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "net.pb.h"

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

namespace {

const ::google::protobuf::Descriptor* ConnectReq_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ConnectReq_reflection_ = NULL;
const ::google::protobuf::Descriptor* ConnectResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ConnectResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* NetMsgId_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* ConnectResult_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_net_2eproto() {
  protobuf_AddDesc_net_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "net.proto");
  GOOGLE_CHECK(file != NULL);
  ConnectReq_descriptor_ = file->message_type(0);
  static const int ConnectReq_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectReq, svrtype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectReq, authkey_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectReq, svrid_),
  };
  ConnectReq_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ConnectReq_descriptor_,
      ConnectReq::default_instance_,
      ConnectReq_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectReq, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectReq, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ConnectReq));
  ConnectResponse_descriptor_ = file->message_type(1);
  static const int ConnectResponse_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectResponse, ret_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectResponse, svrtype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectResponse, svrid_),
  };
  ConnectResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ConnectResponse_descriptor_,
      ConnectResponse::default_instance_,
      ConnectResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ConnectResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ConnectResponse));
  NetMsgId_descriptor_ = file->enum_type(0);
  ConnectResult_descriptor_ = file->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_net_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ConnectReq_descriptor_, &ConnectReq::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ConnectResponse_descriptor_, &ConnectResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_net_2eproto() {
  delete ConnectReq::default_instance_;
  delete ConnectReq_reflection_;
  delete ConnectResponse::default_instance_;
  delete ConnectResponse_reflection_;
}

void protobuf_AddDesc_net_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\tnet.proto\"=\n\nConnectReq\022\017\n\007svrType\030\001 \002"
    "(\005\022\017\n\007authKey\030\002 \002(\t\022\r\n\005svrId\030\003 \001(\005\"N\n\017Co"
    "nnectResponse\022\033\n\003ret\030\001 \002(\0162\016.ConnectResu"
    "lt\022\017\n\007svrType\030\002 \002(\005\022\r\n\005svrId\030\003 \001(\005*2\n\010Ne"
    "tMsgId\022\022\n\016eConnectSvrReq\020\001\022\022\n\016eConnectSv"
    "rAck\020\002*\214\001\n\rConnectResult\022\016\n\nCONNECT_OK\020\000"
    "\022$\n CONNECT_FAIL_UNKNOWN_SERVER_TYPE\020\001\022\""
    "\n\036CONNECT_FAIL_FOUND_SAME_SERVER\020\002\022!\n\035CO"
    "NNECT_FAIL_AUTH_KEY_INVALID\020\003", 349);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "net.proto", &protobuf_RegisterTypes);
  ConnectReq::default_instance_ = new ConnectReq();
  ConnectResponse::default_instance_ = new ConnectResponse();
  ConnectReq::default_instance_->InitAsDefaultInstance();
  ConnectResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_net_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_net_2eproto {
  StaticDescriptorInitializer_net_2eproto() {
    protobuf_AddDesc_net_2eproto();
  }
} static_descriptor_initializer_net_2eproto_;
const ::google::protobuf::EnumDescriptor* NetMsgId_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return NetMsgId_descriptor_;
}
bool NetMsgId_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* ConnectResult_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ConnectResult_descriptor_;
}
bool ConnectResult_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int ConnectReq::kSvrTypeFieldNumber;
const int ConnectReq::kAuthKeyFieldNumber;
const int ConnectReq::kSvrIdFieldNumber;
#endif  // !_MSC_VER

ConnectReq::ConnectReq()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ConnectReq::InitAsDefaultInstance() {
}

ConnectReq::ConnectReq(const ConnectReq& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ConnectReq::SharedCtor() {
  _cached_size_ = 0;
  svrtype_ = 0;
  authkey_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  svrid_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ConnectReq::~ConnectReq() {
  SharedDtor();
}

void ConnectReq::SharedDtor() {
  if (authkey_ != &::google::protobuf::internal::kEmptyString) {
    delete authkey_;
  }
  if (this != default_instance_) {
  }
}

void ConnectReq::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ConnectReq::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ConnectReq_descriptor_;
}

const ConnectReq& ConnectReq::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_net_2eproto();
  return *default_instance_;
}

ConnectReq* ConnectReq::default_instance_ = NULL;

ConnectReq* ConnectReq::New() const {
  return new ConnectReq;
}

void ConnectReq::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    svrtype_ = 0;
    if (has_authkey()) {
      if (authkey_ != &::google::protobuf::internal::kEmptyString) {
        authkey_->clear();
      }
    }
    svrid_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ConnectReq::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 svrType = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &svrtype_)));
          set_has_svrtype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_authKey;
        break;
      }

      // required string authKey = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_authKey:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_authkey()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->authkey().data(), this->authkey().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_svrId;
        break;
      }

      // optional int32 svrId = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_svrId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &svrid_)));
          set_has_svrid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ConnectReq::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 svrType = 1;
  if (has_svrtype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->svrtype(), output);
  }

  // required string authKey = 2;
  if (has_authkey()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->authkey().data(), this->authkey().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->authkey(), output);
  }

  // optional int32 svrId = 3;
  if (has_svrid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->svrid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ConnectReq::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 svrType = 1;
  if (has_svrtype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->svrtype(), target);
  }

  // required string authKey = 2;
  if (has_authkey()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->authkey().data(), this->authkey().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->authkey(), target);
  }

  // optional int32 svrId = 3;
  if (has_svrid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->svrid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ConnectReq::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 svrType = 1;
    if (has_svrtype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->svrtype());
    }

    // required string authKey = 2;
    if (has_authkey()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->authkey());
    }

    // optional int32 svrId = 3;
    if (has_svrid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->svrid());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ConnectReq::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ConnectReq* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ConnectReq*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ConnectReq::MergeFrom(const ConnectReq& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_svrtype()) {
      set_svrtype(from.svrtype());
    }
    if (from.has_authkey()) {
      set_authkey(from.authkey());
    }
    if (from.has_svrid()) {
      set_svrid(from.svrid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ConnectReq::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ConnectReq::CopyFrom(const ConnectReq& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConnectReq::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void ConnectReq::Swap(ConnectReq* other) {
  if (other != this) {
    std::swap(svrtype_, other->svrtype_);
    std::swap(authkey_, other->authkey_);
    std::swap(svrid_, other->svrid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ConnectReq::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ConnectReq_descriptor_;
  metadata.reflection = ConnectReq_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ConnectResponse::kRetFieldNumber;
const int ConnectResponse::kSvrTypeFieldNumber;
const int ConnectResponse::kSvrIdFieldNumber;
#endif  // !_MSC_VER

ConnectResponse::ConnectResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ConnectResponse::InitAsDefaultInstance() {
}

ConnectResponse::ConnectResponse(const ConnectResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ConnectResponse::SharedCtor() {
  _cached_size_ = 0;
  ret_ = 0;
  svrtype_ = 0;
  svrid_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ConnectResponse::~ConnectResponse() {
  SharedDtor();
}

void ConnectResponse::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ConnectResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ConnectResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ConnectResponse_descriptor_;
}

const ConnectResponse& ConnectResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_net_2eproto();
  return *default_instance_;
}

ConnectResponse* ConnectResponse::default_instance_ = NULL;

ConnectResponse* ConnectResponse::New() const {
  return new ConnectResponse;
}

void ConnectResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ret_ = 0;
    svrtype_ = 0;
    svrid_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ConnectResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .ConnectResult ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::ConnectResult_IsValid(value)) {
            set_ret(static_cast< ::ConnectResult >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_svrType;
        break;
      }

      // required int32 svrType = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_svrType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &svrtype_)));
          set_has_svrtype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_svrId;
        break;
      }

      // optional int32 svrId = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_svrId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &svrid_)));
          set_has_svrid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ConnectResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .ConnectResult ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->ret(), output);
  }

  // required int32 svrType = 2;
  if (has_svrtype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->svrtype(), output);
  }

  // optional int32 svrId = 3;
  if (has_svrid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->svrid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ConnectResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .ConnectResult ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->ret(), target);
  }

  // required int32 svrType = 2;
  if (has_svrtype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->svrtype(), target);
  }

  // optional int32 svrId = 3;
  if (has_svrid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->svrid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ConnectResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .ConnectResult ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->ret());
    }

    // required int32 svrType = 2;
    if (has_svrtype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->svrtype());
    }

    // optional int32 svrId = 3;
    if (has_svrid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->svrid());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ConnectResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ConnectResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ConnectResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ConnectResponse::MergeFrom(const ConnectResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      set_ret(from.ret());
    }
    if (from.has_svrtype()) {
      set_svrtype(from.svrtype());
    }
    if (from.has_svrid()) {
      set_svrid(from.svrid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ConnectResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ConnectResponse::CopyFrom(const ConnectResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConnectResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void ConnectResponse::Swap(ConnectResponse* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    std::swap(svrtype_, other->svrtype_);
    std::swap(svrid_, other->svrid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ConnectResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ConnectResponse_descriptor_;
  metadata.reflection = ConnectResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
