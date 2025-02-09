// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Generated by the Codegen C++ plugin.
// If you make any local changes, they will be lost.
// source: google/cloud/dialogflow/v2/document.proto

#include "google/cloud/dialogflow_es/documents_connection_idempotency_policy.h"
#include "absl/memory/memory.h"
#include <memory>

namespace google {
namespace cloud {
namespace dialogflow_es {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

using ::google::cloud::Idempotency;

DocumentsConnectionIdempotencyPolicy::~DocumentsConnectionIdempotencyPolicy() =
    default;

namespace {
class DefaultDocumentsConnectionIdempotencyPolicy
    : public DocumentsConnectionIdempotencyPolicy {
 public:
  ~DefaultDocumentsConnectionIdempotencyPolicy() override = default;

  /// Create a new copy of this object.
  std::unique_ptr<DocumentsConnectionIdempotencyPolicy> clone() const override {
    return absl::make_unique<DefaultDocumentsConnectionIdempotencyPolicy>(
        *this);
  }

  Idempotency ListDocuments(
      google::cloud::dialogflow::v2::ListDocumentsRequest) override {
    return Idempotency::kIdempotent;
  }

  Idempotency GetDocument(
      google::cloud::dialogflow::v2::GetDocumentRequest const&) override {
    return Idempotency::kIdempotent;
  }

  Idempotency CreateDocument(
      google::cloud::dialogflow::v2::CreateDocumentRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency ImportDocuments(
      google::cloud::dialogflow::v2::ImportDocumentsRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency DeleteDocument(
      google::cloud::dialogflow::v2::DeleteDocumentRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency UpdateDocument(
      google::cloud::dialogflow::v2::UpdateDocumentRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency ReloadDocument(
      google::cloud::dialogflow::v2::ReloadDocumentRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency ExportDocument(
      google::cloud::dialogflow::v2::ExportDocumentRequest const&) override {
    return Idempotency::kNonIdempotent;
  }
};
}  // namespace

std::unique_ptr<DocumentsConnectionIdempotencyPolicy>
MakeDefaultDocumentsConnectionIdempotencyPolicy() {
  return absl::make_unique<DefaultDocumentsConnectionIdempotencyPolicy>();
}

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace dialogflow_es
}  // namespace cloud
}  // namespace google
