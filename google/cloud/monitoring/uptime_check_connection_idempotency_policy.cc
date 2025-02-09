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
// source: google/monitoring/v3/uptime_service.proto

#include "google/cloud/monitoring/uptime_check_connection_idempotency_policy.h"
#include "absl/memory/memory.h"
#include <memory>

namespace google {
namespace cloud {
namespace monitoring {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

using ::google::cloud::Idempotency;

UptimeCheckServiceConnectionIdempotencyPolicy::
    ~UptimeCheckServiceConnectionIdempotencyPolicy() = default;

namespace {
class DefaultUptimeCheckServiceConnectionIdempotencyPolicy
    : public UptimeCheckServiceConnectionIdempotencyPolicy {
 public:
  ~DefaultUptimeCheckServiceConnectionIdempotencyPolicy() override = default;

  /// Create a new copy of this object.
  std::unique_ptr<UptimeCheckServiceConnectionIdempotencyPolicy> clone()
      const override {
    return absl::make_unique<
        DefaultUptimeCheckServiceConnectionIdempotencyPolicy>(*this);
  }

  Idempotency ListUptimeCheckConfigs(
      google::monitoring::v3::ListUptimeCheckConfigsRequest) override {
    return Idempotency::kIdempotent;
  }

  Idempotency GetUptimeCheckConfig(
      google::monitoring::v3::GetUptimeCheckConfigRequest const&) override {
    return Idempotency::kIdempotent;
  }

  Idempotency CreateUptimeCheckConfig(
      google::monitoring::v3::CreateUptimeCheckConfigRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency UpdateUptimeCheckConfig(
      google::monitoring::v3::UpdateUptimeCheckConfigRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency DeleteUptimeCheckConfig(
      google::monitoring::v3::DeleteUptimeCheckConfigRequest const&) override {
    return Idempotency::kNonIdempotent;
  }

  Idempotency ListUptimeCheckIps(
      google::monitoring::v3::ListUptimeCheckIpsRequest) override {
    return Idempotency::kIdempotent;
  }
};
}  // namespace

std::unique_ptr<UptimeCheckServiceConnectionIdempotencyPolicy>
MakeDefaultUptimeCheckServiceConnectionIdempotencyPolicy() {
  return absl::make_unique<
      DefaultUptimeCheckServiceConnectionIdempotencyPolicy>();
}

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace monitoring
}  // namespace cloud
}  // namespace google
