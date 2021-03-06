// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game_to_gate.proto

#ifndef PROTOBUF_game_5fto_5fgate_2eproto__INCLUDED
#define PROTOBUF_game_5fto_5fgate_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "client.pb.h"
// @@protoc_insertion_point(includes)

namespace Msg {
namespace Protocol {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_game_5fto_5fgate_2eproto();
void protobuf_AssignDesc_game_5fto_5fgate_2eproto();
void protobuf_ShutdownFile_game_5fto_5fgate_2eproto();

class RouteFromClientMsg;
class RouteToClientMsg;
class RouteLoginRequest;
class ClientDisconnectMsg;

enum GateToGameMsgId {
  GateToGame_RouteFromClient = 1,
  GateToGame_RouteLoginRequest = 2,
  GateToGame_ClientDisconnect = 3
};
bool GateToGameMsgId_IsValid(int value);
const GateToGameMsgId GateToGameMsgId_MIN = GateToGame_RouteFromClient;
const GateToGameMsgId GateToGameMsgId_MAX = GateToGame_ClientDisconnect;
const int GateToGameMsgId_ARRAYSIZE = GateToGameMsgId_MAX + 1;

const ::google::protobuf::EnumDescriptor* GateToGameMsgId_descriptor();
inline const ::std::string& GateToGameMsgId_Name(GateToGameMsgId value) {
  return ::google::protobuf::internal::NameOfEnum(
    GateToGameMsgId_descriptor(), value);
}
inline bool GateToGameMsgId_Parse(
    const ::std::string& name, GateToGameMsgId* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GateToGameMsgId>(
    GateToGameMsgId_descriptor(), name, value);
}
enum GameToGateMsgId {
  GameToGate_RouteToClient = 1
};
bool GameToGateMsgId_IsValid(int value);
const GameToGateMsgId GameToGateMsgId_MIN = GameToGate_RouteToClient;
const GameToGateMsgId GameToGateMsgId_MAX = GameToGate_RouteToClient;
const int GameToGateMsgId_ARRAYSIZE = GameToGateMsgId_MAX + 1;

const ::google::protobuf::EnumDescriptor* GameToGateMsgId_descriptor();
inline const ::std::string& GameToGateMsgId_Name(GameToGateMsgId value) {
  return ::google::protobuf::internal::NameOfEnum(
    GameToGateMsgId_descriptor(), value);
}
inline bool GameToGateMsgId_Parse(
    const ::std::string& name, GameToGateMsgId* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GameToGateMsgId>(
    GameToGateMsgId_descriptor(), name, value);
}
// ===================================================================

class RouteFromClientMsg : public ::google::protobuf::Message {
 public:
  RouteFromClientMsg();
  virtual ~RouteFromClientMsg();

  RouteFromClientMsg(const RouteFromClientMsg& from);

  inline RouteFromClientMsg& operator=(const RouteFromClientMsg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RouteFromClientMsg& default_instance();

  void Swap(RouteFromClientMsg* other);

  // implements Message ----------------------------------------------

  RouteFromClientMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RouteFromClientMsg& from);
  void MergeFrom(const RouteFromClientMsg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 client_id = 1;
  inline bool has_client_id() const;
  inline void clear_client_id();
  static const int kClientIdFieldNumber = 1;
  inline ::google::protobuf::int32 client_id() const;
  inline void set_client_id(::google::protobuf::int32 value);

  // required int32 msg_id = 2;
  inline bool has_msg_id() const;
  inline void clear_msg_id();
  static const int kMsgIdFieldNumber = 2;
  inline ::google::protobuf::int32 msg_id() const;
  inline void set_msg_id(::google::protobuf::int32 value);

  // required string msg = 3;
  inline bool has_msg() const;
  inline void clear_msg();
  static const int kMsgFieldNumber = 3;
  inline const ::std::string& msg() const;
  inline void set_msg(const ::std::string& value);
  inline void set_msg(const char* value);
  inline void set_msg(const char* value, size_t size);
  inline ::std::string* mutable_msg();
  inline ::std::string* release_msg();
  inline void set_allocated_msg(::std::string* msg);

  // @@protoc_insertion_point(class_scope:Msg.Protocol.RouteFromClientMsg)
 private:
  inline void set_has_client_id();
  inline void clear_has_client_id();
  inline void set_has_msg_id();
  inline void clear_has_msg_id();
  inline void set_has_msg();
  inline void clear_has_msg();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 client_id_;
  ::google::protobuf::int32 msg_id_;
  ::std::string* msg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_AssignDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_ShutdownFile_game_5fto_5fgate_2eproto();

  void InitAsDefaultInstance();
  static RouteFromClientMsg* default_instance_;
};
// -------------------------------------------------------------------

class RouteToClientMsg : public ::google::protobuf::Message {
 public:
  RouteToClientMsg();
  virtual ~RouteToClientMsg();

  RouteToClientMsg(const RouteToClientMsg& from);

  inline RouteToClientMsg& operator=(const RouteToClientMsg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RouteToClientMsg& default_instance();

  void Swap(RouteToClientMsg* other);

  // implements Message ----------------------------------------------

  RouteToClientMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RouteToClientMsg& from);
  void MergeFrom(const RouteToClientMsg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 client_id = 1;
  inline bool has_client_id() const;
  inline void clear_client_id();
  static const int kClientIdFieldNumber = 1;
  inline ::google::protobuf::int32 client_id() const;
  inline void set_client_id(::google::protobuf::int32 value);

  // required int32 msg_id = 2;
  inline bool has_msg_id() const;
  inline void clear_msg_id();
  static const int kMsgIdFieldNumber = 2;
  inline ::google::protobuf::int32 msg_id() const;
  inline void set_msg_id(::google::protobuf::int32 value);

  // required string msg = 3;
  inline bool has_msg() const;
  inline void clear_msg();
  static const int kMsgFieldNumber = 3;
  inline const ::std::string& msg() const;
  inline void set_msg(const ::std::string& value);
  inline void set_msg(const char* value);
  inline void set_msg(const char* value, size_t size);
  inline ::std::string* mutable_msg();
  inline ::std::string* release_msg();
  inline void set_allocated_msg(::std::string* msg);

  // @@protoc_insertion_point(class_scope:Msg.Protocol.RouteToClientMsg)
 private:
  inline void set_has_client_id();
  inline void clear_has_client_id();
  inline void set_has_msg_id();
  inline void clear_has_msg_id();
  inline void set_has_msg();
  inline void clear_has_msg();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 client_id_;
  ::google::protobuf::int32 msg_id_;
  ::std::string* msg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_AssignDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_ShutdownFile_game_5fto_5fgate_2eproto();

  void InitAsDefaultInstance();
  static RouteToClientMsg* default_instance_;
};
// -------------------------------------------------------------------

class RouteLoginRequest : public ::google::protobuf::Message {
 public:
  RouteLoginRequest();
  virtual ~RouteLoginRequest();

  RouteLoginRequest(const RouteLoginRequest& from);

  inline RouteLoginRequest& operator=(const RouteLoginRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RouteLoginRequest& default_instance();

  void Swap(RouteLoginRequest* other);

  // implements Message ----------------------------------------------

  RouteLoginRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RouteLoginRequest& from);
  void MergeFrom(const RouteLoginRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 client_id = 1;
  inline bool has_client_id() const;
  inline void clear_client_id();
  static const int kClientIdFieldNumber = 1;
  inline ::google::protobuf::int32 client_id() const;
  inline void set_client_id(::google::protobuf::int32 value);

  // required .Msg.Protocol.LoginReq loginReq = 2;
  inline bool has_loginreq() const;
  inline void clear_loginreq();
  static const int kLoginReqFieldNumber = 2;
  inline const ::Msg::Protocol::LoginReq& loginreq() const;
  inline ::Msg::Protocol::LoginReq* mutable_loginreq();
  inline ::Msg::Protocol::LoginReq* release_loginreq();
  inline void set_allocated_loginreq(::Msg::Protocol::LoginReq* loginreq);

  // @@protoc_insertion_point(class_scope:Msg.Protocol.RouteLoginRequest)
 private:
  inline void set_has_client_id();
  inline void clear_has_client_id();
  inline void set_has_loginreq();
  inline void clear_has_loginreq();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::Msg::Protocol::LoginReq* loginreq_;
  ::google::protobuf::int32 client_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_AssignDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_ShutdownFile_game_5fto_5fgate_2eproto();

  void InitAsDefaultInstance();
  static RouteLoginRequest* default_instance_;
};
// -------------------------------------------------------------------

class ClientDisconnectMsg : public ::google::protobuf::Message {
 public:
  ClientDisconnectMsg();
  virtual ~ClientDisconnectMsg();

  ClientDisconnectMsg(const ClientDisconnectMsg& from);

  inline ClientDisconnectMsg& operator=(const ClientDisconnectMsg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ClientDisconnectMsg& default_instance();

  void Swap(ClientDisconnectMsg* other);

  // implements Message ----------------------------------------------

  ClientDisconnectMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ClientDisconnectMsg& from);
  void MergeFrom(const ClientDisconnectMsg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 client_id = 1;
  inline bool has_client_id() const;
  inline void clear_client_id();
  static const int kClientIdFieldNumber = 1;
  inline ::google::protobuf::int32 client_id() const;
  inline void set_client_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Msg.Protocol.ClientDisconnectMsg)
 private:
  inline void set_has_client_id();
  inline void clear_has_client_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 client_id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_AssignDesc_game_5fto_5fgate_2eproto();
  friend void protobuf_ShutdownFile_game_5fto_5fgate_2eproto();

  void InitAsDefaultInstance();
  static ClientDisconnectMsg* default_instance_;
};
// ===================================================================


// ===================================================================

// RouteFromClientMsg

// required int32 client_id = 1;
inline bool RouteFromClientMsg::has_client_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RouteFromClientMsg::set_has_client_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RouteFromClientMsg::clear_has_client_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RouteFromClientMsg::clear_client_id() {
  client_id_ = 0;
  clear_has_client_id();
}
inline ::google::protobuf::int32 RouteFromClientMsg::client_id() const {
  return client_id_;
}
inline void RouteFromClientMsg::set_client_id(::google::protobuf::int32 value) {
  set_has_client_id();
  client_id_ = value;
}

// required int32 msg_id = 2;
inline bool RouteFromClientMsg::has_msg_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RouteFromClientMsg::set_has_msg_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RouteFromClientMsg::clear_has_msg_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RouteFromClientMsg::clear_msg_id() {
  msg_id_ = 0;
  clear_has_msg_id();
}
inline ::google::protobuf::int32 RouteFromClientMsg::msg_id() const {
  return msg_id_;
}
inline void RouteFromClientMsg::set_msg_id(::google::protobuf::int32 value) {
  set_has_msg_id();
  msg_id_ = value;
}

// required string msg = 3;
inline bool RouteFromClientMsg::has_msg() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RouteFromClientMsg::set_has_msg() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RouteFromClientMsg::clear_has_msg() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RouteFromClientMsg::clear_msg() {
  if (msg_ != &::google::protobuf::internal::kEmptyString) {
    msg_->clear();
  }
  clear_has_msg();
}
inline const ::std::string& RouteFromClientMsg::msg() const {
  return *msg_;
}
inline void RouteFromClientMsg::set_msg(const ::std::string& value) {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  msg_->assign(value);
}
inline void RouteFromClientMsg::set_msg(const char* value) {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  msg_->assign(value);
}
inline void RouteFromClientMsg::set_msg(const char* value, size_t size) {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  msg_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* RouteFromClientMsg::mutable_msg() {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  return msg_;
}
inline ::std::string* RouteFromClientMsg::release_msg() {
  clear_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msg_;
    msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void RouteFromClientMsg::set_allocated_msg(::std::string* msg) {
  if (msg_ != &::google::protobuf::internal::kEmptyString) {
    delete msg_;
  }
  if (msg) {
    set_has_msg();
    msg_ = msg;
  } else {
    clear_has_msg();
    msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// RouteToClientMsg

// required int32 client_id = 1;
inline bool RouteToClientMsg::has_client_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RouteToClientMsg::set_has_client_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RouteToClientMsg::clear_has_client_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RouteToClientMsg::clear_client_id() {
  client_id_ = 0;
  clear_has_client_id();
}
inline ::google::protobuf::int32 RouteToClientMsg::client_id() const {
  return client_id_;
}
inline void RouteToClientMsg::set_client_id(::google::protobuf::int32 value) {
  set_has_client_id();
  client_id_ = value;
}

// required int32 msg_id = 2;
inline bool RouteToClientMsg::has_msg_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RouteToClientMsg::set_has_msg_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RouteToClientMsg::clear_has_msg_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RouteToClientMsg::clear_msg_id() {
  msg_id_ = 0;
  clear_has_msg_id();
}
inline ::google::protobuf::int32 RouteToClientMsg::msg_id() const {
  return msg_id_;
}
inline void RouteToClientMsg::set_msg_id(::google::protobuf::int32 value) {
  set_has_msg_id();
  msg_id_ = value;
}

// required string msg = 3;
inline bool RouteToClientMsg::has_msg() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RouteToClientMsg::set_has_msg() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RouteToClientMsg::clear_has_msg() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RouteToClientMsg::clear_msg() {
  if (msg_ != &::google::protobuf::internal::kEmptyString) {
    msg_->clear();
  }
  clear_has_msg();
}
inline const ::std::string& RouteToClientMsg::msg() const {
  return *msg_;
}
inline void RouteToClientMsg::set_msg(const ::std::string& value) {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  msg_->assign(value);
}
inline void RouteToClientMsg::set_msg(const char* value) {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  msg_->assign(value);
}
inline void RouteToClientMsg::set_msg(const char* value, size_t size) {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  msg_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* RouteToClientMsg::mutable_msg() {
  set_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    msg_ = new ::std::string;
  }
  return msg_;
}
inline ::std::string* RouteToClientMsg::release_msg() {
  clear_has_msg();
  if (msg_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msg_;
    msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void RouteToClientMsg::set_allocated_msg(::std::string* msg) {
  if (msg_ != &::google::protobuf::internal::kEmptyString) {
    delete msg_;
  }
  if (msg) {
    set_has_msg();
    msg_ = msg;
  } else {
    clear_has_msg();
    msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// RouteLoginRequest

// required int32 client_id = 1;
inline bool RouteLoginRequest::has_client_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RouteLoginRequest::set_has_client_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RouteLoginRequest::clear_has_client_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RouteLoginRequest::clear_client_id() {
  client_id_ = 0;
  clear_has_client_id();
}
inline ::google::protobuf::int32 RouteLoginRequest::client_id() const {
  return client_id_;
}
inline void RouteLoginRequest::set_client_id(::google::protobuf::int32 value) {
  set_has_client_id();
  client_id_ = value;
}

// required .Msg.Protocol.LoginReq loginReq = 2;
inline bool RouteLoginRequest::has_loginreq() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RouteLoginRequest::set_has_loginreq() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RouteLoginRequest::clear_has_loginreq() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RouteLoginRequest::clear_loginreq() {
  if (loginreq_ != NULL) loginreq_->::Msg::Protocol::LoginReq::Clear();
  clear_has_loginreq();
}
inline const ::Msg::Protocol::LoginReq& RouteLoginRequest::loginreq() const {
  return loginreq_ != NULL ? *loginreq_ : *default_instance_->loginreq_;
}
inline ::Msg::Protocol::LoginReq* RouteLoginRequest::mutable_loginreq() {
  set_has_loginreq();
  if (loginreq_ == NULL) loginreq_ = new ::Msg::Protocol::LoginReq;
  return loginreq_;
}
inline ::Msg::Protocol::LoginReq* RouteLoginRequest::release_loginreq() {
  clear_has_loginreq();
  ::Msg::Protocol::LoginReq* temp = loginreq_;
  loginreq_ = NULL;
  return temp;
}
inline void RouteLoginRequest::set_allocated_loginreq(::Msg::Protocol::LoginReq* loginreq) {
  delete loginreq_;
  loginreq_ = loginreq;
  if (loginreq) {
    set_has_loginreq();
  } else {
    clear_has_loginreq();
  }
}

// -------------------------------------------------------------------

// ClientDisconnectMsg

// required int32 client_id = 1;
inline bool ClientDisconnectMsg::has_client_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ClientDisconnectMsg::set_has_client_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ClientDisconnectMsg::clear_has_client_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ClientDisconnectMsg::clear_client_id() {
  client_id_ = 0;
  clear_has_client_id();
}
inline ::google::protobuf::int32 ClientDisconnectMsg::client_id() const {
  return client_id_;
}
inline void ClientDisconnectMsg::set_client_id(::google::protobuf::int32 value) {
  set_has_client_id();
  client_id_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol
}  // namespace Msg

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Msg::Protocol::GateToGameMsgId>() {
  return ::Msg::Protocol::GateToGameMsgId_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Msg::Protocol::GameToGateMsgId>() {
  return ::Msg::Protocol::GameToGateMsgId_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_game_5fto_5fgate_2eproto__INCLUDED
