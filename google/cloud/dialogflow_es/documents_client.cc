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

#include "google/cloud/dialogflow_es/documents_client.h"
#include "google/cloud/dialogflow_es/internal/documents_option_defaults.h"
#include <memory>

namespace google {
namespace cloud {
namespace dialogflow_es {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

DocumentsClient::DocumentsClient(
    std::shared_ptr<DocumentsConnection> connection, Options opts)
    : connection_(std::move(connection)),
      options_(internal::MergeOptions(
          std::move(opts), dialogflow_es_internal::DocumentsDefaultOptions(
                               connection_->options()))) {}
DocumentsClient::~DocumentsClient() = default;

StreamRange<google::cloud::dialogflow::v2::Document>
DocumentsClient::ListDocuments(std::string const& parent, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  google::cloud::dialogflow::v2::ListDocumentsRequest request;
  request.set_parent(parent);
  return connection_->ListDocuments(request);
}

StreamRange<google::cloud::dialogflow::v2::Document>
DocumentsClient::ListDocuments(
    google::cloud::dialogflow::v2::ListDocumentsRequest request, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->ListDocuments(std::move(request));
}

StatusOr<google::cloud::dialogflow::v2::Document> DocumentsClient::GetDocument(
    std::string const& name, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  google::cloud::dialogflow::v2::GetDocumentRequest request;
  request.set_name(name);
  return connection_->GetDocument(request);
}

StatusOr<google::cloud::dialogflow::v2::Document> DocumentsClient::GetDocument(
    google::cloud::dialogflow::v2::GetDocumentRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->GetDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::CreateDocument(
    std::string const& parent,
    google::cloud::dialogflow::v2::Document const& document, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  google::cloud::dialogflow::v2::CreateDocumentRequest request;
  request.set_parent(parent);
  *request.mutable_document() = document;
  return connection_->CreateDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::CreateDocument(
    google::cloud::dialogflow::v2::CreateDocumentRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->CreateDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::ImportDocumentsResponse>>
DocumentsClient::ImportDocuments(
    google::cloud::dialogflow::v2::ImportDocumentsRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->ImportDocuments(request);
}

future<StatusOr<google::cloud::dialogflow::v2::KnowledgeOperationMetadata>>
DocumentsClient::DeleteDocument(std::string const& name, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  google::cloud::dialogflow::v2::DeleteDocumentRequest request;
  request.set_name(name);
  return connection_->DeleteDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::KnowledgeOperationMetadata>>
DocumentsClient::DeleteDocument(
    google::cloud::dialogflow::v2::DeleteDocumentRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->DeleteDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::UpdateDocument(
    google::cloud::dialogflow::v2::Document const& document,
    google::protobuf::FieldMask const& update_mask, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  google::cloud::dialogflow::v2::UpdateDocumentRequest request;
  *request.mutable_document() = document;
  *request.mutable_update_mask() = update_mask;
  return connection_->UpdateDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::UpdateDocument(
    google::cloud::dialogflow::v2::UpdateDocumentRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->UpdateDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::ReloadDocument(std::string const& name,
                                std::string const& content_uri, Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  google::cloud::dialogflow::v2::ReloadDocumentRequest request;
  request.set_name(name);
  request.set_content_uri(content_uri);
  return connection_->ReloadDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::ReloadDocument(
    google::cloud::dialogflow::v2::ReloadDocumentRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->ReloadDocument(request);
}

future<StatusOr<google::cloud::dialogflow::v2::Document>>
DocumentsClient::ExportDocument(
    google::cloud::dialogflow::v2::ExportDocumentRequest const& request,
    Options opts) {
  internal::OptionsSpan span(internal::MergeOptions(std::move(opts), options_));
  return connection_->ExportDocument(request);
}

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace dialogflow_es
}  // namespace cloud
}  // namespace google
