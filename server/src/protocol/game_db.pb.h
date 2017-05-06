// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game_db.proto

#ifndef PROTOBUF_game_5fdb_2eproto__INCLUDED
#define PROTOBUF_game_5fdb_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

namespace Msg {
namespace Protocol {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_game_5fdb_2eproto();
void protobuf_AssignDesc_game_5fdb_2eproto();
void protobuf_ShutdownFile_game_5fdb_2eproto();

class SqlMsg;
class PostSql;

enum GameToDBMsgId {
  eGameToDBMsgIdMin = 2000,
  ePostSql = 2001,
  eGameToDBMsgIdMax = 3000
};
bool GameToDBMsgId_IsValid(int value);
const GameToDBMsgId GameToDBMsgId_MIN = eGameToDBMsgIdMin;
const GameToDBMsgId GameToDBMsgId_MAX = eGameToDBMsgIdMax;
const int GameToDBMsgId_ARRAYSIZE = GameToDBMsgId_MAX + 1;

const ::google::protobuf::EnumDescriptor* GameToDBMsgId_descriptor();
inline const ::std::string& GameToDBMsgId_Name(GameToDBMsgId value) {
  return ::google::protobuf::internal::NameOfEnum(
    GameToDBMsgId_descriptor(), value);
}
inline bool GameToDBMsgId_Parse(
    const ::std::string& name, GameToDBMsgId* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GameToDBMsgId>(
    GameToDBMsgId_descriptor(), name, value);
}
enum SqlType {
  NormalSql = 0,
  PlayerSql = 1,
  GuidSql = 2
};
bool SqlType_IsValid(int value);
const SqlType SqlType_MIN = NormalSql;
const SqlType SqlType_MAX = GuidSql;
const int SqlType_ARRAYSIZE = SqlType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SqlType_descriptor();
inline const ::std::string& SqlType_Name(SqlType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SqlType_descriptor(), value);
}
inline bool SqlType_Parse(
    const ::std::string& name, SqlType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SqlType>(
    SqlType_descriptor(), name, value);
}
enum SqlDBType {
  GameDB = 0,
  LogDB = 1
};
bool SqlDBType_IsValid(int value);
const SqlDBType SqlDBType_MIN = GameDB;
const SqlDBType SqlDBType_MAX = LogDB;
const int SqlDBType_ARRAYSIZE = SqlDBType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SqlDBType_descriptor();
inline const ::std::string& SqlDBType_Name(SqlDBType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SqlDBType_descriptor(), value);
}
inline bool SqlDBType_Parse(
    const ::std::string& name, SqlDBType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SqlDBType>(
    SqlDBType_descriptor(), name, value);
}
// ===================================================================

class SqlMsg : public ::google::protobuf::Message {
 public:
  SqlMsg();
  virtual ~SqlMsg();

  SqlMsg(const SqlMsg& from);

  inline SqlMsg& operator=(const SqlMsg& from) {
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
  static const SqlMsg& default_instance();

  void Swap(SqlMsg* other);

  // implements Message ----------------------------------------------

  SqlMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SqlMsg& from);
  void MergeFrom(const SqlMsg& from);
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

  // required .Msg.Protocol.SqlType sqltype = 1 [default = NormalSql];
  inline bool has_sqltype() const;
  inline void clear_sqltype();
  static const int kSqltypeFieldNumber = 1;
  inline ::Msg::Protocol::SqlType sqltype() const;
  inline void set_sqltype(::Msg::Protocol::SqlType value);

  // required fixed64 sqlid = 2;
  inline bool has_sqlid() const;
  inline void clear_sqlid();
  static const int kSqlidFieldNumber = 2;
  inline ::google::protobuf::uint64 sqlid() const;
  inline void set_sqlid(::google::protobuf::uint64 value);

  // required string sql = 3;
  inline bool has_sql() const;
  inline void clear_sql();
  static const int kSqlFieldNumber = 3;
  inline const ::std::string& sql() const;
  inline void set_sql(const ::std::string& value);
  inline void set_sql(const char* value);
  inline void set_sql(const char* value, size_t size);
  inline ::std::string* mutable_sql();
  inline ::std::string* release_sql();
  inline void set_allocated_sql(::std::string* sql);

  // @@protoc_insertion_point(class_scope:Msg.Protocol.SqlMsg)
 private:
  inline void set_has_sqltype();
  inline void clear_has_sqltype();
  inline void set_has_sqlid();
  inline void clear_has_sqlid();
  inline void set_has_sql();
  inline void clear_has_sql();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint64 sqlid_;
  ::std::string* sql_;
  int sqltype_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_game_5fdb_2eproto();
  friend void protobuf_AssignDesc_game_5fdb_2eproto();
  friend void protobuf_ShutdownFile_game_5fdb_2eproto();

  void InitAsDefaultInstance();
  static SqlMsg* default_instance_;
};
// -------------------------------------------------------------------

class PostSql : public ::google::protobuf::Message {
 public:
  PostSql();
  virtual ~PostSql();

  PostSql(const PostSql& from);

  inline PostSql& operator=(const PostSql& from) {
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
  static const PostSql& default_instance();

  void Swap(PostSql* other);

  // implements Message ----------------------------------------------

  PostSql* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PostSql& from);
  void MergeFrom(const PostSql& from);
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

  // required fixed32 time = 1;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 1;
  inline ::google::protobuf::uint32 time() const;
  inline void set_time(::google::protobuf::uint32 value);

  // required .Msg.Protocol.SqlDBType dbtype = 2;
  inline bool has_dbtype() const;
  inline void clear_dbtype();
  static const int kDbtypeFieldNumber = 2;
  inline ::Msg::Protocol::SqlDBType dbtype() const;
  inline void set_dbtype(::Msg::Protocol::SqlDBType value);

  // repeated .Msg.Protocol.SqlMsg sqls = 3;
  inline int sqls_size() const;
  inline void clear_sqls();
  static const int kSqlsFieldNumber = 3;
  inline const ::Msg::Protocol::SqlMsg& sqls(int index) const;
  inline ::Msg::Protocol::SqlMsg* mutable_sqls(int index);
  inline ::Msg::Protocol::SqlMsg* add_sqls();
  inline const ::google::protobuf::RepeatedPtrField< ::Msg::Protocol::SqlMsg >&
      sqls() const;
  inline ::google::protobuf::RepeatedPtrField< ::Msg::Protocol::SqlMsg >*
      mutable_sqls();

  // @@protoc_insertion_point(class_scope:Msg.Protocol.PostSql)
 private:
  inline void set_has_time();
  inline void clear_has_time();
  inline void set_has_dbtype();
  inline void clear_has_dbtype();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 time_;
  int dbtype_;
  ::google::protobuf::RepeatedPtrField< ::Msg::Protocol::SqlMsg > sqls_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_game_5fdb_2eproto();
  friend void protobuf_AssignDesc_game_5fdb_2eproto();
  friend void protobuf_ShutdownFile_game_5fdb_2eproto();

  void InitAsDefaultInstance();
  static PostSql* default_instance_;
};
// ===================================================================


// ===================================================================

// SqlMsg

// required .Msg.Protocol.SqlType sqltype = 1 [default = NormalSql];
inline bool SqlMsg::has_sqltype() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SqlMsg::set_has_sqltype() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SqlMsg::clear_has_sqltype() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SqlMsg::clear_sqltype() {
  sqltype_ = 0;
  clear_has_sqltype();
}
inline ::Msg::Protocol::SqlType SqlMsg::sqltype() const {
  return static_cast< ::Msg::Protocol::SqlType >(sqltype_);
}
inline void SqlMsg::set_sqltype(::Msg::Protocol::SqlType value) {
  assert(::Msg::Protocol::SqlType_IsValid(value));
  set_has_sqltype();
  sqltype_ = value;
}

// required fixed64 sqlid = 2;
inline bool SqlMsg::has_sqlid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SqlMsg::set_has_sqlid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SqlMsg::clear_has_sqlid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SqlMsg::clear_sqlid() {
  sqlid_ = GOOGLE_ULONGLONG(0);
  clear_has_sqlid();
}
inline ::google::protobuf::uint64 SqlMsg::sqlid() const {
  return sqlid_;
}
inline void SqlMsg::set_sqlid(::google::protobuf::uint64 value) {
  set_has_sqlid();
  sqlid_ = value;
}

// required string sql = 3;
inline bool SqlMsg::has_sql() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SqlMsg::set_has_sql() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SqlMsg::clear_has_sql() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SqlMsg::clear_sql() {
  if (sql_ != &::google::protobuf::internal::kEmptyString) {
    sql_->clear();
  }
  clear_has_sql();
}
inline const ::std::string& SqlMsg::sql() const {
  return *sql_;
}
inline void SqlMsg::set_sql(const ::std::string& value) {
  set_has_sql();
  if (sql_ == &::google::protobuf::internal::kEmptyString) {
    sql_ = new ::std::string;
  }
  sql_->assign(value);
}
inline void SqlMsg::set_sql(const char* value) {
  set_has_sql();
  if (sql_ == &::google::protobuf::internal::kEmptyString) {
    sql_ = new ::std::string;
  }
  sql_->assign(value);
}
inline void SqlMsg::set_sql(const char* value, size_t size) {
  set_has_sql();
  if (sql_ == &::google::protobuf::internal::kEmptyString) {
    sql_ = new ::std::string;
  }
  sql_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SqlMsg::mutable_sql() {
  set_has_sql();
  if (sql_ == &::google::protobuf::internal::kEmptyString) {
    sql_ = new ::std::string;
  }
  return sql_;
}
inline ::std::string* SqlMsg::release_sql() {
  clear_has_sql();
  if (sql_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = sql_;
    sql_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void SqlMsg::set_allocated_sql(::std::string* sql) {
  if (sql_ != &::google::protobuf::internal::kEmptyString) {
    delete sql_;
  }
  if (sql) {
    set_has_sql();
    sql_ = sql;
  } else {
    clear_has_sql();
    sql_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// PostSql

// required fixed32 time = 1;
inline bool PostSql::has_time() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PostSql::set_has_time() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PostSql::clear_has_time() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PostSql::clear_time() {
  time_ = 0u;
  clear_has_time();
}
inline ::google::protobuf::uint32 PostSql::time() const {
  return time_;
}
inline void PostSql::set_time(::google::protobuf::uint32 value) {
  set_has_time();
  time_ = value;
}

// required .Msg.Protocol.SqlDBType dbtype = 2;
inline bool PostSql::has_dbtype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PostSql::set_has_dbtype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PostSql::clear_has_dbtype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PostSql::clear_dbtype() {
  dbtype_ = 0;
  clear_has_dbtype();
}
inline ::Msg::Protocol::SqlDBType PostSql::dbtype() const {
  return static_cast< ::Msg::Protocol::SqlDBType >(dbtype_);
}
inline void PostSql::set_dbtype(::Msg::Protocol::SqlDBType value) {
  assert(::Msg::Protocol::SqlDBType_IsValid(value));
  set_has_dbtype();
  dbtype_ = value;
}

// repeated .Msg.Protocol.SqlMsg sqls = 3;
inline int PostSql::sqls_size() const {
  return sqls_.size();
}
inline void PostSql::clear_sqls() {
  sqls_.Clear();
}
inline const ::Msg::Protocol::SqlMsg& PostSql::sqls(int index) const {
  return sqls_.Get(index);
}
inline ::Msg::Protocol::SqlMsg* PostSql::mutable_sqls(int index) {
  return sqls_.Mutable(index);
}
inline ::Msg::Protocol::SqlMsg* PostSql::add_sqls() {
  return sqls_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Msg::Protocol::SqlMsg >&
PostSql::sqls() const {
  return sqls_;
}
inline ::google::protobuf::RepeatedPtrField< ::Msg::Protocol::SqlMsg >*
PostSql::mutable_sqls() {
  return &sqls_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol
}  // namespace Msg

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Msg::Protocol::GameToDBMsgId>() {
  return ::Msg::Protocol::GameToDBMsgId_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Msg::Protocol::SqlType>() {
  return ::Msg::Protocol::SqlType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Msg::Protocol::SqlDBType>() {
  return ::Msg::Protocol::SqlDBType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_game_5fdb_2eproto__INCLUDED
