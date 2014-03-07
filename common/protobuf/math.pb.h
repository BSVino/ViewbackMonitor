// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common/protobuf/math.proto

#ifndef PROTOBUF_common_2fprotobuf_2fmath_2eproto__INCLUDED
#define PROTOBUF_common_2fprotobuf_2fmath_2eproto__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace tinker {
namespace protobuf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_common_2fprotobuf_2fmath_2eproto();
void protobuf_AssignDesc_common_2fprotobuf_2fmath_2eproto();
void protobuf_ShutdownFile_common_2fprotobuf_2fmath_2eproto();

class Vector;
class Vector2D;
class EAngle;
class AABB;
class TRS;

// ===================================================================

class Vector : public ::google::protobuf::Message {
 public:
  Vector();
  virtual ~Vector();

  Vector(const Vector& from);

  inline Vector& operator=(const Vector& from) {
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
  static const Vector& default_instance();

  void Swap(Vector* other);

  // implements Message ----------------------------------------------

  Vector* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Vector& from);
  void MergeFrom(const Vector& from);
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

  // required float x = 1;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 1;
  inline float x() const;
  inline void set_x(float value);

  // required float y = 2;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline float y() const;
  inline void set_y(float value);

  // required float z = 3;
  inline bool has_z() const;
  inline void clear_z();
  static const int kZFieldNumber = 3;
  inline float z() const;
  inline void set_z(float value);

  // @@protoc_insertion_point(class_scope:tinker.protobuf.Vector)
 private:
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  inline void set_has_z();
  inline void clear_has_z();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  float x_;
  float y_;
  float z_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_AssignDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_ShutdownFile_common_2fprotobuf_2fmath_2eproto();

  void InitAsDefaultInstance();
  static Vector* default_instance_;
};
// -------------------------------------------------------------------

class Vector2D : public ::google::protobuf::Message {
 public:
  Vector2D();
  virtual ~Vector2D();

  Vector2D(const Vector2D& from);

  inline Vector2D& operator=(const Vector2D& from) {
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
  static const Vector2D& default_instance();

  void Swap(Vector2D* other);

  // implements Message ----------------------------------------------

  Vector2D* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Vector2D& from);
  void MergeFrom(const Vector2D& from);
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

  // required float x = 1;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 1;
  inline float x() const;
  inline void set_x(float value);

  // required float y = 2;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline float y() const;
  inline void set_y(float value);

  // @@protoc_insertion_point(class_scope:tinker.protobuf.Vector2D)
 private:
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  float x_;
  float y_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_AssignDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_ShutdownFile_common_2fprotobuf_2fmath_2eproto();

  void InitAsDefaultInstance();
  static Vector2D* default_instance_;
};
// -------------------------------------------------------------------

class EAngle : public ::google::protobuf::Message {
 public:
  EAngle();
  virtual ~EAngle();

  EAngle(const EAngle& from);

  inline EAngle& operator=(const EAngle& from) {
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
  static const EAngle& default_instance();

  void Swap(EAngle* other);

  // implements Message ----------------------------------------------

  EAngle* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const EAngle& from);
  void MergeFrom(const EAngle& from);
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

  // required float p = 1;
  inline bool has_p() const;
  inline void clear_p();
  static const int kPFieldNumber = 1;
  inline float p() const;
  inline void set_p(float value);

  // required float y = 2;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline float y() const;
  inline void set_y(float value);

  // required float r = 3;
  inline bool has_r() const;
  inline void clear_r();
  static const int kRFieldNumber = 3;
  inline float r() const;
  inline void set_r(float value);

  // @@protoc_insertion_point(class_scope:tinker.protobuf.EAngle)
 private:
  inline void set_has_p();
  inline void clear_has_p();
  inline void set_has_y();
  inline void clear_has_y();
  inline void set_has_r();
  inline void clear_has_r();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  float p_;
  float y_;
  float r_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_AssignDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_ShutdownFile_common_2fprotobuf_2fmath_2eproto();

  void InitAsDefaultInstance();
  static EAngle* default_instance_;
};
// -------------------------------------------------------------------

class AABB : public ::google::protobuf::Message {
 public:
  AABB();
  virtual ~AABB();

  AABB(const AABB& from);

  inline AABB& operator=(const AABB& from) {
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
  static const AABB& default_instance();

  void Swap(AABB* other);

  // implements Message ----------------------------------------------

  AABB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AABB& from);
  void MergeFrom(const AABB& from);
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

  // required .tinker.protobuf.Vector min = 1;
  inline bool has_min() const;
  inline void clear_min();
  static const int kMinFieldNumber = 1;
  inline const ::tinker::protobuf::Vector& min() const;
  inline ::tinker::protobuf::Vector* mutable_min();
  inline ::tinker::protobuf::Vector* release_min();
  inline void set_allocated_min(::tinker::protobuf::Vector* min);

  // required .tinker.protobuf.Vector max = 2;
  inline bool has_max() const;
  inline void clear_max();
  static const int kMaxFieldNumber = 2;
  inline const ::tinker::protobuf::Vector& max() const;
  inline ::tinker::protobuf::Vector* mutable_max();
  inline ::tinker::protobuf::Vector* release_max();
  inline void set_allocated_max(::tinker::protobuf::Vector* max);

  // @@protoc_insertion_point(class_scope:tinker.protobuf.AABB)
 private:
  inline void set_has_min();
  inline void clear_has_min();
  inline void set_has_max();
  inline void clear_has_max();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::tinker::protobuf::Vector* min_;
  ::tinker::protobuf::Vector* max_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_AssignDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_ShutdownFile_common_2fprotobuf_2fmath_2eproto();

  void InitAsDefaultInstance();
  static AABB* default_instance_;
};
// -------------------------------------------------------------------

class TRS : public ::google::protobuf::Message {
 public:
  TRS();
  virtual ~TRS();

  TRS(const TRS& from);

  inline TRS& operator=(const TRS& from) {
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
  static const TRS& default_instance();

  void Swap(TRS* other);

  // implements Message ----------------------------------------------

  TRS* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TRS& from);
  void MergeFrom(const TRS& from);
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

  // required .tinker.protobuf.Vector translation = 1;
  inline bool has_translation() const;
  inline void clear_translation();
  static const int kTranslationFieldNumber = 1;
  inline const ::tinker::protobuf::Vector& translation() const;
  inline ::tinker::protobuf::Vector* mutable_translation();
  inline ::tinker::protobuf::Vector* release_translation();
  inline void set_allocated_translation(::tinker::protobuf::Vector* translation);

  // required .tinker.protobuf.EAngle rotation = 2;
  inline bool has_rotation() const;
  inline void clear_rotation();
  static const int kRotationFieldNumber = 2;
  inline const ::tinker::protobuf::EAngle& rotation() const;
  inline ::tinker::protobuf::EAngle* mutable_rotation();
  inline ::tinker::protobuf::EAngle* release_rotation();
  inline void set_allocated_rotation(::tinker::protobuf::EAngle* rotation);

  // required .tinker.protobuf.Vector scaling = 3;
  inline bool has_scaling() const;
  inline void clear_scaling();
  static const int kScalingFieldNumber = 3;
  inline const ::tinker::protobuf::Vector& scaling() const;
  inline ::tinker::protobuf::Vector* mutable_scaling();
  inline ::tinker::protobuf::Vector* release_scaling();
  inline void set_allocated_scaling(::tinker::protobuf::Vector* scaling);

  // @@protoc_insertion_point(class_scope:tinker.protobuf.TRS)
 private:
  inline void set_has_translation();
  inline void clear_has_translation();
  inline void set_has_rotation();
  inline void clear_has_rotation();
  inline void set_has_scaling();
  inline void clear_has_scaling();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::tinker::protobuf::Vector* translation_;
  ::tinker::protobuf::EAngle* rotation_;
  ::tinker::protobuf::Vector* scaling_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_AssignDesc_common_2fprotobuf_2fmath_2eproto();
  friend void protobuf_ShutdownFile_common_2fprotobuf_2fmath_2eproto();

  void InitAsDefaultInstance();
  static TRS* default_instance_;
};
// ===================================================================


// ===================================================================

// Vector

// required float x = 1;
inline bool Vector::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Vector::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Vector::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Vector::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline float Vector::x() const {
  return x_;
}
inline void Vector::set_x(float value) {
  set_has_x();
  x_ = value;
}

// required float y = 2;
inline bool Vector::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Vector::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Vector::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Vector::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline float Vector::y() const {
  return y_;
}
inline void Vector::set_y(float value) {
  set_has_y();
  y_ = value;
}

// required float z = 3;
inline bool Vector::has_z() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Vector::set_has_z() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Vector::clear_has_z() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Vector::clear_z() {
  z_ = 0;
  clear_has_z();
}
inline float Vector::z() const {
  return z_;
}
inline void Vector::set_z(float value) {
  set_has_z();
  z_ = value;
}

// -------------------------------------------------------------------

// Vector2D

// required float x = 1;
inline bool Vector2D::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Vector2D::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Vector2D::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Vector2D::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline float Vector2D::x() const {
  return x_;
}
inline void Vector2D::set_x(float value) {
  set_has_x();
  x_ = value;
}

// required float y = 2;
inline bool Vector2D::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Vector2D::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Vector2D::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Vector2D::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline float Vector2D::y() const {
  return y_;
}
inline void Vector2D::set_y(float value) {
  set_has_y();
  y_ = value;
}

// -------------------------------------------------------------------

// EAngle

// required float p = 1;
inline bool EAngle::has_p() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void EAngle::set_has_p() {
  _has_bits_[0] |= 0x00000001u;
}
inline void EAngle::clear_has_p() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void EAngle::clear_p() {
  p_ = 0;
  clear_has_p();
}
inline float EAngle::p() const {
  return p_;
}
inline void EAngle::set_p(float value) {
  set_has_p();
  p_ = value;
}

// required float y = 2;
inline bool EAngle::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void EAngle::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void EAngle::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void EAngle::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline float EAngle::y() const {
  return y_;
}
inline void EAngle::set_y(float value) {
  set_has_y();
  y_ = value;
}

// required float r = 3;
inline bool EAngle::has_r() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void EAngle::set_has_r() {
  _has_bits_[0] |= 0x00000004u;
}
inline void EAngle::clear_has_r() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void EAngle::clear_r() {
  r_ = 0;
  clear_has_r();
}
inline float EAngle::r() const {
  return r_;
}
inline void EAngle::set_r(float value) {
  set_has_r();
  r_ = value;
}

// -------------------------------------------------------------------

// AABB

// required .tinker.protobuf.Vector min = 1;
inline bool AABB::has_min() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AABB::set_has_min() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AABB::clear_has_min() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AABB::clear_min() {
  if (min_ != NULL) min_->::tinker::protobuf::Vector::Clear();
  clear_has_min();
}
inline const ::tinker::protobuf::Vector& AABB::min() const {
  return min_ != NULL ? *min_ : *default_instance_->min_;
}
inline ::tinker::protobuf::Vector* AABB::mutable_min() {
  set_has_min();
  if (min_ == NULL) min_ = new ::tinker::protobuf::Vector;
  return min_;
}
inline ::tinker::protobuf::Vector* AABB::release_min() {
  clear_has_min();
  ::tinker::protobuf::Vector* temp = min_;
  min_ = NULL;
  return temp;
}
inline void AABB::set_allocated_min(::tinker::protobuf::Vector* min) {
  delete min_;
  min_ = min;
  if (min) {
    set_has_min();
  } else {
    clear_has_min();
  }
}

// required .tinker.protobuf.Vector max = 2;
inline bool AABB::has_max() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AABB::set_has_max() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AABB::clear_has_max() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AABB::clear_max() {
  if (max_ != NULL) max_->::tinker::protobuf::Vector::Clear();
  clear_has_max();
}
inline const ::tinker::protobuf::Vector& AABB::max() const {
  return max_ != NULL ? *max_ : *default_instance_->max_;
}
inline ::tinker::protobuf::Vector* AABB::mutable_max() {
  set_has_max();
  if (max_ == NULL) max_ = new ::tinker::protobuf::Vector;
  return max_;
}
inline ::tinker::protobuf::Vector* AABB::release_max() {
  clear_has_max();
  ::tinker::protobuf::Vector* temp = max_;
  max_ = NULL;
  return temp;
}
inline void AABB::set_allocated_max(::tinker::protobuf::Vector* max) {
  delete max_;
  max_ = max;
  if (max) {
    set_has_max();
  } else {
    clear_has_max();
  }
}

// -------------------------------------------------------------------

// TRS

// required .tinker.protobuf.Vector translation = 1;
inline bool TRS::has_translation() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TRS::set_has_translation() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TRS::clear_has_translation() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TRS::clear_translation() {
  if (translation_ != NULL) translation_->::tinker::protobuf::Vector::Clear();
  clear_has_translation();
}
inline const ::tinker::protobuf::Vector& TRS::translation() const {
  return translation_ != NULL ? *translation_ : *default_instance_->translation_;
}
inline ::tinker::protobuf::Vector* TRS::mutable_translation() {
  set_has_translation();
  if (translation_ == NULL) translation_ = new ::tinker::protobuf::Vector;
  return translation_;
}
inline ::tinker::protobuf::Vector* TRS::release_translation() {
  clear_has_translation();
  ::tinker::protobuf::Vector* temp = translation_;
  translation_ = NULL;
  return temp;
}
inline void TRS::set_allocated_translation(::tinker::protobuf::Vector* translation) {
  delete translation_;
  translation_ = translation;
  if (translation) {
    set_has_translation();
  } else {
    clear_has_translation();
  }
}

// required .tinker.protobuf.EAngle rotation = 2;
inline bool TRS::has_rotation() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TRS::set_has_rotation() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TRS::clear_has_rotation() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TRS::clear_rotation() {
  if (rotation_ != NULL) rotation_->::tinker::protobuf::EAngle::Clear();
  clear_has_rotation();
}
inline const ::tinker::protobuf::EAngle& TRS::rotation() const {
  return rotation_ != NULL ? *rotation_ : *default_instance_->rotation_;
}
inline ::tinker::protobuf::EAngle* TRS::mutable_rotation() {
  set_has_rotation();
  if (rotation_ == NULL) rotation_ = new ::tinker::protobuf::EAngle;
  return rotation_;
}
inline ::tinker::protobuf::EAngle* TRS::release_rotation() {
  clear_has_rotation();
  ::tinker::protobuf::EAngle* temp = rotation_;
  rotation_ = NULL;
  return temp;
}
inline void TRS::set_allocated_rotation(::tinker::protobuf::EAngle* rotation) {
  delete rotation_;
  rotation_ = rotation;
  if (rotation) {
    set_has_rotation();
  } else {
    clear_has_rotation();
  }
}

// required .tinker.protobuf.Vector scaling = 3;
inline bool TRS::has_scaling() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void TRS::set_has_scaling() {
  _has_bits_[0] |= 0x00000004u;
}
inline void TRS::clear_has_scaling() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void TRS::clear_scaling() {
  if (scaling_ != NULL) scaling_->::tinker::protobuf::Vector::Clear();
  clear_has_scaling();
}
inline const ::tinker::protobuf::Vector& TRS::scaling() const {
  return scaling_ != NULL ? *scaling_ : *default_instance_->scaling_;
}
inline ::tinker::protobuf::Vector* TRS::mutable_scaling() {
  set_has_scaling();
  if (scaling_ == NULL) scaling_ = new ::tinker::protobuf::Vector;
  return scaling_;
}
inline ::tinker::protobuf::Vector* TRS::release_scaling() {
  clear_has_scaling();
  ::tinker::protobuf::Vector* temp = scaling_;
  scaling_ = NULL;
  return temp;
}
inline void TRS::set_allocated_scaling(::tinker::protobuf::Vector* scaling) {
  delete scaling_;
  scaling_ = scaling;
  if (scaling) {
    set_has_scaling();
  } else {
    clear_has_scaling();
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf
}  // namespace tinker

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_common_2fprotobuf_2fmath_2eproto__INCLUDED
