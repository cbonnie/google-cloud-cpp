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
// source: google/devtools/artifactregistry/v1/service.proto

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_ARTIFACTREGISTRY_ARTIFACT_REGISTRY_CONNECTION_H
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_ARTIFACTREGISTRY_ARTIFACT_REGISTRY_CONNECTION_H

#include "google/cloud/artifactregistry/artifact_registry_connection_idempotency_policy.h"
#include "google/cloud/artifactregistry/internal/artifact_registry_retry_traits.h"
#include "google/cloud/artifactregistry/internal/artifact_registry_stub.h"
#include "google/cloud/backoff_policy.h"
#include "google/cloud/future.h"
#include "google/cloud/options.h"
#include "google/cloud/polling_policy.h"
#include "google/cloud/status_or.h"
#include "google/cloud/stream_range.h"
#include "google/cloud/version.h"
#include <google/longrunning/operations.grpc.pb.h>
#include <memory>

namespace google {
namespace cloud {
namespace artifactregistry {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

using ArtifactRegistryRetryPolicy =
    ::google::cloud::internal::TraitBasedRetryPolicy<
        artifactregistry_internal::ArtifactRegistryRetryTraits>;

using ArtifactRegistryLimitedTimeRetryPolicy =
    ::google::cloud::internal::LimitedTimeRetryPolicy<
        artifactregistry_internal::ArtifactRegistryRetryTraits>;

using ArtifactRegistryLimitedErrorCountRetryPolicy =
    ::google::cloud::internal::LimitedErrorCountRetryPolicy<
        artifactregistry_internal::ArtifactRegistryRetryTraits>;

class ArtifactRegistryConnection {
 public:
  virtual ~ArtifactRegistryConnection() = 0;

  virtual Options options() { return Options{}; }

  virtual StreamRange<google::devtools::artifactregistry::v1::DockerImage>
  ListDockerImages(
      google::devtools::artifactregistry::v1::ListDockerImagesRequest request);

  virtual StatusOr<google::devtools::artifactregistry::v1::DockerImage>
  GetDockerImage(
      google::devtools::artifactregistry::v1::GetDockerImageRequest const&
          request);

  virtual future<StatusOr<
      google::devtools::artifactregistry::v1::ImportAptArtifactsResponse>>
  ImportAptArtifacts(
      google::devtools::artifactregistry::v1::ImportAptArtifactsRequest const&
          request);

  virtual future<StatusOr<
      google::devtools::artifactregistry::v1::ImportYumArtifactsResponse>>
  ImportYumArtifacts(
      google::devtools::artifactregistry::v1::ImportYumArtifactsRequest const&
          request);

  virtual StreamRange<google::devtools::artifactregistry::v1::Repository>
  ListRepositories(
      google::devtools::artifactregistry::v1::ListRepositoriesRequest request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Repository>
  GetRepository(
      google::devtools::artifactregistry::v1::GetRepositoryRequest const&
          request);

  virtual future<StatusOr<google::devtools::artifactregistry::v1::Repository>>
  CreateRepository(
      google::devtools::artifactregistry::v1::CreateRepositoryRequest const&
          request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Repository>
  UpdateRepository(
      google::devtools::artifactregistry::v1::UpdateRepositoryRequest const&
          request);

  virtual future<
      StatusOr<google::devtools::artifactregistry::v1::OperationMetadata>>
  DeleteRepository(
      google::devtools::artifactregistry::v1::DeleteRepositoryRequest const&
          request);

  virtual StreamRange<google::devtools::artifactregistry::v1::Package>
  ListPackages(
      google::devtools::artifactregistry::v1::ListPackagesRequest request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Package> GetPackage(
      google::devtools::artifactregistry::v1::GetPackageRequest const& request);

  virtual future<
      StatusOr<google::devtools::artifactregistry::v1::OperationMetadata>>
  DeletePackage(
      google::devtools::artifactregistry::v1::DeletePackageRequest const&
          request);

  virtual StreamRange<google::devtools::artifactregistry::v1::Version>
  ListVersions(
      google::devtools::artifactregistry::v1::ListVersionsRequest request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Version> GetVersion(
      google::devtools::artifactregistry::v1::GetVersionRequest const& request);

  virtual future<
      StatusOr<google::devtools::artifactregistry::v1::OperationMetadata>>
  DeleteVersion(
      google::devtools::artifactregistry::v1::DeleteVersionRequest const&
          request);

  virtual StreamRange<google::devtools::artifactregistry::v1::File> ListFiles(
      google::devtools::artifactregistry::v1::ListFilesRequest request);

  virtual StatusOr<google::devtools::artifactregistry::v1::File> GetFile(
      google::devtools::artifactregistry::v1::GetFileRequest const& request);

  virtual StreamRange<google::devtools::artifactregistry::v1::Tag> ListTags(
      google::devtools::artifactregistry::v1::ListTagsRequest request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Tag> GetTag(
      google::devtools::artifactregistry::v1::GetTagRequest const& request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Tag> CreateTag(
      google::devtools::artifactregistry::v1::CreateTagRequest const& request);

  virtual StatusOr<google::devtools::artifactregistry::v1::Tag> UpdateTag(
      google::devtools::artifactregistry::v1::UpdateTagRequest const& request);

  virtual Status DeleteTag(
      google::devtools::artifactregistry::v1::DeleteTagRequest const& request);

  virtual StatusOr<google::iam::v1::Policy> SetIamPolicy(
      google::iam::v1::SetIamPolicyRequest const& request);

  virtual StatusOr<google::iam::v1::Policy> GetIamPolicy(
      google::iam::v1::GetIamPolicyRequest const& request);

  virtual StatusOr<google::iam::v1::TestIamPermissionsResponse>
  TestIamPermissions(google::iam::v1::TestIamPermissionsRequest const& request);

  virtual StatusOr<google::devtools::artifactregistry::v1::ProjectSettings>
  GetProjectSettings(
      google::devtools::artifactregistry::v1::GetProjectSettingsRequest const&
          request);

  virtual StatusOr<google::devtools::artifactregistry::v1::ProjectSettings>
  UpdateProjectSettings(google::devtools::artifactregistry::v1::
                            UpdateProjectSettingsRequest const& request);
};

std::shared_ptr<ArtifactRegistryConnection> MakeArtifactRegistryConnection(
    Options options = {});

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace artifactregistry
}  // namespace cloud
}  // namespace google

namespace google {
namespace cloud {
namespace artifactregistry_internal {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

std::shared_ptr<artifactregistry::ArtifactRegistryConnection>
MakeArtifactRegistryConnection(std::shared_ptr<ArtifactRegistryStub> stub,
                               Options options);

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace artifactregistry_internal
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_ARTIFACTREGISTRY_ARTIFACT_REGISTRY_CONNECTION_H
