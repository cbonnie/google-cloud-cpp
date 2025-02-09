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
// source: google/monitoring/v3/group_service.proto

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_MONITORING_GROUP_CONNECTION_H
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_MONITORING_GROUP_CONNECTION_H

#include "google/cloud/monitoring/group_connection_idempotency_policy.h"
#include "google/cloud/monitoring/internal/group_retry_traits.h"
#include "google/cloud/monitoring/internal/group_stub.h"
#include "google/cloud/backoff_policy.h"
#include "google/cloud/options.h"
#include "google/cloud/status_or.h"
#include "google/cloud/stream_range.h"
#include "google/cloud/version.h"
#include <memory>

namespace google {
namespace cloud {
namespace monitoring {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

using GroupServiceRetryPolicy =
    ::google::cloud::internal::TraitBasedRetryPolicy<
        monitoring_internal::GroupServiceRetryTraits>;

using GroupServiceLimitedTimeRetryPolicy =
    ::google::cloud::internal::LimitedTimeRetryPolicy<
        monitoring_internal::GroupServiceRetryTraits>;

using GroupServiceLimitedErrorCountRetryPolicy =
    ::google::cloud::internal::LimitedErrorCountRetryPolicy<
        monitoring_internal::GroupServiceRetryTraits>;

class GroupServiceConnection {
 public:
  virtual ~GroupServiceConnection() = 0;

  virtual Options options() { return Options{}; }

  virtual StreamRange<google::monitoring::v3::Group> ListGroups(
      google::monitoring::v3::ListGroupsRequest request);

  virtual StatusOr<google::monitoring::v3::Group> GetGroup(
      google::monitoring::v3::GetGroupRequest const& request);

  virtual StatusOr<google::monitoring::v3::Group> CreateGroup(
      google::monitoring::v3::CreateGroupRequest const& request);

  virtual StatusOr<google::monitoring::v3::Group> UpdateGroup(
      google::monitoring::v3::UpdateGroupRequest const& request);

  virtual Status DeleteGroup(
      google::monitoring::v3::DeleteGroupRequest const& request);

  virtual StreamRange<google::api::MonitoredResource> ListGroupMembers(
      google::monitoring::v3::ListGroupMembersRequest request);
};

std::shared_ptr<GroupServiceConnection> MakeGroupServiceConnection(
    Options options = {});

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace monitoring
}  // namespace cloud
}  // namespace google

namespace google {
namespace cloud {
namespace monitoring_internal {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

std::shared_ptr<monitoring::GroupServiceConnection> MakeGroupServiceConnection(
    std::shared_ptr<GroupServiceStub> stub, Options options);

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace monitoring_internal
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_MONITORING_GROUP_CONNECTION_H
