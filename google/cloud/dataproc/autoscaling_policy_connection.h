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
// source: google/cloud/dataproc/v1/autoscaling_policies.proto

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_DATAPROC_AUTOSCALING_POLICY_CONNECTION_H
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_DATAPROC_AUTOSCALING_POLICY_CONNECTION_H

#include "google/cloud/dataproc/autoscaling_policy_connection_idempotency_policy.h"
#include "google/cloud/dataproc/internal/autoscaling_policy_retry_traits.h"
#include "google/cloud/dataproc/internal/autoscaling_policy_stub.h"
#include "google/cloud/backoff_policy.h"
#include "google/cloud/options.h"
#include "google/cloud/status_or.h"
#include "google/cloud/stream_range.h"
#include "google/cloud/version.h"
#include <memory>

namespace google {
namespace cloud {
namespace dataproc {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

using AutoscalingPolicyServiceRetryPolicy =
    ::google::cloud::internal::TraitBasedRetryPolicy<
        dataproc_internal::AutoscalingPolicyServiceRetryTraits>;

using AutoscalingPolicyServiceLimitedTimeRetryPolicy =
    ::google::cloud::internal::LimitedTimeRetryPolicy<
        dataproc_internal::AutoscalingPolicyServiceRetryTraits>;

using AutoscalingPolicyServiceLimitedErrorCountRetryPolicy =
    ::google::cloud::internal::LimitedErrorCountRetryPolicy<
        dataproc_internal::AutoscalingPolicyServiceRetryTraits>;

class AutoscalingPolicyServiceConnection {
 public:
  virtual ~AutoscalingPolicyServiceConnection() = 0;

  virtual Options options() { return Options{}; }

  virtual StatusOr<google::cloud::dataproc::v1::AutoscalingPolicy>
  CreateAutoscalingPolicy(
      google::cloud::dataproc::v1::CreateAutoscalingPolicyRequest const&
          request);

  virtual StatusOr<google::cloud::dataproc::v1::AutoscalingPolicy>
  UpdateAutoscalingPolicy(
      google::cloud::dataproc::v1::UpdateAutoscalingPolicyRequest const&
          request);

  virtual StatusOr<google::cloud::dataproc::v1::AutoscalingPolicy>
  GetAutoscalingPolicy(
      google::cloud::dataproc::v1::GetAutoscalingPolicyRequest const& request);

  virtual StreamRange<google::cloud::dataproc::v1::AutoscalingPolicy>
  ListAutoscalingPolicies(
      google::cloud::dataproc::v1::ListAutoscalingPoliciesRequest request);

  virtual Status DeleteAutoscalingPolicy(
      google::cloud::dataproc::v1::DeleteAutoscalingPolicyRequest const&
          request);
};

std::shared_ptr<AutoscalingPolicyServiceConnection>
MakeAutoscalingPolicyServiceConnection(Options options = {});

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace dataproc
}  // namespace cloud
}  // namespace google

namespace google {
namespace cloud {
namespace dataproc_internal {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

std::shared_ptr<dataproc::AutoscalingPolicyServiceConnection>
MakeAutoscalingPolicyServiceConnection(
    std::shared_ptr<AutoscalingPolicyServiceStub> stub, Options options);

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace dataproc_internal
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_DATAPROC_AUTOSCALING_POLICY_CONNECTION_H
