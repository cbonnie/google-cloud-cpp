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
// source: google/cloud/orchestration/airflow/service/v1/environments.proto

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_COMPOSER_MOCKS_MOCK_ENVIRONMENTS_CONNECTION_H
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_COMPOSER_MOCKS_MOCK_ENVIRONMENTS_CONNECTION_H

#include "google/cloud/composer/environments_connection.h"
#include <gmock/gmock.h>

namespace google {
namespace cloud {
namespace composer_mocks {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

class MockEnvironmentsConnection : public composer::EnvironmentsConnection {
 public:
  MOCK_METHOD(
      future<StatusOr<
          google::cloud::orchestration::airflow::service::v1::Environment>>,
      CreateEnvironment,
      (google::cloud::orchestration::airflow::service::v1::
           CreateEnvironmentRequest const& request),
      (override));

  MOCK_METHOD(
      StatusOr<google::cloud::orchestration::airflow::service::v1::Environment>,
      GetEnvironment,
      (google::cloud::orchestration::airflow::service::v1::
           GetEnvironmentRequest const& request),
      (override));

  MOCK_METHOD(
      StreamRange<
          google::cloud::orchestration::airflow::service::v1::Environment>,
      ListEnvironments,
      (google::cloud::orchestration::airflow::service::v1::
           ListEnvironmentsRequest request),
      (override));

  MOCK_METHOD(
      future<StatusOr<
          google::cloud::orchestration::airflow::service::v1::Environment>>,
      UpdateEnvironment,
      (google::cloud::orchestration::airflow::service::v1::
           UpdateEnvironmentRequest const& request),
      (override));

  MOCK_METHOD(future<StatusOr<google::cloud::orchestration::airflow::service::
                                  v1::OperationMetadata>>,
              DeleteEnvironment,
              (google::cloud::orchestration::airflow::service::v1::
                   DeleteEnvironmentRequest const& request),
              (override));
};

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace composer_mocks
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_COMPOSER_MOCKS_MOCK_ENVIRONMENTS_CONNECTION_H
