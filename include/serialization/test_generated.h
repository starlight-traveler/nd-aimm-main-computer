// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_EXAMPLE_EXAMPLE_H_
#define FLATBUFFERS_GENERATED_EXAMPLE_EXAMPLE_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 25,
             "Non-compatible flatbuffers version included");

namespace Example {

struct Message;
struct MessageBuilder;

struct Message FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef MessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXT = 4
  };
  const ::flatbuffers::String *text() const {
    return GetPointer<const ::flatbuffers::String *>(VT_TEXT);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TEXT) &&
           verifier.VerifyString(text()) &&
           verifier.EndTable();
  }
};

struct MessageBuilder {
  typedef Message Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_text(::flatbuffers::Offset<::flatbuffers::String> text) {
    fbb_.AddOffset(Message::VT_TEXT, text);
  }
  explicit MessageBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Message> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Message>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Message> CreateMessage(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> text = 0) {
  MessageBuilder builder_(_fbb);
  builder_.add_text(text);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Message> CreateMessageDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *text = nullptr) {
  auto text__ = text ? _fbb.CreateString(text) : 0;
  return Example::CreateMessage(
      _fbb,
      text__);
}

inline const Example::Message *GetMessage(const void *buf) {
  return ::flatbuffers::GetRoot<Example::Message>(buf);
}

inline const Example::Message *GetSizePrefixedMessage(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<Example::Message>(buf);
}

inline bool VerifyMessageBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Example::Message>(nullptr);
}

inline bool VerifySizePrefixedMessageBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Example::Message>(nullptr);
}

inline void FinishMessageBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Example::Message> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMessageBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<Example::Message> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Example

#endif  // FLATBUFFERS_GENERATED_EXAMPLE_EXAMPLE_H_
